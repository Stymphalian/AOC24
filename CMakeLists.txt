cmake_minimum_required(VERSION 3.10)
project(BASIC_CPP)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")


message("Preparing GLM build")
include_directories(${CMAKE_CURRENT_LIST_DIR}/external/glm/)

message("Preparing Noin build")
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
set(NOIN_SRCS
    src/main.cpp
)

add_executable(noin ${NOIN_SRCS})
target_link_libraries(noin)
target_compile_definitions(noin PUBLIC
    GLM_FORCE_QUAT_DATA_XYZW
    GLM_ENABLE_EXPERIMENTAL
)
add_custom_command(TARGET noin POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   "${CMAKE_CURRENT_LIST_DIR}/res"
                   "$<TARGET_FILE_DIR:noin>/res")