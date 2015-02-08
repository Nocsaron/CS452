typedef struct node {
    unsigned int pid;
    unsigned int status;
    struct node *next;
} Node;


Node *create_node();
void delete_node(Node *node);

typedef struct list {
    Node *first;
} List;

List *create_list();
void append_list(List *list, unsigned int pid, unsigned int status);
Node get_element(List *list, int element);
int print_list(List *list);
void delete_list(List *list);
