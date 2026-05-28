add_subdirectory(GLFW)
add_subdirectory(volk)
add_subdirectory(ImGui)
add_subdirectory(meshoptimizer)


set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "" FORCE)
add_subdirectory(assimp)
target_compile_options(assimp PRIVATE /EHsc /WX-)


add_subdirectory(glm)
