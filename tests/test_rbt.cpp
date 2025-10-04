#include <gtest/gtest.h>
#include "RBT.hpp"

using namespace RBT;

TEST(RBT_InsertTest, RootIsBlackAfterFirstInsert) {
    RBTree<int> tree;
    tree.insert(10);

    auto root = tree.lower_bound(10);
    ASSERT_NE(root, nullptr);
    EXPECT_EQ(root->key(), 10);
    EXPECT_EQ(root->color(), color_t::BLACK); // Корень всегда чёрный
}

TEST(RBT_InsertTest, InsertMaintainsBSTProperty) {
    RBTree<int> tree;
    tree.insert(10);
    tree.insert(5);
    tree.insert(15);

    auto left = tree.lower_bound(5);
    auto root = tree.lower_bound(10);
    auto right = tree.lower_bound(15);

    ASSERT_NE(root->left(), nullptr);
    ASSERT_NE(root->right(), nullptr);
    EXPECT_EQ(root->left()->key(), 5);
    EXPECT_EQ(root->right()->key(), 15);
}

TEST(RBT_SuccessorTest, SuccessorWithRightSubtree) {
    RBTree<int> tree;
    tree.insert(20);
    tree.insert(10);
    tree.insert(30);
    tree.insert(25);

    auto node20 = tree.lower_bound(20);
    auto succ = tree.successor(node20);

    ASSERT_NE(succ, nullptr);
    EXPECT_EQ(succ->key(), 25);
}

TEST(RBT_SuccessorTest, SuccessorWithoutRightSubtree) {
    RBTree<int> tree;
    tree.insert(20);
    tree.insert(10);
    tree.insert(30);

    auto node10 = tree.lower_bound(10);
    auto succ = tree.successor(node10);

    ASSERT_NE(succ, nullptr);
    EXPECT_EQ(succ->key(), 20);
}

TEST(RBT_SuccessorTest, MaxElementHasNoSuccessor) {
    RBTree<int> tree;
    tree.insert(20);
    tree.insert(10);
    tree.insert(30);

    auto node30 = tree.lower_bound(30);
    auto succ = tree.successor(node30);

    EXPECT_EQ(succ, nullptr);
}

TEST(RBT_BoundsTest, LowerBoundAndUpperBound) {
    RBTree<int> tree;
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);

    auto lb = tree.lower_bound(15);
    ASSERT_NE(lb, nullptr);
    EXPECT_EQ(lb->key(), 20);

    auto ub = tree.upper_bound(20);
    ASSERT_NE(ub, nullptr);
    EXPECT_EQ(ub->key(), 30);
}

TEST(RBT_RangeQueryTest, CountElementsInRange) {
    RBTree<int> tree;
    for (int x : {10, 20, 30, 40, 50}) tree.insert(x);

    int cnt = range_query(tree, 15, 45);
    EXPECT_EQ(cnt, 3); // 20,30,40
}
