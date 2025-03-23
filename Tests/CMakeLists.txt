



set( gtest_force_shared_crt TRUE)

add_executable(my_tests test.cpp)

target_link_libraries(my_tests PRIVATE
    gtest
    gtest_main
    )

enable_testing()

add_test(NAME MyTests COMMAND my_tests)

add_subdirectory(googletest)