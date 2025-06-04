# 最小CMake版本需求
cmake_minimum_required(VERSION 3.15)

## 解决方案名
project(JoyAlgorithmWork)

# Turn on the ability to create folders to organize projects (.vcproj)
# It creates "CMakePredefinedTargets" folder by default and adds CMake
# defined projects like INSTALL.vcproj and ZERO_CHECK.vcproj
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# CPP版本
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
## 添加UNICODE/_UNICODE宏 使用unicode字符集
add_definitions(-DUNICODE -D_UNICODE)

# 输出一些信息
message(STATUS "OutPut Someting Begin ===========================")
message(STATUS "CMAKE_CXX_COMPILER_ID = ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "CMAKE_SYSTEM_NAME = ${CMAKE_SYSTEM_NAME}")
message(STATUS "OutPut Someting End =============================")

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    ## MSVC忽略4819警告
    add_definitions(/wd"4819")
endif()

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/execution-charset:utf-8>")

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(
        -Wdeprecated
        -Wextra-semi
        -Wshadow
        -Wconversion
        -Wsign-conversion
    )
endif()

# add_compile_options("-fstandalone-debug")

# 添加数据结构和算法学习子项目路径
add_subdirectory(${PROJECT_SOURCE_DIR}/AlgorithmWork/0.HelloAlgorithm)
add_subdirectory(${PROJECT_SOURCE_DIR}/AlgorithmWork/1.List)
add_subdirectory(${PROJECT_SOURCE_DIR}/AlgorithmWork/2.Tree)
add_subdirectory(${PROJECT_SOURCE_DIR}/AlgorithmWork/3.Hashing)
add_subdirectory(${PROJECT_SOURCE_DIR}/AlgorithmWork/4.PriorityQueue)
add_subdirectory(${PROJECT_SOURCE_DIR}/AlgorithmWork/5.Sorting)
add_subdirectory(${PROJECT_SOURCE_DIR}/AlgorithmWork/6.MemoryPool)

# 添加LeetCode练习子项目路径
add_subdirectory(${PROJECT_SOURCE_DIR}/LeetCode/1.TwoSum)
add_subdirectory(${PROJECT_SOURCE_DIR}/LeetCode/2.AddTwoNumbers)
add_subdirectory(${PROJECT_SOURCE_DIR}/LeetCode/3.LongestSubstringWithoutRepeatingCharacters)
add_subdirectory(${PROJECT_SOURCE_DIR}/LeetCode/4.MedianOfTwoSortedArrays)

# 添加语言基础知识项目路径
add_subdirectory(${PROJECT_SOURCE_DIR}/LanguageWork/0.TheBasic)
add_subdirectory(${PROJECT_SOURCE_DIR}/LanguageWork/1.IOLibrary)
add_subdirectory(${PROJECT_SOURCE_DIR}/LanguageWork/2.TextQuery)
add_subdirectory(${PROJECT_SOURCE_DIR}/LanguageWork/3.NetworkClient)
add_subdirectory(${PROJECT_SOURCE_DIR}/LanguageWork/4.NetworkServer)
add_subdirectory(${PROJECT_SOURCE_DIR}/LanguageWork/5.ThreadWork)
add_subdirectory(${PROJECT_SOURCE_DIR}/LanguageWork/6.TemplateWork)