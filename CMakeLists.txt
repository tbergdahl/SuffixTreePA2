cmake_minimum_required(VERSION 3.10)
project(PA2)

set(CMAKE_CXX_STANDARD 17)

# for tests
if(MSVC)
    # that was brutal
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT")
endif()

# Constants for project directories
set(PROJECT_ROOT_DIR ${CMAKE_SOURCE_DIR})

# Define the executable target
add_executable(PA2 main.cpp)

# Constants for project directories for submodules
set(SUFFIX_TREE_DIR ${PROJECT_ROOT_DIR}/SuffixTree)
set(PARSE_FASTA_DIR ${PROJECT_ROOT_DIR}/ParseFasta)

# Include directories variables
set(SUFFIX_TREE_INCLUDE ${SUFFIX_TREE_DIR}/include)
set(PARSE_FASTA_INCLUDE ${PARSE_FASTA_DIR}/include)

# Include directories for the PA2
target_include_directories(PA2 PRIVATE
    ${SUFFIX_TREE_INCLUDE}
    ${PARSE_FASTA_INCLUDE}
)

# Link libraries (Static libraries for SuffixTree and ParseFasta)
target_link_libraries(PA2 PRIVATE
    SuffixTree
    ParseFasta
)



# Add subdirectories for SuffixTree, ParseFasta, and Tests
add_subdirectory(SuffixTree)
add_subdirectory(ParseFasta)
add_subdirectory(Tests)