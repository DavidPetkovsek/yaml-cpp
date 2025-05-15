find_package(Threads REQUIRED)
add_executable(mem_test mem_test.cpp)
target_link_libraries(mem_test PUBLIC yaml-cpp::yaml-cpp)
target_link_libraries(mem_test PUBLIC Threads::Threads)