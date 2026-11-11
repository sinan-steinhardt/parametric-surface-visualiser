cmake_minimum_required(VERSION 3.20)

project(Parametric3DVisualiser
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(PkgConfig REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(X11 REQUIRED)

pkg_check_modules(RAYLIB REQUIRED IMPORTED_TARGET raylib)

find_path(MUPARSER_INCLUDE_DIR muParser.h REQUIRED)
find_library(MUPARSER_LIBRARY NAMES muparser REQUIRED)

add_executable(parametric-3d-visualiser
    src/main.cpp
)

target_include_directories(parametric-3d-visualiser PRIVATE
    ${MUPARSER_INCLUDE_DIR}
)

target_link_libraries(parametric-3d-visualiser PRIVATE
    glm::glm
    ${MUPARSER_LIBRARY}
    /usr/local/lib/libraylib.a
    X11::X11
)