set(target tables_test)

#file(GLOB hdrs "*.h*" "../include/*.h" "../gtest/*.h")
file(GLOB hdrs "*.h*")
file(GLOB srcs "*.cpp" "../src/*.cpp")

add_executable(${target} ${srcs} ${hdrs}
        test_sorted.cpp)
target_link_libraries(${target} gtest)