typedef struct node {
    unsigned int pid;
    int priority;
    struct node *next;
} Node;


Node *create_node();
void delete_node(Node *node);

typedef struct list {
    Node *first;
} List;

extern List *create_list();
extern void append_list(List *list, unsigned int pid, int priority);
extern void insert(List *list, unsigned int pid, int priority);
extern Node *get_element(List *list, int element);
extern Node *peek(List *list);
extern Node *pop(List* list);
extern Node *find_status(List *list, unsigned int status);
extern Node *find_pid(List *list, unsigned int pid);
extern int print_list(List *list);
extern void delete_list(List *list);
extern void remove_node(List *list, unsigned int pid);
