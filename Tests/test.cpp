#include <gtest/gtest.h>

#include "parse.h"
#include "tree.h"

using namespace ParseFasta;
using namespace SuffixTree;

TEST(tree_tests, CorrectNodeCounts)
{
    auto tree = Tree::build("banana", "ban");
    EXPECT_EQ(7, tree.leaf_node_count());
    EXPECT_EQ(4, tree.internal_node_count());
    EXPECT_EQ(11, tree.total_node_count());


    tree = Tree::build("MISSISSIPPI", "MISP");
    EXPECT_EQ(12, tree.leaf_node_count());
    EXPECT_EQ(7, tree.internal_node_count());
    EXPECT_EQ(19, tree.total_node_count());


    tree = Tree::build("BANANABAN", "BAN");
    EXPECT_EQ(10, tree.leaf_node_count());
    EXPECT_EQ(7, tree.internal_node_count());
    EXPECT_EQ(17, tree.total_node_count());
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}