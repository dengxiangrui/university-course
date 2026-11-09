/*
 * 计算机图形学 实验一：OpenGL 基本使用
 * 重庆大学  邓翔睿  20252757
 *
 * 功能：
 *  1. 搭建 OpenGL 基础程序框架（freeglut），渲染包含地面、茶壶、立方体、
 *     球体、圆环的基础场景；
 *  2. 通过鼠标右键菜单交互选择不同着色模型（Flat / Gouraud / 无光照固定色）；
 *  3. 通过鼠标对场景进行几何变换：左键拖动旋转、Shift+左键拖动平移、
 *     Ctrl+左键拖动缩放、滚轮缩放视距；
 *  4. 通过菜单/键盘选择纹理贴图（棋盘格 / 渐变 / 条纹 / 关闭，均为程序生成纹理），
 *     纹理作用于立方体、球体与地面；
 *  5. 键盘辅助：R 复位视角，T 循环切换纹理，S 循环切换着色模型，
 *     空格 启停自动旋转，ESC 退出。
 *
 * 编译（MSYS2 ucrt64 gcc）：
 *   gcc src/main.c -o build/GraphicsExp1.exe -lfreeglut -lglu32 -lopengl32
 */

#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------- 全局状态 ------------------------- */
static float g_rotX = -12.0f, g_rotY = 30.0f;   /* 场景旋转角（度） */
static float g_panX = 0.0f, g_panY = 0.0f;      /* 平移量 */
static float g_scale = 1.0f;                     /* 整体缩放 */
static float g_dist = 9.0f;                      /* 视距（滚轮缩放） */

static int   g_lastX = 0, g_lastY = 0;           /* 鼠标上一次位置 */
static int   g_autoSpin = 1;                     /* 自动旋转开关 */

/* 着色模型：0=Flat(粗糙着色) 1=Gouraud(平滑着色) 2=无光照固定色 */
static int   g_shading = 1;
static const char *g_shadingName[3] = {"Flat", "Gouraud(Smooth)", "No-Lighting"};

/* 纹理：0=棋盘格 1=双色渐变 2=斜条纹 3=关闭 */
#define NUM_TEX 4
#define TEX_SIZE 256
static unsigned char g_texData[NUM_TEX][TEX_SIZE][TEX_SIZE][4];
static GLuint g_texName[NUM_TEX];
static int   g_texOn = 1;                        /* 是否启用纹理 */
static int   g_texIdx = 0;                       /* 当前纹理编号 */
static const char *g_texNameStr[4] = {"Checkerboard", "Gradient", "Stripe", "Off"};

/* ------------------------- 程序化纹理 ------------------------- */
static void buildTextures(void)
{
    int x, y, i;
    /* 0. 棋盘格 */
    for (y = 0; y < TEX_SIZE; ++y)
        for (x = 0; x < TEX_SIZE; ++x) {
            int c = ((x / 32) + (y / 32)) & 1;
            unsigned char v = c ? 235 : 55;
            g_texData[0][y][x][0] = v; g_texData[0][y][x][1] = v;
            g_texData[0][y][x][2] = v; g_texData[0][y][x][3] = 255;
        }
    /* 1. 红蓝双色渐变 */
    for (y = 0; y < TEX_SIZE; ++y)
        for (x = 0; x < TEX_SIZE; ++x) {
            g_texData[1][y][x][0] = (unsigned char)(255 * x / (TEX_SIZE - 1));
            g_texData[1][y][x][1] = (unsigned char)(120 * (x + y) / (2 * TEX_SIZE - 2));
            g_texData[1][y][x][2] = (unsigned char)(255 - 255 * x / (TEX_SIZE - 1));
            g_texData[1][y][x][3] = 255;
        }
    /* 2. 45 度斜条纹 */
    for (y = 0; y < TEX_SIZE; ++y)
        for (x = 0; x < TEX_SIZE; ++x) {
            int c = (((x + y) / 24) & 1) ? 1 : 0;
            g_texData[2][y][x][0] = c ? 40 : 220;
            g_texData[2][y][x][1] = c ? 90 : 220;
            g_texData[2][y][x][2] = c ? 160 : 60;
            g_texData[2][y][x][3] = 255;
        }
    /* 3. 关闭纹理时使用的中性色 */
    for (y = 0; y < TEX_SIZE; ++y)
        for (x = 0; x < TEX_SIZE; ++x) {
            g_texData[3][y][x][0] = 200; g_texData[3][y][x][1] = 200;
            g_texData[3][y][x][2] = 200; g_texData[3][y][x][3] = 255;
        }

    glGenTextures(NUM_TEX, g_texName);
    for (i = 0; i < NUM_TEX; ++i) {
        glBindTexture(GL_TEXTURE_2D, g_texName[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEX_SIZE, TEX_SIZE, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, g_texData[i]);
    }
}

static void applyTexture(void)
{
    if (g_texOn) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, g_texName[g_texIdx]);
        glColor3f(1.0f, 1.0f, 1.0f);           /* 让纹理原色显示 */
    } else {
        glDisable(GL_TEXTURE_2D);
    }
}

/* ------------------------- 光照与材质 ------------------------- */
static void setupLighting(void)
{
    GLfloat pos[4]   = { 2.0f, 6.0f, 4.0f, 1.0f };
    GLfloat amb[4]   = { 0.25f, 0.25f, 0.28f, 1.0f };
    GLfloat dif[4]   = { 0.9f, 0.9f, 0.9f, 1.0f };
    GLfloat sp[4]    = { 0.7f, 0.7f, 0.7f, 1.0f };
    GLfloat specM[4] = { 0.6f, 0.6f, 0.6f, 1.0f };
    GLfloat shin[1]  = { 60.0f };

    if (g_shading == 2) {                      /* 无光照模式 */
        glDisable(GL_LIGHTING);
        glDisable(GL_COLOR_MATERIAL);
        return;
    }
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
    glLightfv(GL_LIGHT0, GL_SPECULAR, sp);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specM);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shin);
    glShadeModel(g_shading == 0 ? GL_FLAT : GL_SMOOTH);
}

/* ------------------------- 场景图元 ------------------------- */
static void drawGround(void)
{
    glPushMatrix();
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    applyTexture();
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2f(-6.0f, -6.0f);
        glTexCoord2f(8, 0); glVertex2f( 6.0f, -6.0f);
        glTexCoord2f(8, 8); glVertex2f( 6.0f,  6.0f);
        glTexCoord2f(0, 8); glVertex2f(-6.0f,  6.0f);
    glEnd();
    /* 无纹理时叠一层淡绿 */
    if (!g_texOn) {
        glColor3f(0.45f, 0.55f, 0.45f);
        glBegin(GL_QUADS);
            glVertex2f(-6.0f, -6.0f); glVertex2f(6.0f, -6.0f);
            glVertex2f(6.0f, 6.0f);   glVertex2f(-6.0f, 6.0f);
        glEnd();
    }
    glPopMatrix();
}

static void drawObjects(void)
{
    /* 中央茶壶（无纹理坐标，固定金属色） */
    glPushMatrix();
    glTranslatef(0.0f, 1.35f, 0.0f);
    if (!g_texOn || 1) { glDisable(GL_TEXTURE_2D); }
    if (g_shading == 2) glColor3f(0.85f, 0.65f, 0.25f);
    glutSolidTeapot(1.0);
    glPopMatrix();

    /* 左侧立方体：纹理贴图 */
    glPushMatrix();
    glTranslatef(-2.6f, 0.75f, 0.6f);
    glRotatef(20.0f, 0.0f, 1.0f, 0.0f);
    applyTexture();
    if (!g_texOn) glColor3f(0.80f, 0.35f, 0.30f);
    glutSolidCube(1.5);
    glPopMatrix();

    /* 右侧球体：纹理贴图 */
    glPushMatrix();
    glTranslatef(2.6f, 0.9f, 0.3f);
    applyTexture();
    if (!g_texOn) glColor3f(0.30f, 0.50f, 0.85f);
    glutSolidSphere(0.9, 36, 24);
    glPopMatrix();

    /* 后方圆环：纯色 */
    glPushMatrix();
    glTranslatef(0.0f, 1.0f, -2.8f);
    glRotatef(60.0f, 1.0f, 0.0f, 0.0f);
    glDisable(GL_TEXTURE_2D);
    if (g_shading == 2) glColor3f(0.4f, 0.8f, 0.4f);
    else glColor3f(0.4f, 0.8f, 0.4f);
    glutSolidTorus(0.22, 0.75, 24, 36);
    glPopMatrix();
    glEnable(GL_TEXTURE_2D);
}

/* ------------------------- 显示回调 ------------------------- */
static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    setupLighting();

    gluLookAt(0.0, 2.5 + g_dist * 0.35, g_dist, 0.0, 0.9, 0.0, 0.0, 1.0, 0.0);

    glTranslatef(g_panX, g_panY, 0.0f);
    glScalef(g_scale, g_scale, g_scale);
    glRotatef(g_rotX, 1.0f, 0.0f, 0.0f);
    glRotatef(g_rotY, 0.0f, 1.0f, 0.0f);

    drawGround();
    drawObjects();

    glutSwapBuffers();
}

static void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50.0, (double)w / (double)(h ? h : 1), 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

/* ------------------------- 交互回调 ------------------------- */
static void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        g_lastX = x; g_lastY = y;
    } else if (state == GLUT_DOWN && (button == 3 || button == 4)) { /* 滚轮: 3=上 4=下 */
        g_dist += (button == 3) ? -0.6f : 0.6f;
        if (g_dist < 3.0f) g_dist = 3.0f;
        if (g_dist > 30.0f) g_dist = 30.0f;
        glutPostRedisplay();
    }
}

static void motion(int x, int y)
{
    int dx = x - g_lastX, dy = y - g_lastY;
    g_lastX = x; g_lastY = y;
    if (glutGetModifiers() & GLUT_ACTIVE_SHIFT) {        /* 平移 */
        g_panX += dx * 0.01f;
        g_panY -= dy * 0.01f;
    } else if (glutGetModifiers() & GLUT_ACTIVE_CTRL) {  /* 缩放 */
        g_scale *= powf(1.01f, -dy);
        if (g_scale < 0.2f) g_scale = 0.2f;
        if (g_scale > 5.0f) g_scale = 5.0f;
    } else {                                             /* 旋转 */
        g_rotY += dx * 0.4f;
        g_rotX += dy * 0.4f;
        if (g_rotX > 89.0f) g_rotX = 89.0f;
        if (g_rotX < -89.0f) g_rotX = -89.0f;
    }
    glutPostRedisplay();
}

static void menuShading(int v)  { g_shading = v;  printf("[menu] shading = %s\n", g_shadingName[v]); glutPostRedisplay(); }
static void menuTexture(int v)  { g_texIdx = v % NUM_TEX; g_texOn = (v != 3) ? 1 : 0;
                                  printf("[menu] texture = %s\n", g_texNameStr[v]); glutPostRedisplay(); }
static void menuReset(int v)    { (void)v;
    g_rotX = -12.0f; g_rotY = 30.0f; g_panX = g_panY = 0.0f; g_scale = 1.0f; g_dist = 9.0f;
    glutPostRedisplay(); }

static int buildMenu(void)
{
    int mShade = glutCreateMenu(menuShading);
    glutAddMenuEntry("Flat (rough)", 0);
    glutAddMenuEntry("Gouraud (smooth)", 1);
    glutAddMenuEntry("No lighting (constant color)", 2);

    int mTex = glutCreateMenu(menuTexture);
    glutAddMenuEntry("Checkerboard", 0);
    glutAddMenuEntry("Gradient", 1);
    glutAddMenuEntry("Stripe", 2);
    glutAddMenuEntry("Texture OFF", 3);

    int m = glutCreateMenu(menuReset);
    glutAddSubMenu("Shading model", mShade);
    glutAddSubMenu("Texture", mTex);
    glutAddMenuEntry("Reset view (R)", 0);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    return m;
}

static void keyboard(unsigned char key, int x, int y)
{
    (void)x; (void)y;
    switch (key) {
    case 0x1B: exit(0); break;                       /* ESC */
    case 'r': case 'R':
        g_rotX = -12.0f; g_rotY = 30.0f; g_panX = g_panY = 0.0f;
        g_scale = 1.0f; g_dist = 9.0f; break;
    case 's': case 'S':
        g_shading = (g_shading + 1) % 3;
        printf("[key] shading = %s\n", g_shadingName[g_shading]); break;
    case 't': case 'T':
        g_texIdx = (g_texIdx + 1) % NUM_TEX;
        g_texOn = (g_texIdx != 3);
        printf("[key] texture = %s\n", g_texNameStr[g_texIdx]); break;
    case ' ':
        g_autoSpin = !g_autoSpin; break;
    default: return;
    }
    glutPostRedisplay();
}

static void timer(int v)
{
    if (g_autoSpin) {
        g_rotY += 0.3f;
        if (g_rotY > 360.0f) g_rotY -= 360.0f;
        glutPostRedisplay();
    }
    glutTimerFunc(33, timer, v);                     /* ~30 FPS */
}

/* ------------------------- 初始化与主函数 ------------------------- */
static void initGL(void)
{
    glClearColor(0.10f, 0.12f, 0.16f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    buildTextures();
    setupLighting();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1000, 700);
    glutInitWindowPosition(120, 80);
    glutCreateWindow("CG Lab1 - OpenGL Basic (20252757 Deng Xiangrui)");

    initGL();
    buildMenu();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(33, timer, 0);

    printf("Controls: LMB drag=rotate  Shift+drag=pan  Ctrl+drag=scale  wheel=zoom\n");
    printf("          RMB menu: shading / texture;  keys: R reset, S shading, T texture, Space spin, Esc quit\n");

    glutMainLoop();
    return 0;
}
