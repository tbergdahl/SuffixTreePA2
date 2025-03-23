#include <gtest/gtest.h>

#include "parse.h"

using namespace ParseFasta;

TEST(parse_tests, CorrectEmptyString)
{
    EXPECT_EQ("", ParseFasta::parse("", ""));
}

// Main entry point (optional, but typical)
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}