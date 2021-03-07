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
struct AST_Node *makeTreeNode(int, int, char *, int, int, char *, struct AST_Node *, struct AST_Node *);
//for printing syntax tree
void ASTPrintTree(struct AST_Node *);
//for deleting syntax tree
struct AST_Node *ASTDelete(struct AST_Node *);
//----------------------------------------------------------------------------------------------------------
//-------------------------------Interpreter Functions Declarations-----------------------------------------
void reduce(struct AST_Node *);
struct AST_Node *reduceBooleanExpression(struct AST_Node *);