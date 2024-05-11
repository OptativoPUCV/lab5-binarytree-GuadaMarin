#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * new = (TreeMap *) malloc(sizeof(TreeMap));
    if(!new) return NULL;
    new->root= NULL;
    new->current= NULL;
    new->lower_than= lower_than;

    
    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    if (tree->root == NULL){
        tree->root= createTreeNode(key,value);
        tree->current= tree->root;
    }
    
    TreeNode * parent = NULL, * node= tree->root;
    while(node!=NULL){
        parent = node;
        if(tree->lower_than(key, node->pair->key)){
            node= node->left;
        }
        else if(tree->lower_than(node->pair->key, key)){
                node= node->right;
        }
        else 
            return;
    }
    TreeNode * new_node = createTreeNode(key, value);
    if (parent != NULL) {
        new_node->parent = parent;
        if (tree->lower_than(key, parent->pair->key)) {
            parent->left = new_node;
        } else {
            parent->right = new_node;
        }
    }
    tree->current = new_node;

}

TreeNode * minimum(TreeNode * x){
    while(x && x->left != NULL){
        x= x->left;
    }
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    TreeNode * replace = NULL;
    if(node->left == NULL || node->right == NULL){
        replace= (node->left !=NULL) ? node->left : node->right;
        if(replace)replace->parent = node->parent;
        if(node->parent == NULL){
            tree->root = replace;
        }
        else if(node == node->parent->left){
            node->parent->left= replace;
        }
        else
            node->parent->right= replace;
        free(node->pair);
        free(node);
    }
        
    else {
        struct TreeNode * successor = minimum(node->right);
        node->pair->key = successor->pair->key;
        node->pair->value = successor->pair->value;
        removeNode(tree, successor);
    }

}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode * node= tree->root;
    while(node!=NULL){
        if(tree->lower_than(key, node->pair->key)){
            node=node->left;
        }
        else if(tree->lower_than(node->pair->key, key)){
                node=node->right;
        }
        else{
            tree->current = node;
            return node->pair;
        }
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    TreeNode * node= minimum(tree->root);
    tree->current = node;
    return node ? node->pair : NULL;
}

Pair * nextTreeMap(TreeMap * tree) {
    TreeNode * node= tree->current;
    if(node->right != NULL){
        node = minimum(node->right);
    }
    else {
        TreeNode * parent= node->parent;
        while(parent != NULL && node == parent->right){
            node = parent;
            parent= parent->parent;
        }
        node= parent;
    }
    tree->current = node;
    return node ? node->pair : NULL;
}
