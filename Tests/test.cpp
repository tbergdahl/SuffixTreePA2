#include <gtest/gtest.h>

int add(int a, int b) {
    return a + b;
}

TEST(MathTests, AddWorks) {
    EXPECT_EQ(add(1, 1), 2);
    EXPECT_EQ(add(-1, 1), 0);
}

TEST(MathTests, AddFailsExample) {
    EXPECT_NE(add(2, 2), 5);  // This test passes because 4 != 5
}

// Main entry point (optional, but typical)
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}