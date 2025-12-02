cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_COMPILER "/opt/homebrew/opt/llvm/bin/clang++")
set(CMAKE_C_COMPILER "/opt/homebrew/opt/llvm/bin/clang")

project(HTTPWebserver)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

# Configure for macOS SDK
execute_process(COMMAND xcrun --show-sdk-path OUTPUT_VARIABLE SDKROOT OUTPUT_STRIP_TRAILING_WHITESPACE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -isysroot ${SDKROOT}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -isysroot ${SDKROOT}")

add_executable(HTTPWebserver
    main.cpp
)

target_sources(HTTPWebserver
    PUBLIC 
    FILE_SET CXX_MODULES FILES
        coroutines.ixx
        threadpool.ixx
        tcpserver.ixx
)
