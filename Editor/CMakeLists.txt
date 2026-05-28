cmake_minimum_required(VERSION 3.28)
project(Editor)

#set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "CodeEditor/Source/*.cpp")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "CodeEditor/Include/*.hpp")

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/CodeEditor/Include
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        MeshShaderEngine
        ImGui
        rhi_imgui
)

copy_engine_resources(Editor)