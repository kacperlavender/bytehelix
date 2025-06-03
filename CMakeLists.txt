cmake_minimum_required(VERSION 3.31)
project(helix_3d)

set(CMAKE_CXX_STANDARD 20)

# INCLUDE-y
include_directories(
        libs/glad/include
        thirdparty/imgui
        thirdparty/imgui/backends
        /opt/homebrew/include
)

# LINKER
link_directories(/opt/homebrew/lib)

# ŹRÓDŁA
set(SOURCES
        main.cpp
        libs/glad/src/glad.c

        thirdparty/imgui/imgui.cpp
        thirdparty/imgui/imgui_draw.cpp
        thirdparty/imgui/imgui_widgets.cpp
        thirdparty/imgui/imgui_tables.cpp
        thirdparty/imgui/backends/imgui_impl_glfw.cpp
        thirdparty/imgui/backends/imgui_impl_opengl3.cpp
)

# BINARKA
add_executable(helix_3d ${SOURCES})

# LINKOWANIE
target_link_libraries(helix_3d glfw)
target_link_libraries(helix_3d "-framework OpenGL")
