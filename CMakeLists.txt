cmake_minimum_required(VERSION 3.14)
project(elven)

set(CMAKE_EXPORT_COMPILE_COMANDS on)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Release)
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "-O3")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "-O5")
endif()

add_executable(
    ${PROJECT_NAME}
    src/Ayanami.cpp
    src/Image.cpp
    src/Utils.cpp
    src/Frame.cpp
    src/Renderer.cpp
    src/Object.cpp
    src/World.cpp
    src/Scene.cpp
    src/Spec.cpp
    src/Serializer/Serializer.cpp
    src/Serializer/Parser.cpp
    src/Serializer/FileIO.cpp
)

find_package(OpenCL REQUIRED)
target_link_libraries(${PROJECT_NAME} OpenCL::OpenCL)

target_include_directories(
                        ${PROJECT_NAME}
                        PRIVATE
                        lib/
                        src/include
                        src/Serializer
                        lib/glm
                    )