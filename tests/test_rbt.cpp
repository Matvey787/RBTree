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

TEST(RBT_RangeQueryTest, RangeQueryTest1) {
    RBTree<int> tree;
    for (int x : {10, 20, 30, 40, 50}) tree.insert(x);

    int cnt = range_query(tree, 15, 45);
    EXPECT_EQ(cnt, 3);
}

// Тест с диапазоном внутри элементов
TEST(RBT_RangeQueryTest, RangeQueryWithin) {
    RBTree<int> tree;
    for (int x : {10, 20, 30, 40, 50}) tree.insert(x);

    int cnt = range_query(tree, 15, 45);
    EXPECT_EQ(cnt, 3);
}

// Тест с диапазоном, включающим все элементы
TEST(RBT_RangeQueryTest, RangeQueryFullRange) {
    RBTree<int> tree;
    for (int x : {10, 20, 30, 40, 50}) tree.insert(x);

    int cnt = range_query(tree, 0, 100);
    EXPECT_EQ(cnt, 5);
}

// Тест с диапазоном меньше минимального элемента
TEST(RBT_RangeQueryTest, RangeQueryBelowMin) {
    RBTree<int> tree;
    for (int x : {10, 20, 30, 40, 50}) tree.insert(x);

    int cnt = range_query(tree, 0, 5);
    EXPECT_EQ(cnt, 0);
}

// Тест с диапазоном больше максимального элемента
TEST(RBT_RangeQueryTest, RangeQueryAboveMax) {
    RBTree<int> tree;
    for (int x : {10, 20, 30, 40, 50}) tree.insert(x);

    int cnt = range_query(tree, 55, 100);
    EXPECT_EQ(cnt, 0);
}

// Тест с диапазоном равным существующему элементу
TEST(RBT_RangeQueryTest, RangeQuerySingleElement) {
    RBTree<int> tree;
    for (int x : {10, 20, 30, 40, 50}) tree.insert(x);

    int cnt = range_query(tree, 30, 30);
    EXPECT_EQ(cnt, 1);
}

// Тест с диапазоном из одного элемента, отсутствующего в дереве
TEST(RBT_RangeQueryTest, RangeQuerySingleAbsentElement) {
    RBTree<int> tree;
    for (int x : {10, 20, 30, 40, 50}) tree.insert(x);

    int cnt = range_query(tree, 35, 35);
    EXPECT_EQ(cnt, 0);
}

// Тест с перевернутым диапазоном (fst > snd)
TEST(RBT_RangeQueryTest, RangeQueryReversedRange) {
    RBTree<int> tree;
    for (int x : {10, 20, 30, 40, 50}) tree.insert(x);

    int cnt = range_query(tree, 45, 15);
    EXPECT_EQ(cnt, 0);
}

// Тест с дубликатами элементов
TEST(RBT_RangeQueryTest, RangeQueryWithDuplicates) {
    RBTree<int> tree;
    for (int x : {10, 20, 20, 30, 30, 30, 40, 50}) tree.insert(x);

    int cnt = range_query(tree, 20, 30);
    EXPECT_EQ(cnt, 2);
}

// Тест с диапазоном, включающим все дубликаты одного значения
TEST(RBT_RangeQueryTest, RangeQuerySingleDuplicateValue) {
    RBTree<int> tree;
    for (int x : {10, 10, 10, 20, 30}) tree.insert(x);

    int cnt = range_query(tree, 10, 10);
    EXPECT_EQ(cnt, 1);
}