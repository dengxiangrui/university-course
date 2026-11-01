# 1. 设置最低版本
cmake_minimum_required(VERSION 3.15)

# 2. 设置项目名称（这里可以随意，比如叫 MyWorkspace）
project(MyCppWorkspace)

# 3. 设置 C++ 标准
set(CMAKE_CXX_STANDARD 14)

# 4. ⭐ 核心设置：把所有 .exe 输出到根目录下的 bin 文件夹
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# 5. 自动扫描：查找根目录以及所有子目录（如 test, test2 等）里的 .cpp 文件
# GLOB_RECURSE 是“递归扫描”的意思，能穿透文件夹查找
file(GLOB_RECURSE ALL_SOURCE_FILES
        "${CMAKE_SOURCE_DIR}/*.cpp"
)

# 6. 遍历每一个找到的 .cpp 文件
foreach(src_file ${ALL_SOURCE_FILES})
    # 提取文件名（不含后缀），作为可执行文件的名字
    # 例如：test/main.cpp -> 生成 main.exe
    get_filename_component(exe_name ${src_file} NAME_WE)

    # 提取文件所在的路径（用于确定依赖关系）
    get_filename_component(src_dir ${src_file} DIRECTORY)

    # 动态创建编译目标：生成名为 ${exe_name} 的程序，源文件是 ${src_file}
    add_executable(${exe_name} ${src_file})

    # 设置该目标的输出属性，确保 .exe 真的跑进 bin 文件夹
    set_target_properties(${exe_name} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    )
endforeach()