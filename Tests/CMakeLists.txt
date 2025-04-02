set( gtest_force_shared_crt OFF)

add_executable(my_tests test.cpp)

target_link_libraries(my_tests PRIVATE
    gtest
    gtest_main
    SuffixTree
    parsefasta
    )

enable_testing()

target_include_directories(my_tests PRIVATE
    ${SUFFIX_TREE_INCLUDE}
    ${PARSE_FASTA_INCLUDE}
)


add_test(NAME MyTests COMMAND my_tests)

add_subdirectory(googletest)