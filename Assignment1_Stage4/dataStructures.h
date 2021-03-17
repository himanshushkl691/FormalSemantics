#define NONE 0

#define EXPRESSION 1
#define VARIABLE 2
#define VALUE 3

#define BOOLEAN 4
#define NOT 5
#define AND 6
#define OR 7
#define TRUE 8
#define FALSE 9
#define EQUALITY 10

#define ARITHMETIC 11
#define PLUS 12
#define MINUS 13
#define MUL 14

#define STATEMENT 15
#define ASSIGNMENT 16
#define ASSIGN 17
#define SEQUENCE 18
#define SEMICOLON 19
#define SKIP 20
#define CONDITIONAL 21
#define ELSE 22
#define WHILE 23

//-------------------------------------Doubly Linked List---------------------------
struct dll_node
{
    char *key;
    int val;
    struct dll_node *left, *right;
};
struct dll
{
    int size;
    struct dll_node *head, *tail;
};
struct dll_node *getNewDLLNode(char *k, int v);
struct dll *getNewDLL();
struct dll *push_back(struct dll *list, char *key, int val);
struct dll *erase(struct dll *list, struct dll_node *node);
void printLL(struct dll *list);
//----------------------------------------------------------------------------------
//--------------------------------------Hash Table----------------------------------
struct hash_table
{
    struct dll **arr;
};
struct hash_table *getNewHashTable();
int hash_function(char *str);
struct dll_node *search(struct hash_table *map, char *key);
struct hash_table *put(struct hash_table *map, char *key, int val);
struct hash_table *removeKey(struct hash_table *map, char *key);
void printMap(struct hash_table *map);
//----------------------------------------------------------------------------------
//--------------------------------------Abstract Syntax Tree Declarations---------------------------------
struct AST_Node
{
    int typeClass;
    int nodeType;
    char *varname;
    int oper;
    int val;
    char *s;
    struct AST_Node *left, *right;
};

//function declaration for Abstract Syntax Tree
struct AST_Node *makeTreeNode(int typeClass, int nodeType, char *varname, int operator, int val, char *s, struct AST_Node *l, struct AST_Node *r);
//for printing syntax tree
void ASTPrintTree(struct AST_Node *);
//for deleting syntax tree
struct AST_Node *ASTDelete(struct AST_Node *);
//----------------------------------------------------------------------------------------------------------
//-------------------------------Interpreter Functions Declarations-----------------------------------------
struct AST_Node *reduce(struct AST_Node *, int);
struct AST_Node *reduceBooleanExpression(struct AST_Node *);
struct AST_Node *reduceArithmeticExpression(struct AST_Node *);
struct AST_Node *reduceStatement(struct AST_Node *);