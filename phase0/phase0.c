#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "linked_list.h"


List *create_list() {
    List *new_list = malloc(sizeof(List));
    Node *first = create_node();
    new_list->first=first;
    return new_list;
}

Node *create_node() {
    Node *temp = malloc(sizeof(Node));
    temp->data="";
    temp->next=NULL;
    return temp;
}

void delete_list(List *list) {
    Node *node = list->first;
    Node *next;
    while(node) {
        next = node->next;
        delete_node(node);
        node=next;
    }
    free(list);
}

void delete_node(Node *node) {
    free(node);
}

void append_list(List *list, char *str) {
    Node *node = list->first;
    while(node->next != NULL) { node=node->next; }
    node->data=str;
    node->next=create_node();
}

int print_list(List *list) {
    Node *node = list->first;
    int i=0;
    while(node != NULL) {
        printf("%s\t",node->data);
        node = node->next;
        i++;
    }
    printf("\n");
    delete_list(list);
    return i-1;
}

int main(int argc, char *argv[]) {
    List *list = create_list();
    int i;
    for(i = 0; i < argc; i++) {
        append_list(list,argv[i]);
    }
    int num = print_list(list);
    printf("%i arguments printed\n",num);

}
