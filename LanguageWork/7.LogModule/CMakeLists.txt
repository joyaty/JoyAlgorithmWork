## 最小CMake版本要求
cmake_minimum_required(VERSION 3.15)

# 包含cmake文件
include("${CMAKE_SOURCE_DIR}/CMake/CMakeMacro.cmake")
# 设置头文件引用路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Source/)
# 设置子项目名
set(SUB_PROJECT_NAME 7.LogModule)

# 将所有源码设置到ALL_FILES变量上
set(ALL_FILES 
Source/LogModule/LogUtil.cpp
Source/LogModule/LogUtil.h
Source/LogModule/AsyncLogger.cpp
Source/LogModule/AsyncLogger.h
Source/LogModule/LogSink/ConsoleSink.cpp
Source/LogModule/LogSink/ConsoleSink.h
Source/LogModule/LogSink/ILogSink.h
Source/LogModule/LogSink/LogEntry.h
Source/Main.cpp
)

## 调用外部cmake定义的宏，将源文件按文件加分组
source_group_by_dir("${ALL_FILES}")
## 添加编译可执行文件
add_executable(${SUB_PROJECT_NAME} ${ALL_FILES})
# 项目添加在JoyLeetCode文件夹下
set_target_properties(${SUB_PROJECT_NAME} PROPERTIES FOLDER LanguageWork)