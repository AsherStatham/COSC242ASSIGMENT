#include <stdio.h>
#include <stdlib.h>
#include "rbt.h"
#include "mylib.h"
#include <string.h>

#define IS_BLACK(x) ((NULL == (x)) || (BLACK == (x)->colour))
#define IS_RED(x) ((NULL != (x)) && (RED == (x)->colour))

typedef enum { RED, BLACK} rbt_colour;

struct rbt_node{
    char *key;
    rbt_colour colour;
    rbt left;
    rbt right;
};

rbt rbt_new(){
    return NULL;
}

static rbt right_rotate(rbt r){
    rbt temp;
    temp = r;
    r = r->left;
    temp->left = r->right;
    r->right = temp;
    return r;
}

static rbt left_rotate(rbt r){
    rbt temp;
    temp = r;
    r = r->right;
    temp->right = r->left;
    r->left = temp;
    return r;
}

static rbt rbt_fix(rbt r){
    if (IS_RED(r->left) && IS_RED(r->left->left)){
        if (IS_RED(r->right)){
            r->colour = RED;
            r->left->colour = BLACK;
            r->right->colour = BLACK;
        } else if (IS_BLACK(r->right)) {
            r = right_rotate(r);
            r->colour = BLACK;
            r->right->colour = RED;
        }
    } else if (IS_RED(r->left) && IS_RED(r->left->right)){
        if (IS_RED(r->right)){
            r->colour = RED;
            r->left->colour = BLACK;
            r->right->colour = BLACK;
        } else if(IS_BLACK(r->right)) {
            r->left = left_rotate(r->left);
            r = right_rotate(r);
            r->colour = BLACK;
            r->right->colour = RED;
        }
    } else if (IS_RED(r->right) && IS_RED(r->right->left)){
        if (IS_RED(r->left)){
            r->colour = RED;
            r->left->colour = BLACK;
            r->right->colour = BLACK;
        } else if (IS_BLACK(r->left)){
            r->right = right_rotate(r->right);
            r = left_rotate(r);
            r->colour = BLACK;
            r->left->colour = RED;
        }
    } else if (IS_RED(r->right) && IS_RED(r->right->right)){
        if (IS_RED(r->left)){
            r->colour = RED;
            r->left->colour = BLACK;
            r->right->colour = BLACK;
        } else if (IS_BLACK(r->left)){
            r = left_rotate(r);
            r->colour = BLACK;
            r->left->colour = RED;
        }
    }
    return r;
}

rbt rbt_insert(rbt b, char *str){
    if (b == NULL){
	b = emalloc(sizeof *b);
	b->key = emalloc(strlen(str) * sizeof str[0]+1);
	strcpy(b->key, str);
        b->colour = RED;
    } else if (strcmp(b->key, str) == 0){
    } else if (strcmp(b->key, str) < 0){
	b->right = rbt_insert(b->right, str);
    } else {
	b->left = rbt_insert(b->left, str);
    }
    b = rbt_fix(b);
    return b;
}

int rbt_search(rbt b, char *str){
    if (b == NULL){
	return 0;
    } else if (strcmp(b->key, str) == 0){
	return 1;
    } else if (strcmp(b->key, str) < 0){
	return rbt_search(b->right, str);
    } else {
	return rbt_search(b->left, str);
    }
}

void rbt_inorder(rbt b, void f(char *str, char *c)){
    char *colour;
    if (b == NULL){
	return;
    }
    if (b->colour == BLACK){
        colour = "black";
    } else {
        colour = "red";
    }
    rbt_inorder(b->left, f);
    f(b->key, colour);
    rbt_inorder(b->right, f);
}

void rbt_preorder(rbt b, void f(char *str, char *c)){
    char *colour;
    if (b == NULL){
	return;
    }
    if (b->colour == BLACK){
        colour = "black";
    } else {
        colour = "red";
    }
    f(b->key, colour);
    rbt_preorder(b->left, f);
    rbt_preorder(b->right, f);
}

rbt rbt_free(rbt b){
    free(b->key);
    free(b);
    return NULL;
}

rbt splice(rbt b){
    if (b->left == NULL && b->right == NULL){
	rbt_free(b);
	return NULL;
    } else if (b->left != NULL && b->right == NULL){
	rbt_free(b);
	return b->left;
    } else if (b->left == NULL && b->right != NULL){
	rbt_free(b);
	return b->right;
    } else if (b->left != NULL && b->right != NULL){
	rbt temp;
	temp = b->right;
	while (temp->left != NULL){
	    temp = temp->left;
	}
	strcpy(b->key, temp->key);
	b->right = splice(b->right);
	return b;
    } else {
	return b;
    }
}

rbt rbt_delete(rbt b, char *str){
    if (b == NULL){
	return b;
    } else if (strcmp(b->key, str) == 0){
	b = splice(b);
    } else if (strcmp(b->key, str) < 0){
	b->right = rbt_delete(b->right,str);
    } else{
	b->left = rbt_delete(b->left, str);
    }
    return b;
}
