/*
 * 重庆大学 计算机图形学 实验一：OpenGL 基本使用
 * 功能覆盖（对应任务书评分点）：
 *   1. OpenGL 基础程序框架 + 场景 + 交互界面            (75分 基础渲染)
 *   2. 不同着色模型的界面交互选择（平面/平滑/光照开关）  (+5)
 *   3. 通过鼠标进行几何变换（旋转 / 平移 / 缩放）        (+5)
 *   4. 通过界面进行纹理贴图的选择（多种纹理 + 关闭）     (+5)
 *   5. 复杂场景（多物体 + 地面网格 + 多光源）            (+10 提高分)
 *
 * 运行环境：MSYS2 / MinGW64 + freeglut，固定渲染管线
 * 编译命令（在你已配好的 MINGW64 终端里执行）：
 *   D:/msys64/mingw64/bin/g++.exe main.cpp -o main_test.exe \
 *       -ID:/msys64/mingw64/include -LD:/msys64/mingw64/lib \
 *       -lfreeglut -lopengl32 -lgdi32
 * 运行：./main_test.exe
 */

#include <GL/freeglut.h>
#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <cstdio>

/* ---------- 全局交互状态 ---------- */
static float g_rotX = 20.0f, g_rotY = 30.0f;  // 鼠标旋转角
static float g_transX = 0.0f, g_transY = 0.0f, g_transZ = -6.0f; // 平移（含默认后退）
static float g_scale = 1.0f;                  // 缩放

static int   g_shadeMode = 1;   // 0=平面(Flat), 1=平滑(Smooth)
static int   g_lightOn   = 1;   // 光照开关
static int   g_texId     = 0;   // 当前纹理：0=关闭, 1..N=已生成纹理
static const int TEX_COUNT = 3; // 程序化生成的纹理数量

static int   g_lastX = 0, g_lastY = 0; // 鼠标上一次位置
static int   g_button = -1;            // 当前按下的鼠标键
static float g_angleZ = 0.0f;          // 动画自转角

static GLuint g_textures[TEX_COUNT + 1] = {0}; // 1..TEX_COUNT 使用，0 表示关闭

/* ---------- 程序化生成纹理（无需外部图片文件） ---------- */
static void buildChecker(GLubyte* img, int n) {
    for (int y = 0; y < n; ++y)
        for (int x = 0; x < n; ++x) {
            int c = ((x / 8 + y / 8) % 2) ? 255 : 40;
            img[(y * n + x) * 3 + 0] = c;
            img[(y * n + x) * 3 + 1] = c;
            img[(y * n + x) * 3 + 2] = c;
        }
}
static void buildGradient(GLubyte* img, int n) { // 彩虹渐变
    for (int y = 0; y < n; ++y)
        for (int x = 0; x < n; ++x) {
            img[(y * n + x) * 3 + 0] = (GLubyte)(255.0 * x / n);
            img[(y * n + x) * 3 + 1] = (GLubyte)(255.0 * y / n);
            img[(y * n + x) * 3 + 2] = 160;
        }
}
static void buildStripes(GLubyte* img, int n) { // 条纹
    for (int y = 0; y < n; ++y)
        for (int x = 0; x < n; ++x) {
            bool red = ((x + y) / 8) % 2;
            img[(y * n + x) * 3 + 0] = red ? 220 : 30;
            img[(y * n + x) * 3 + 1] = red ? 40  : 120;
            img[(y * n + x) * 3 + 2] = red ? 40  : 220;
        }
}
static void initTextures() {
    const int n = 64;
    GLubyte* img = new GLubyte[n * n * 3];

    glGenTextures(1, &g_textures[1]);
    buildChecker(img, n);
    glBindTexture(GL_TEXTURE_2D, g_textures[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, n, n, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenTextures(1, &g_textures[2]);
    buildGradient(img, n);
    glBindTexture(GL_TEXTURE_2D, g_textures[2]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, n, n, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenTextures(1, &g_textures[3]);
    buildStripes(img, n);
    glBindTexture(GL_TEXTURE_2D, g_textures[3]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, n, n, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    delete[] img;
}

/* ---------- 光照初始化 ---------- */
static void initLights() {
    GLfloat amb[]  = {0.25f, 0.25f, 0.25f, 1.0f};
    GLfloat dif[]  = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat spec[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat pos[]  = {4.0f, 6.0f, 6.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
    glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    glEnable(GL_LIGHT0);

    GLfloat amb2[]  = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat dif2[]  = {0.3f, 0.6f, 0.9f, 1.0f};
    GLfloat pos2[]  = {-5.0f, -2.0f, 4.0f, 1.0f};
    glLightfv(GL_LIGHT1, GL_AMBIENT, amb2);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, dif2);
    glLightfv(GL_LIGHT1, GL_POSITION, pos2);
    glEnable(GL_LIGHT1);
}

/* ---------- 画一个带纹理坐标的立方体 ---------- */
static void drawTexturedCube(float s) {
    if (g_texId > 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, g_textures[g_texId]);
    } else {
        glDisable(GL_TEXTURE_2D);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glColor3f(0.9f, 0.55f, 0.2f);
    }
    const float h = s;
    struct V { float x, y, z; } c[8] = {
        {-h,-h,-h},{ h,-h,-h},{ h, h,-h},{-h, h,-h},
        {-h,-h, h},{ h,-h, h},{ h, h, h},{-h, h, h}
    };
    struct F { int a,b,c,d; } f[6] = {
        {0,1,2,3},{5,4,7,6},{4,0,3,7},{1,5,6,2},{4,3,2,6},{0,4,5,1}
    };
    glBegin(GL_QUADS);
    for (int i = 0; i < 6; ++i) {
        // 让纹理随光照与顶点色混合，平面着色时法线更明显
        glNormal3f(0,0,1);
        int idx[4] = {f[i].a, f[i].b, f[i].c, f[i].d};
        float uv[4][2] = {{0,0},{1,0},{1,1},{0,1}};
        for (int k = 0; k < 4; ++k) {
            glTexCoord2f(uv[k][0], uv[k][1]);
            glVertex3f(c[idx[k]].x, c[idx[k]].y, c[idx[k]].z);
        }
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

/* ---------- 地面网格 ---------- */
static void drawGround() {
    glDisable(GL_LIGHTING);
    glColor3f(0.35f, 0.35f, 0.35f);
    glBegin(GL_LINES);
    for (int i = -5; i <= 5; ++i) {
        glVertex3f(-5.0f, -1.5f, (float)i);
        glVertex3f( 5.0f, -1.5f, (float)i);
        glVertex3f((float)i, -1.5f, -5.0f);
        glVertex3f((float)i, -1.5f,  5.0f);
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

/* ---------- 场景绘制 ---------- */
static void drawScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    // 应用鼠标几何变换：平移 -> 缩放 -> 旋转
    glTranslatef(g_transX, g_transY, g_transZ);
    glScalef(g_scale, g_scale, g_scale);
    glRotatef(g_rotX, 1.0f, 0.0f, 0.0f);
    glRotatef(g_rotY + g_angleZ, 0.0f, 1.0f, 0.0f);

    glShadeModel(g_shadeMode ? GL_SMOOTH : GL_FLAT);

    drawGround();

    // 主体：贴图立方体
    GLfloat matD[] = {0.9f, 0.6f, 0.2f, 1.0f};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matD);
    glPushMatrix();
    glTranslatef(-1.6f, 0.0f, 0.0f);
    drawTexturedCube(0.9f);
    glPopMatrix();

    // 球体（平滑/平面着色对比明显）
    GLfloat matS[] = {0.3f, 0.7f, 0.9f, 1.0f};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matS);
    glPushMatrix();
    glTranslatef(1.6f, 0.4f, 0.0f);
    glutSolidSphere(0.85f, 28, 28);
    glPopMatrix();

    // 茶壶（freeglut 内置，展示高光与多光源）
    GLfloat matH[] = {0.8f, 0.8f, 0.85f, 1.0f};
    GLfloat matHi[] = {0.6f, 0.6f, 0.6f, 1.0f};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matH);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matHi);
    glMaterialf(GL_FRONT, GL_SHININESS, 60.0f);
    glPushMatrix();
    glTranslatef(0.0f, 1.4f, 0.0f);
    glScalef(0.55f, 0.55f, 0.55f);
    glutSolidTeapot(1.0f);
    glPopMatrix();

    glutSwapBuffers();
}

/* ---------- 回调 ---------- */
static void display() { drawScene(); }

static void idle() {
    g_angleZ += 0.2f;          // 持续自转动画
    if (g_angleZ > 360.0f) g_angleZ -= 360.0f;
    glutPostRedisplay();
}

static void reshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

static void mouse(int btn, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        g_button = btn;
        g_lastX = x; g_lastY = y;
    } else {
        g_button = -1;
    }
}

static void motion(int x, int y) {
    int dx = x - g_lastX, dy = y - g_lastY;
    g_lastX = x; g_lastY = y;
    if (g_button == GLUT_LEFT_BUTTON) {          // 左键：旋转
        g_rotY += dx * 0.5f;
        g_rotX += dy * 0.5f;
    } else if (g_button == GLUT_RIGHT_BUTTON) {  // 右键：平移
        g_transX += dx * 0.01f;
        g_transY -= dy * 0.01f;
    } else if (g_button == GLUT_MIDDLE_BUTTON) { // 中键：缩放
        g_scale *= (1.0f + (dx - dy) * 0.002f);
        if (g_scale < 0.2f) g_scale = 0.2f;
        if (g_scale > 8.0f) g_scale = 8.0f;
    }
    glutPostRedisplay();
}

static void wheel(int, int dir, int, int) {
    g_scale *= (dir > 0) ? 1.1f : 0.9f;           // 滚轮缩放
    if (g_scale < 0.2f) g_scale = 0.2f;
    if (g_scale > 8.0f) g_scale = 8.0f;
    glutPostRedisplay();
}

static void resetView() {
    g_rotX = 20.0f; g_rotY = 30.0f;
    g_transX = 0.0f; g_transY = 0.0f; g_transZ = -6.0f;
    g_scale = 1.0f;
    glutPostRedisplay();
}

static void keyboard(unsigned char key, int, int) {
    switch (key) {
        case 'f': case 'F': g_shadeMode = 0; break; // 平面着色
        case 's': case 'S': g_shadeMode = 1; break; // 平滑着色
        case 'l': case 'L': g_lightOn = !g_lightOn;
            if (g_lightOn) glEnable(GL_LIGHTING); else glDisable(GL_LIGHTING); break;
        case '1': g_texId = 1; break;
        case '2': g_texId = 2; break;
        case '3': g_texId = 3; break;
        case '0': g_texId = 0; break;
        case 'r': case 'R': resetView(); break;
        case 'q': case 'Q': case 27:              // q / ESC 退出
            exit(0); break;
    }
    glutPostRedisplay();
}

static void printHelp() {
    printf("==================================================\n");
    printf(" 计算机图形学 实验一 操作说明\n");
    printf(" 鼠标左键拖动 : 旋转物体        右键拖动 : 平移\n");
    printf(" 鼠标中键拖动 : 缩放           滚轮     : 缩放\n");
    printf(" 键盘 F       : 平面着色       S        : 平滑着色\n");
    printf(" 键盘 L       : 光照开关\n");
    printf(" 键盘 1/2/3   : 切换纹理(棋盘/渐变/条纹)  0 : 关闭纹理\n");
    printf(" 键盘 R       : 复位视角       Q/ESC    : 退出\n");
    printf("==================================================\n");
}

/* ---------- 主函数 ---------- */
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(900, 700);
    glutInitWindowPosition(100, 80);
    glutCreateWindow("Computer Graphics Exp1 - OpenGL Basic");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    initLights();
    initTextures();

    printHelp();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutMouseWheelFunc(wheel);  // FreeGLUT 标准滚轮接口
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}
