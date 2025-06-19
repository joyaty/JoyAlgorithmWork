## 最小CMake版本要求
cmake_minimum_required(VERSION 3.15)

# 包含cmake文件
include("${CMAKE_SOURCE_DIR}/CMake/CMakeMacro.cmake")
# 设置头文件引用路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Source/)
# 设置子项目名
set(SUB_PROJECT_NAME 3.NetworkClient)

# 将所有源码设置到ALL_FILES变量上
set(ALL_FILES 
Source/Serialization/SerializationUtils.cpp
Source/Serialization/SerializationUtils.h
Source/Serialization/ByteSwap.h
Source/Serialization/Reflection.h
Source/Gameplay/GameObject.h
Source/Gameplay/GameObject.h
Source/SocketAddress.h
Source/TCPSocket.h
Source/Main.cpp
Source/NetworkService.cpp
Source/NetworkService.h
Source/CircleBuffer.cpp
Source/CircleBuffer.h
Source/PacketParser.cpp
Source/PacketParser.h
Source/nlohmann/json_fwd.hpp
Source/nlohmann/json.hpp
Source/Message/MessageDefine.cpp
Source/Message/MessageDefine.h
Source/Gameplay/BallVO.cpp
Source/Gameplay/BallVO.h
)
# 不同系统平台API差异，各自实现
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(ALL_FILES ${ALL_FILES}
        # Source/Platform/Windows/TestSocket.cpp
        Source/Platform/Windows/SocketAddress.cpp
        Source/Platform/Windows/TCPSocket_Windows.cpp
    )
elseif(CMAKE_SYSTEM_NAME MATCHES "Darwin")
    set(ALL_FILES ${ALL_FILES}
        Source/Platform/MacOS/TestSocket.cpp
    )
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(ALL_FILES ${ALL_FILES}
        Source/Platform/POSIX/SocketAddress.cpp
        Source/Platform/POSIX/TCPSocket_POSIX.cpp
    )
endif()

## 调用外部cmake定义的宏，将源文件按文件加分组
source_group_by_dir("${ALL_FILES}")
## 添加编译可执行文件
add_executable(${SUB_PROJECT_NAME} ${ALL_FILES})

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    ## 添加Windows的Windows Socket2支持库，当前系统版本为2.2
    target_link_libraries(${SUB_PROJECT_NAME} ws2_32)
endif()

# 项目添加在LanguageWork文件夹下
set_target_properties(${SUB_PROJECT_NAME} PROPERTIES FOLDER LanguageWork)
