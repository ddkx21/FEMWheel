cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME FEMWheel)

project(${PROJECT_NAME} LANGUAGES CXX)

add_executable(${PROJECT_NAME} src/main.cpp
        src/render/ShadeProgram.cpp
        src/render/ShadeProgram.h
        src/resources/ResourcesManager.cpp
        src/resources/ResourcesManager.h
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_compile_definitions(${PROJECT_NAME}  PRIVATE RES_DIR="${CMAKE_SOURCE_DIR}/res")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)


set(GLM_BUILD_LIBRARY OFF CACHE BOOL "" FORCE)
set(GLM_BUILD_TESTS OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glm)
target_link_libraries(${PROJECT_NAME} glm::glm)

add_subdirectory(external/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} glad)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD

        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/res/shaders"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/res"
)
