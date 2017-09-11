#include <stdio.h>
#include <stdlib.h>
#include "mylib.h"
#include "flexarray.h"
#include <string.h>

struct flexarrayrec {
    int capacity;
    int itemcount;
    char **items;
};

static void insertion_sort(char **a, int n){
    int p, i;
    char *key;
    for (p = 1; p < n; p++){
        key = emalloc(strlen(a[p] + 1) * sizeof a[0][0]);
        key = a[p];
        for (i = p-1; i > -1; i--){
            if (strcmp(a[i], key) > 0){
                a[i+1] = a[i];
                a[i] = key;
            }else{
                break;
            }
        }
    }
}

static void quicksort(char **a, int n){
    int i, j;
    char *temp, *pivot;
    if (n < 2){
	return;
    }
    if (n < 40){
	insertion_sort(a, n);
	return;
    }
    pivot = a[0];
    i = -1;
    j = n;
    for (;;) {
	do {
	    i++;
	} while (strcmp(a[i], pivot) < 0);
	do {
	    j--;
	} while (strcmp(a[j], pivot) > 0);
        
        if (i < j){
            temp = a[i];
            a[i] = a[j];
            a[j] = temp;
        } else {
            break;
        }
    }
    quicksort(&a[0], j+1);
    quicksort(&a[j+1], n-j-1);
}


flexarray flexarray_new(){
    flexarray result = emalloc(sizeof *result);
    result -> capacity = 2;
    result -> itemcount = 0;
    result -> items = emalloc(result -> capacity * sizeof result -> items[0]);
    return result;
}

void flexarray_append(flexarray f, char *string){
    if (f -> itemcount == f -> capacity){
	f->capacity += f->capacity;
	f->items = erealloc(f->items, f->capacity * sizeof f[0]);
    }
    f->items[f->itemcount] = emalloc((strlen(string) + 1) * sizeof f->items[0][0]);
    strcpy(f->items[f->itemcount], string);
    f->itemcount++;
}

void flexarray_print(flexarray f){
    int i;
    for (i = 0; i < f->itemcount; i++){
	printf("%s\n", f->items[i]);
    }
}

void flexarray_sort(flexarray f){
    quicksort(f->items, f->itemcount);
}

void flexarray_free(flexarray f){
    free(f);
}

int is_present(flexarray f, char *string){
    int i;
    if (f->itemcount == 0){
        return 0;
    }
    for(i = 0; i < f->itemcount; i++){
        if (strcmp(f->items[i], string) == 0){
            return 1;
        }
    }
    return 0;
}
