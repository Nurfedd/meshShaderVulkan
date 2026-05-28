cmake_minimum_required(VERSION 3.20)
project(ImGui)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(output_dir "${CMAKE_SOURCE_DIR}/build/$<CONFIG>")

message("[${PROJECT_NAME}]: Output directory --> ${output_dir}")

set(SOURCES
    Source/imgui.cpp
    Source/imgui_draw.cpp
    Source/imgui_tables.cpp
    Source/imgui_widgets.cpp
    Source/imgui_impl_glfw.cpp
    Source/ImGuizmo.cpp
)

file(GLOB_RECURSE HEADERS "Include/*.hpp" "Include/*.h")

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
target_compile_options(${PROJECT_NAME} PUBLIC /std:c++20 /Zc:__cplusplus)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        glfw
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
    GLFW_INCLUDE_NONE
)
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Include")
