## 最小CMake版本要求
cmake_minimum_required(VERSION 3.15)

# 包含cmake文件
include("${CMAKE_SOURCE_DIR}/CMake/CMakeMacro.cmake")
# 设置头文件引用路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Source/)

# 预编译头文件和源文件的路径
# set(STDAFX_PCH_H ${CMAKE_CURRENT_SOURCE_DIR}/Source/stdafx.h)
# set(STDAFX_PCH_C ${CMAKE_CURRENT_SOURCE_DIR}/Source/stdafx.cpp)

set(SUB_PROJECT_NAME 0.HelloAlgorithm)

# 将所有源码设置到ALL_FILES变量上
set(ALL_FILES 
Source/Main.cpp
)

## 调用外部cmake定义的宏，将源文件按文件加分组
source_group_by_dir("${ALL_FILES}")
## 添加编译可执行文件
add_executable(${SUB_PROJECT_NAME} ${ALL_FILES})
# 项目添加在JoyDirectXSample文件夹下
set_target_properties(${SUB_PROJECT_NAME} PROPERTIES FOLDER JoyAlgorithm)
# 调用定义在CMakeMacro.cmake中的宏，使用预编译头
# USE_MSVC_PCH(${SUB_PROJECT_NAME} stdafx.h ${STDAFX_PCH_C})
