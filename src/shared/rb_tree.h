#ifndef RBTREE_H__
#define RBTREE_H__

#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    RB_RED,
    RB_BLACK
} RBColor;

typedef struct RBNode {
    int key;
    void *value;
    RBColor color;
    struct RBNode *parent;
    struct RBNode *left;
    struct RBNode *right;
} RBNode;

typedef struct {
    RBNode *root;
    size_t size;
    size_t value_size;
} RBTree;

// Create a new Red Black Tree
static inline RBTree* rbtree_create(size_t value_size) {
    RBTree *tree = (RBTree *)malloc(sizeof(RBTree));
    if (!tree) return NULL;
    
    tree->root = NULL;
    tree->size = 0;
    tree->value_size = value_size;
    
    return tree;
}

// Create a new node
static inline RBNode* rbtree_node_create(int key, const void *value, size_t value_size) {
    RBNode *node = (RBNode *)malloc(sizeof(RBNode));
    if (!node) return NULL;
    
    node->key = key;
    node->color = RB_RED;
    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;
    
    if (value_size > 0) {
        node->value = malloc(value_size);
        if (!node->value) {
            free(node);
            return NULL;
        }
        memcpy(node->value, value, value_size);
    } else {
        node->value = NULL;
    }
    
    return node;
}

// Rotate left
static inline void rbtree_rotate_left(RBTree *tree, RBNode *x) {
    RBNode *y = x->right;
    x->right = y->left;
    
    if (y->left != NULL) {
        y->left->parent = x;
    }
    
    y->parent = x->parent;
    if (x->parent == NULL) {
        tree->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    
    y->left = x;
    x->parent = y;
}

// Rotate right
static inline void rbtree_rotate_right(RBTree *tree, RBNode *y) {
    RBNode *x = y->left;
    y->left = x->right;
    
    if (x->right != NULL) {
        x->right->parent = y;
    }
    
    x->parent = y->parent;
    if (y->parent == NULL) {
        tree->root = x;
    } else if (y == y->parent->right) {
        y->parent->right = x;
    } else {
        y->parent->left = x;
    }
    
    x->right = y;
    y->parent = x;
}

// Fix insertion violations
static inline void rbtree_insert_fix(RBTree *tree, RBNode *k) {
    while (k->parent && k->parent->color == RB_RED) {
        if (k->parent == k->parent->parent->left) {
            RBNode *u = k->parent->parent->right;
            
            if (u && u->color == RB_RED) {
                k->parent->color = RB_BLACK;
                u->color = RB_BLACK;
                k->parent->parent->color = RB_RED;
                k = k->parent->parent;
            } else {
                if (k == k->parent->right) {
                    k = k->parent;
                    rbtree_rotate_left(tree, k);
                }
                k->parent->color = RB_BLACK;
                k->parent->parent->color = RB_RED;
                rbtree_rotate_right(tree, k->parent->parent);
            }
        } else {
            RBNode *u = k->parent->parent->left;
            
            if (u && u->color == RB_RED) {
                k->parent->color = RB_BLACK;
                u->color = RB_BLACK;
                k->parent->parent->color = RB_RED;
                k = k->parent->parent;
            } else {
                if (k == k->parent->left) {
                    k = k->parent;
                    rbtree_rotate_right(tree, k);
                }
                k->parent->color = RB_BLACK;
                k->parent->parent->color = RB_RED;
                rbtree_rotate_left(tree, k->parent->parent);
            }
        }
    }
    tree->root->color = RB_BLACK;
}

// Insert a key-value pair
static inline int rbtree_insert(RBTree *tree, int key, const void *value) {
    if (!tree) return -1;
    
    RBNode *y = NULL;
    RBNode *x = tree->root;
    
    while (x != NULL) {
        y = x;
        if (key < x->key) {
            x = x->left;
        } else if (key > x->key) {
            x = x->right;
        } else {
            // Key already exists, update value
            if (value && tree->value_size > 0) {
                memcpy(x->value, value, tree->value_size);
            }
            return 0;
        }
    }
    
    RBNode *z = rbtree_node_create(key, value, tree->value_size);
    if (!z) return -1;
    
    z->parent = y;
    
    if (y == NULL) {
        tree->root = z;
    } else if (key < y->key) {
        y->left = z;
    } else {
        y->right = z;
    }
    
    tree->size++;
    rbtree_insert_fix(tree, z);
    
    return 0;
}

// Find a node by key
static inline RBNode* rbtree_find_node(RBTree *tree, int key) {
    if (!tree) return NULL;
    
    RBNode *x = tree->root;
    while (x != NULL) {
        if (key < x->key) {
            x = x->left;
        } else if (key > x->key) {
            x = x->right;
        } else {
            return x;
        }
    }
    
    return NULL;
}

// Get value by key
static inline void* rbtree_get(RBTree *tree, int key) {
    RBNode *node = rbtree_find_node(tree, key);
    return node ? node->value : NULL;
}

// Check if key exists
static inline int rbtree_contains(RBTree *tree, int key) {
    return rbtree_find_node(tree, key) != NULL;
}

// Get the size
static inline size_t rbtree_size(RBTree *tree) {
    return tree ? tree->size : 0;
}

// Find minimum key node
static inline RBNode* rbtree_minimum(RBNode *node) {
    while (node && node->left != NULL) {
        node = node->left;
    }
    return node;
}

// Find maximum key node
static inline RBNode* rbtree_maximum(RBNode *node) {
    while (node && node->right != NULL) {
        node = node->right;
    }
    return node;
}

// Transplant subtrees
static inline void rbtree_transplant(RBTree *tree, RBNode *u, RBNode *v) {
    if (u->parent == NULL) {
        tree->root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    
    if (v != NULL) {
        v->parent = u->parent;
    }
}

// Fix deletion violations
static inline void rbtree_delete_fix(RBTree *tree, RBNode *x) {
    while (x != tree->root && (x == NULL || x->color == RB_BLACK)) {
        if (x == NULL) break;
        
        if (x == x->parent->left) {
            RBNode *s = x->parent->right;
            
            if (s && s->color == RB_RED) {
                s->color = RB_BLACK;
                x->parent->color = RB_RED;
                rbtree_rotate_left(tree, x->parent);
                s = x->parent->right;
            }
            
            if (s && (!s->left || s->left->color == RB_BLACK) && 
                (!s->right || s->right->color == RB_BLACK)) {
                if (s) s->color = RB_RED;
                x = x->parent;
            } else if (s) {
                if (!s->right || s->right->color == RB_BLACK) {
                    if (s->left) s->left->color = RB_BLACK;
                    s->color = RB_RED;
                    rbtree_rotate_right(tree, s);
                    s = x->parent->right;
                }
                s->color = x->parent->color;
                x->parent->color = RB_BLACK;
                if (s->right) s->right->color = RB_BLACK;
                rbtree_rotate_left(tree, x->parent);
                x = tree->root;
            }
        } else {
            RBNode *s = x->parent->left;
            
            if (s && s->color == RB_RED) {
                s->color = RB_BLACK;
                x->parent->color = RB_RED;
                rbtree_rotate_right(tree, x->parent);
                s = x->parent->left;
            }
            
            if (s && (!s->right || s->right->color == RB_BLACK) && 
                (!s->left || s->left->color == RB_BLACK)) {
                if (s) s->color = RB_RED;
                x = x->parent;
            } else if (s) {
                if (!s->left || s->left->color == RB_BLACK) {
                    if (s->right) s->right->color = RB_BLACK;
                    s->color = RB_RED;
                    rbtree_rotate_left(tree, s);
                    s = x->parent->left;
                }
                s->color = x->parent->color;
                x->parent->color = RB_BLACK;
                if (s->left) s->left->color = RB_BLACK;
                rbtree_rotate_right(tree, x->parent);
                x = tree->root;
            }
        }
    }
    
    if (x) x->color = RB_BLACK;
}

// Delete a node by key
static inline int rbtree_delete(RBTree *tree, int key) {
    if (!tree) return -1;
    
    RBNode *z = rbtree_find_node(tree, key);
    if (!z) return -1;
    
    RBNode *y = z;
    RBNode *x = NULL;
    RBColor y_original_color = y->color;
    
    if (!z->left) {
        x = z->right;
        rbtree_transplant(tree, z, z->right);
    } else if (!z->right) {
        x = z->left;
        rbtree_transplant(tree, z, z->left);
    } else {
        y = rbtree_minimum(z->right);
        y_original_color = y->color;
        x = y->right;
        
        if (y->parent == z) {
            if (x) x->parent = y;
        } else {
            rbtree_transplant(tree, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        
        rbtree_transplant(tree, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    
    if (y_original_color == RB_BLACK) {
        rbtree_delete_fix(tree, x);
    }
    
    free(z->value);
    free(z);
    tree->size--;
    
    return 0;
}

// Iterator for in-order traversal
typedef struct {
    RBNode **stack;
    size_t stack_size;
    size_t stack_capacity;
    RBNode *current;
} RBTreeIterator;

// Create an iterator
static inline RBTreeIterator* rbtree_iterator_create() {
    RBTreeIterator *iter = (RBTreeIterator *)malloc(sizeof(RBTreeIterator));
    if (!iter) return NULL;
    
    iter->stack_capacity = 50;
    iter->stack = (RBNode **)malloc(sizeof(RBNode *) * iter->stack_capacity);
    if (!iter->stack) {
        free(iter);
        return NULL;
    }
    
    iter->stack_size = 0;
    iter->current = NULL;
    
    return iter;
}

// Initialize iterator for in-order traversal
static inline void rbtree_iterator_init(RBTreeIterator *iter, RBNode *root) {
    if (!iter) return;
    
    iter->stack_size = 0;
    iter->current = root;
    
    // Push all left nodes onto stack
    while (iter->current != NULL) {
        if (iter->stack_size >= iter->stack_capacity) {
            iter->stack_capacity *= 2;
            RBNode **new_stack = (RBNode **)realloc(iter->stack, 
                                                     sizeof(RBNode *) * iter->stack_capacity);
            if (!new_stack) return;
            iter->stack = new_stack;
        }
        
        iter->stack[iter->stack_size++] = iter->current;
        iter->current = iter->current->left;
    }
}

// Check if iterator has next element
static inline int rbtree_iterator_has_next(RBTreeIterator *iter) {
    return iter && iter->stack_size > 0;
}

// Get next node in iteration
static inline RBNode* rbtree_iterator_next(RBTreeIterator *iter) {
    if (!iter || iter->stack_size == 0) return NULL;
    
    // Pop from stack
    RBNode *node = iter->stack[--iter->stack_size];
    iter->current = node;
    
    // Process right subtree
    RBNode *right = node->right;
    while (right != NULL) {
        if (iter->stack_size >= iter->stack_capacity) {
            iter->stack_capacity *= 2;
            RBNode **new_stack = (RBNode **)realloc(iter->stack, 
                                                     sizeof(RBNode *) * iter->stack_capacity);
            if (!new_stack) return node;
            iter->stack = new_stack;
        }
        
        iter->stack[iter->stack_size++] = right;
        right = right->left;
    }
    
    return node;
}

// Destroy iterator
static inline void rbtree_iterator_destroy(RBTreeIterator *iter) {
    if (!iter) return;
    
    free(iter->stack);
    free(iter);
}

// Free the entire tree
static inline void rbtree_destroy_helper(RBNode *node) {
    if (!node) return;
    
    rbtree_destroy_helper(node->left);
    rbtree_destroy_helper(node->right);
    
    free(node->value);
    free(node);
}

static inline void rbtree_destroy(RBTree *tree) {
    if (!tree) return;
    
    rbtree_destroy_helper(tree->root);
    free(tree);
}

#ifdef __cplusplus
}
#endif

#endif // RBTREE_H
