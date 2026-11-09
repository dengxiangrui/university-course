# 实验一 OpenGL 基本使用 —— 项目说明

学生：邓翔睿（20252757）

## 目录结构

```
实验一/
├── src/main.c            实验源代码（C + freeglut）
├── build/                可执行文件及运行所需 DLL
│   ├── GraphicsExp1.exe
│   └── *.dll             （从 MSYS2 复制，便于双击直接运行）
└── .vscode/
    ├── tasks.json        VSCode 一键构建/运行任务
    └── c_cpp_properties.json  智能提示配置
```

## 在 VSCode 中编译运行

1. 用 VSCode 打开本目录（实验一/）。
2. 按 `Ctrl+Shift+B` 执行默认构建任务 **build GraphicsExp1**；
   或 `Ctrl+Shift+P` → *Run Task* → **run GraphicsExp1**（先自动编译，再启动窗口）。
3. 命令行手动编译（等价方式，需在 MSYS2 环境或把 `D:\msys64\ucrt64\bin` 加入 PATH）：

```bash
gcc src/main.c -o build/GraphicsExp1.exe -lfreeglut -lglu32 -lopengl32
```

> 注意：gcc 真正执行需要 `D:\msys64\ucrt64\bin` 在 PATH 中（tasks.json 已自动处理）。
> 直接双击 `build/GraphicsExp1.exe` 也可运行，所需 DLL 已复制到同目录。

## 功能与操作说明（对应任务书要求）

| 任务书要求 | 实现方式 |
| --- | --- |
| 基础程序框架 + 场景渲染 | freeglut 双缓冲 + 深度测试；场景含地面、茶壶、立方体、球体、圆环，带光源与材质 |
| 着色模型交互选择 | 鼠标**右键菜单** Shading model：Flat（粗糙着色）/ Gouraud（平滑着色）/ 无光照固定色；快捷键 `S` 循环切换 |
| 鼠标几何变换 | 左键拖动=旋转，`Shift+左键`拖动=平移，`Ctrl+左键`拖动=缩放，滚轮=调整视距 |
| 纹理贴图选择 | 右键菜单 Texture：棋盘格 / 红蓝渐变 / 斜条纹 / 关闭（程序化生成纹理，作用于地面、立方体、球体）；快捷键 `T` 循环切换 |
| 其他 | `R` 复位视角，`空格` 启停自动旋转，`ESC` 退出；窗口标题含学号姓名 |

## 场景复杂度的加分点

- 多物体组合场景（茶壶/立方体/球体/圆环/地面）
- GL_LIGHT0 定向点光源 + 镜面高光材质 + 法线归一化
- 双缓冲渲染 + 30 FPS 定时器自动旋转动画
- 程序化生成三种 RGBA 纹理（GL_LINEAR 滤波、GL_REPEAT 环绕）
