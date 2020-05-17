include_directories (${CMAKE_SOURCE_DIR}/src/headers)
link_directories(${CMAKE_SOURCE_DIR}/src)
add_executable (Test test.cpp)
target_link_libraries (Test ThreadPool)