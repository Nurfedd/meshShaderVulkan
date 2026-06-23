

project(NinoEngine)

cmake_minimum_required(VERSION 3.20)

set(GOOFY_ENGINE_ROOT_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}"
    CACHE INTERNAL "Engine root directory"
)
message("ENGINE ROOT DIR = ${GOOFY_ENGINE_ROOT_DIR}")


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(output_dir "${CMAKE_SOURCE_DIR}/build/$<CONFIG>")


file(GLOB_RECURSE SOURCES "CodeNino/Source/*.cpp")
file(GLOB_RECURSE HEADERS "CodeNino/Include/*.hpp")

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})


target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC /std:c++20 /Zc:__cplusplus /EHsc)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/CodeNino/Include"
    "${CMAKE_CURRENT_SOURCE_DIR}/CodeNino/Include/Network"
)


target_link_libraries(${PROJECT_NAME} PUBLIC glfw)
target_link_libraries(${PROJECT_NAME} PUBLIC glm)
target_link_libraries(${PROJECT_NAME} PUBLIC RHI)
target_link_libraries(${PROJECT_NAME} PUBLIC nino_thread)
#target_link_libraries(${PROJECT_NAME} PUBLIC Physics)
target_link_libraries(${PROJECT_NAME} PRIVATE assimp)
target_link_libraries(${PROJECT_NAME} PRIVATE meshoptimizer)


# Copy "resources/" folder.
function(copy_engine_resources target)

    set(SRC_DIR "${GOOFY_ENGINE_ROOT_DIR}/Resources")

    message("Copy from (captured): ${SRC_DIR}")

    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${SRC_DIR}"
            "$<TARGET_FILE_DIR:${target}>/Resources"
        COMMENT "Copying engine resources"
    )
endfunction()



