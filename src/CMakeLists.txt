include_directories(${CMAKE_SOURCE_DIR}/src/headers)

add_library(ThreadPool 
"ThreadPool.cpp"
"Semaphore.cpp"
"ExitSemaphore.cpp")