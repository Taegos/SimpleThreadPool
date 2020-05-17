include_directories (${CMAKE_SOURCE_DIR}/src/headers)
link_directories(${CMAKE_SOURCE_DIR}/src)
add_executable (Demo demo.cpp)
target_link_libraries (Demo ThreadPool)