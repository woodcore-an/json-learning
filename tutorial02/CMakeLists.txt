# 指定 cmake 版本
cmake_minimum_required(VERSION 2.6)

# 本 CMakeLists.txt 的 project 名称
# 会自动创建两个变量，PROJECT_SOURCE_DIR 和 PROJECT_NAME
# ${PROJECT_SOURCE_DIR}：本 CMakeLists.txt 所在的文件夹路径
# ${PROJECT_NAME}：本 CMakeLists.txt 的 project 名称
project(woodjson_test C)

if (CMAKE_C_COMPILE_ID MATCHES "GNU|Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ansi -pedantic -Wall")
endif()

# 将 .cpp/.c/.cc 文件生成库文件
# 注意：库文件名称通常为 libxxx.a 或 libxxx.so，在这里只要写xxx即可
# add_library(库文件名称 文件)
add_library(woodjson woodjson.c)

# 将 .cpp/.c/.cc 文件生成可执行文件
# add_executable(可执行文件名称 文件)
add_executable(woodjson_test test.c)

# 对 add_library 或 add_executable 生成的文件进行链接操作
# 注意：库文件名称通常为 libxxx.a 或 libxxx.so，在这里只要写xxx即可
# target_link_libraries(库文件名称/可执行文件名称 链接的库文件名称)
target_link_libraries(woodjson_test woodjson)