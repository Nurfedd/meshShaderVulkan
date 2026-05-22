# Project name
project(RHI)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Look for header and source files.
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "*.hpp" "*.h")

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
target_compile_options(${PROJECT_NAME} PUBLIC /std:c++20 /Zc:__cplusplus)
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/CodeRhi/Include")


# Ensure the computer has Vulkan SDK installed.
find_package(Vulkan)
if(NOT Vulkan_FOUND)
    message(FATAL_ERROR
        "Vulkan SDK not found. Install it from https://vulkan.lunarg.com/")
endif()

target_include_directories(${PROJECT_NAME}
    PUBLIC
    "${Vulkan_INCLUDE_DIRS}"
)

# Link libraries.
target_link_libraries(${PROJECT_NAME}
    PUBLIC
    volk
    glfw
)


# Macro definitions.
target_compile_definitions(${PROJECT_NAME}
    PRIVATE
    VK_NO_PROTOTYPES
    GLFW_INCLUDE_NONE
    VULKAN_STATIC_DEFINE
)
