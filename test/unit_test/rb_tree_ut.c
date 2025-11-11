#define CTEST_IMPLEMENTATION
#include "ctest.h"
#include "src/shared/rb_tree.h"

TEST(RBTree, Create) {
    RBTree *tree = rbtree_create(sizeof(int));
    EXPECT_NE(tree, NULL);
    EXPECT_EQ(rbtree_size(tree), 0);
    EXPECT_EQ(tree->root, NULL);
    rbtree_destroy(tree);
}

TEST(RBTree, Insert) {
    RBTree *tree = rbtree_create(sizeof(int));
    
    int val1 = 100;
    int val2 = 50;
    int val3 = 150;
    
    EXPECT_EQ(rbtree_insert(tree, 10, &val1), 0);
    EXPECT_EQ(rbtree_size(tree), 1);
    
    EXPECT_EQ(rbtree_insert(tree, 5, &val2), 0);
    EXPECT_EQ(rbtree_size(tree), 2);
    
    EXPECT_EQ(rbtree_insert(tree, 15, &val3), 0);
    EXPECT_EQ(rbtree_size(tree), 3);
    
    rbtree_destroy(tree);
}

TEST(RBTree, Get) {
    RBTree *tree = rbtree_create(sizeof(int));
    
    int val1 = 100;
    int val2 = 200;
    int val3 = 300;
    
    rbtree_insert(tree, 10, &val1);
    rbtree_insert(tree, 20, &val2);
    rbtree_insert(tree, 30, &val3);
    
    EXPECT_EQ(*(int*)rbtree_get(tree, 10), 100);
    EXPECT_EQ(*(int*)rbtree_get(tree, 20), 200);
    EXPECT_EQ(*(int*)rbtree_get(tree, 30), 300);
    
    // Non-existent key
    EXPECT_EQ(rbtree_get(tree, 999), NULL);
    
    rbtree_destroy(tree);
}

TEST(RBTree, Contains) {
    RBTree *tree = rbtree_create(sizeof(int));
    
    int val = 42;
    rbtree_insert(tree, 10, &val);
    rbtree_insert(tree, 20, &val);
    
    EXPECT_EQ(rbtree_contains(tree, 10), 1);
    EXPECT_EQ(rbtree_contains(tree, 20), 1);
    EXPECT_EQ(rbtree_contains(tree, 30), 0);
    
    rbtree_destroy(tree);
}

TEST(RBTree, UpdateValue) {
    RBTree *tree = rbtree_create(sizeof(int));
    
    int val1 = 100;
    int val2 = 999;
    
    rbtree_insert(tree, 10, &val1);
    EXPECT_EQ(*(int*)rbtree_get(tree, 10), 100);
    
    rbtree_insert(tree, 10, &val2);
    EXPECT_EQ(*(int*)rbtree_get(tree, 10), 999);
    EXPECT_EQ(rbtree_size(tree), 1);
    
    rbtree_destroy(tree);
}

TEST(RBTree, Delete) {
    RBTree *tree = rbtree_create(sizeof(int));
    
    int val = 42;
    for (int i = 1; i <= 5; i++) {
        rbtree_insert(tree, i * 10, &val);
    }
    
    EXPECT_EQ(rbtree_size(tree), 5);
    
    EXPECT_EQ(rbtree_delete(tree, 30), 0);
    EXPECT_EQ(rbtree_size(tree), 4);
    EXPECT_EQ(rbtree_contains(tree, 30), 0);
    
    EXPECT_EQ(rbtree_delete(tree, 10), 0);
    EXPECT_EQ(rbtree_size(tree), 3);
    
    // Delete non-existent key
    EXPECT_EQ(rbtree_delete(tree, 999), -1);
    EXPECT_EQ(rbtree_size(tree), 3);
    
    rbtree_destroy(tree);
}

TEST(RBTree, DeleteRoot) {
    RBTree *tree = rbtree_create(sizeof(int));
    
    int val = 42;
    rbtree_insert(tree, 50, &val);
    rbtree_insert(tree, 25, &val);
    rbtree_insert(tree, 75, &val);
    
    EXPECT_EQ(rbtree_delete(tree, 50), 0);
    EXPECT_EQ(rbtree_size(tree), 2);
    EXPECT_EQ(rbtree_contains(tree, 50), 0);
    
    rbtree_destroy(tree);
}

TEST(RBTree, MultipleInserts) {
    RBTree *tree = rbtree_create(sizeof(int));
    
    int val = 42;
    for (int i = 1; i <= 10; i++) {
        EXPECT_EQ(rbtree_insert(tree, i, &val), 0);
    }
    
    EXPECT_EQ(rbtree_size(tree), 10);
    
    for (int i = 1; i <= 10; i++) {
        EXPECT_EQ(rbtree_contains(tree, i), 1);
    }
    
    rbtree_destroy(tree);
}

TEST(RBTree, DuplicateInserts) {
    RBTree *tree = rbtree_create(sizeof(int));
    
    int val1 = 100;
    int val2 = 200;
    
    EXPECT_EQ(rbtree_insert(tree, 10, &val1), 0);
    EXPECT_EQ(rbtree_insert(tree, 10, &val2), 0);
    EXPECT_EQ(rbtree_insert(tree, 10, &val1), 0);
    
    EXPECT_EQ(rbtree_size(tree), 1);
    
    rbtree_destroy(tree);
}

TEST(RBTree, StructValues) {
    typedef struct {
        int id;
        char name[20];
    } Person;
    
    RBTree *tree = rbtree_create(sizeof(Person));
    
    Person p1 = {1, "Alice"};
    Person p2 = {2, "Bob"};
    Person p3 = {3, "Charlie"};
    
    EXPECT_EQ(rbtree_insert(tree, 100, &p1), 0);
    EXPECT_EQ(rbtree_insert(tree, 200, &p2), 0);
    EXPECT_EQ(rbtree_insert(tree, 300, &p3), 0);
    
    Person *retrieved = (Person*)rbtree_get(tree, 200);
    EXPECT_EQ(retrieved->id, 2);
    EXPECT_STR_EQ(retrieved->name, "Bob");
    
    rbtree_destroy(tree);
}

TEST(RBTree, NegativeKeys) {
    RBTree *tree = rbtree_create(sizeof(int));
    
    int val = 42;
    rbtree_insert(tree, -10, &val);
    rbtree_insert(tree, 0, &val);
    rbtree_insert(tree, 10, &val);
    
    EXPECT_EQ(rbtree_size(tree), 3);
    EXPECT_EQ(rbtree_contains(tree, -10), 1);
    EXPECT_EQ(rbtree_contains(tree, 0), 1);
    EXPECT_EQ(rbtree_contains(tree, 10), 1);
    
    rbtree_destroy(tree);
}

TEST(RBTree, DeleteAllElements) {
    RBTree *tree = rbtree_create(sizeof(int));
    
    int val = 42;
    for (int i = 1; i <= 5; i++) {
        rbtree_insert(tree, i, &val);
    }
    
    EXPECT_EQ(rbtree_size(tree), 5);
    
    for (int i = 1; i <= 5; i++) {
        EXPECT_EQ(rbtree_delete(tree, i), 0);
    }
    
    EXPECT_EQ(rbtree_size(tree), 0);
    EXPECT_EQ(tree->root, NULL);
    
    rbtree_destroy(tree);
}

TEST(RBTree, RedBlackProperties) {
    RBTree *tree = rbtree_create(sizeof(int));
    
    int val = 42;
    
    // Insert sequential values to test rebalancing
    for (int i = 1; i <= 15; i++) {
        rbtree_insert(tree, i, &val);
    }
    
    // Root must be black
    EXPECT_EQ(tree->root->color, RB_BLACK);
    
    // Tree should maintain all keys
    EXPECT_EQ(rbtree_size(tree), 15);
    
    for (int i = 1; i <= 15; i++) {
        EXPECT_EQ(rbtree_contains(tree, i), 1);
    }
    
    rbtree_destroy(tree);
}

TEST(RBTree, NullPointer) {
    EXPECT_EQ(rbtree_size(NULL), 0);
    EXPECT_EQ(rbtree_contains(NULL, 10), 0);
    EXPECT_EQ(rbtree_get(NULL, 10), NULL);
}

TEST(RBTree, NoValue) {
    RBTree *tree = rbtree_create(0);
    
    EXPECT_EQ(rbtree_insert(tree, 10, NULL), 0);
    EXPECT_EQ(rbtree_insert(tree, 20, NULL), 0);
    
    EXPECT_EQ(rbtree_size(tree), 2);
    EXPECT_EQ(rbtree_get(tree, 10), NULL);
    
    rbtree_destroy(tree);
}

CTEST_MAIN()
