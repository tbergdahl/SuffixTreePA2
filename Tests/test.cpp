#include <gtest/gtest.h>

#include "parse.h"
#include "tree.h"

using namespace ParseFasta;
using namespace SuffixTree;

TEST(parse_tests, CorrectEmptyString)
{
    EXPECT_EQ("", ParseFasta::parse("", ""));
}

TEST(tree_tests, CorrectNodeCounts)
{
    auto tree = Tree::build("banana", "ban");
    EXPECT_EQ(7, tree.leaf_count());
    EXPECT_EQ(3, tree.internal_count());

    tree = Tree::build("MISSISSIPI", "MISP");
    EXPECT_EQ(11, tree.leaf_count());
    EXPECT_EQ(6, tree.internal_count());

}






int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}