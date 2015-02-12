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

void remove_node(List *list, unsigned int pid) {
    if(list->first->pid == pid) {
        Node *temp = list->first;
        list->first=list->first->next;
        free(temp);
    } else {
        Node *node = list->first;
        Node *prev_node;
 //       printf("in remove\n");
        while(node->next != NULL) {
   //         printf("in while loop\n");
            if(node->pid == pid) {
     //           printf("\tPID MATCHES\n");
                prev_node->next = node->next;
                free(node);
                break;
            }
        }
    }
}
void delete_node(Node *node) {
    free(node);
}

void append_list(List *list, unsigned int pid, int priority) {
    Node *node = list->first;
    while(node->next != NULL) { node=node->next; }
    node->pid=pid;
    node->priority=priority;
    node->next=create_node();
}

void insert(List *list, unsigned int pid, int priority) {
    Node *temp; Node *node;
    temp=create_node();
    temp->pid=pid;
    temp->priority = priority;
    if(list->first == NULL || priority < list->first->priority) {
        temp->next=list->first;
        list->first = temp;
    } else {
        node=list->first;
        Node *prev_node;
        while(node->next != NULL) {
            if(node->priority >= priority) {
               prev_node->next=temp;
               temp->next=node;
               return;
            }
            prev_node=node;
            node=node->next;
        }
               prev_node->next=temp;
               temp->next=node;
    }
}

Node *pop(List *list) {
    Node *temp = list->first;
    if(temp->next == NULL) {
        Node *new_first = create_node();
        list->first=new_first;
    } else {
        Node *new_first=temp->next;
        list->first=new_first;
    }
    return temp;
}

Node *peek(List *list) { return list->first; }

Node *get_element(List *list, int element) {
    Node *node = list->first;
    int i = 0;
    while(i < element && node != NULL) { node=node->next; i++; }
    return node;
}

Node *find_pid(List *list, unsigned int pid) {
    Node *node = list->first;
    while(node->next != NULL) {
        if(node->pid == pid) { return node; }
        node=node->next;
    }
    return NULL;
}

int print_list(List *list) {
    Node *node = list->first;
    int i=0;
    while(node->next != NULL) {
        printf("Pid: %d\t Priority: %d\n",node->pid,node->priority);
        node = node->next;
        i++;
    }
    printf("\n");
    return i;
}
/*
int main(int argc, char *argv[]) {
    List *list = create_list();
    int i;
    append_list(list,1,6);
    print_list(list);
    append_list(list,2,8);
    insert(list,3,4);
    insert(list,4,1);
    insert(list,5,5);
    insert(list,6,-2);
    insert(list,7,10);
    insert(list,8,-100);
    remove_node(list,7);
    print_list(list);
    delete_list(list);
}
*/
