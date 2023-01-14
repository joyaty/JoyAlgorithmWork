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
## 忽略4819警告
## add_definitions(/wd"4819")

# 添加数据结构和算法学习子项目路径
add_subdirectory(${PROJECT_SOURCE_DIR}/AlgorithmWork/0.HelloAlgorithm)
add_subdirectory(${PROJECT_SOURCE_DIR}/AlgorithmWork/1.ADT)

# 添加LeetCode练习子项目路径
add_subdirectory(${PROJECT_SOURCE_DIR}/LeetCode/1.TwoSum)
add_subdirectory(${PROJECT_SOURCE_DIR}/LeetCode/2.AddTwoNumbers)
add_subdirectory(${PROJECT_SOURCE_DIR}/LeetCode/3.LongestSubstringWithoutRepeatingCharacters)
add_subdirectory(${PROJECT_SOURCE_DIR}/LeetCode/4.MedianOfTwoSortedArrays)
