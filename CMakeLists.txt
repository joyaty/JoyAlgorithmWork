# 最小CMake版本需求
cmake_minimum_required(VERSION 3.5)

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

# 添加数据结构和算法学习子项目路径
add_subdirectory(${PROJECT_SOURCE_DIR}/AlgorithmWork/0.HelloAlgorithm)
add_subdirectory(${PROJECT_SOURCE_DIR}/AlgorithmWork/1.List)
add_subdirectory(${PROJECT_SOURCE_DIR}/AlgorithmWork/2.Tree)
add_subdirectory(${PROJECT_SOURCE_DIR}/AlgorithmWork/3.Hashing)
add_subdirectory(${PROJECT_SOURCE_DIR}/AlgorithmWork/4.PriorityQueue)
add_subdirectory(${PROJECT_SOURCE_DIR}/AlgorithmWork/5.Sorting)

# 添加LeetCode练习子项目路径
add_subdirectory(${PROJECT_SOURCE_DIR}/LeetCode/1.TwoSum)
add_subdirectory(${PROJECT_SOURCE_DIR}/LeetCode/2.AddTwoNumbers)
add_subdirectory(${PROJECT_SOURCE_DIR}/LeetCode/3.LongestSubstringWithoutRepeatingCharacters)
add_subdirectory(${PROJECT_SOURCE_DIR}/LeetCode/4.MedianOfTwoSortedArrays)

# 添加语言基础知识项目路径
add_subdirectory(${PROJECT_SOURCE_DIR}/LanguageWork/0.TheBasic)
add_subdirectory(${PROJECT_SOURCE_DIR}/LanguageWork/1.IOLibrary)