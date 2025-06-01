## 最小CMake版本要求
cmake_minimum_required(VERSION 3.15)

# 包含cmake文件
include("${CMAKE_SOURCE_DIR}/CMake/CMakeMacro.cmake")
# 设置头文件引用路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Source/)
# 设置子项目名
set(SUB_PROJECT_NAME 5.ThreadWork)

# 将所有源码设置到ALL_FILES变量上
set(ALL_FILES 
Source/Main.cpp
)
# 不同系统平台API差异，各自实现
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(ALL_FILES ${ALL_FILES}
        Source/Platform/Windows/ThreadWork.cpp
        Source/STL/ThreadWork.cpp
    )
else(CMAKE_SYSTEM_NAME MATCHES "Darwin" OR CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(ALL_FILES ${ALL_FILES}
        Source/Platform/MacOS/ThreadWork.cpp
        Source/STL/ThreadWork.cpp
    )
endif()

## 调用外部cmake定义的宏，将源文件按文件加分组
source_group_by_dir("${ALL_FILES}")
## 添加编译可执行文件
add_executable(${SUB_PROJECT_NAME} ${ALL_FILES})

# 项目添加在LanguageWork文件夹下
set_target_properties(${SUB_PROJECT_NAME} PROPERTIES FOLDER LanguageWork)
