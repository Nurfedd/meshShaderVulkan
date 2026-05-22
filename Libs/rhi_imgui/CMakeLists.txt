# Project name
project(rhi_imgui)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Look for header and source files.
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "*.hpp" "*.h")

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
target_compile_options(${PROJECT_NAME} PUBLIC /std:c++20 /Zc:__cplusplus)
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/CodeRhiImGui/Include")
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/backend_imgui/Include")



# Link libraries.
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        RHI
        volk
        ImGui
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
    VK_NO_PROTOTYPES
    IMGUI_IMPL_VULKAN_USE_VOLK
)

