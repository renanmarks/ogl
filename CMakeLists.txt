project(ogl)
cmake_minimum_required(VERSION 3.1)

set (CMAKE_CXX_STANDARD 14)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

aux_source_directory(. SRC_LIST)

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARY} ${OPENGL_LIBRARIES} ${SDL2_LIBRARIES})
