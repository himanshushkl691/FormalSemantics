// #include "dataStructures.h"
#include <limits.h>
struct hash_table *map;
//-------------------------------------Doubly Linked List---------------------------
struct dll_node *getNewDLLNode(char *k, int v)
{
    struct dll_node *newn = (struct dll_node *)malloc(sizeof(struct dll_node));
    newn->key = (char *)malloc(strlen(k) * sizeof(char));
    newn->key = strdup(k);
    newn->val = v;
    newn->left = NULL;
    newn->right = NULL;
    return newn;
}
struct dll *getNewDLL()
{
    struct dll *newn = (struct dll *)malloc(sizeof(struct dll));
    newn->head = NULL;
    newn->tail = NULL;
    newn->size = 0;
    return newn;
}
struct dll *push_back(struct dll *list, char *key, int val)
{
    struct dll_node *newn = getNewDLLNode(key, val);
    if (list->size == 0)
    {
        list->head = list->tail = newn;
    }
    else
    {
        list->tail->right = newn;
        newn->left = list->tail;
        list->tail = newn;
    }
    list->size++;
    return list;
}
struct dll *erase(struct dll *list, struct dll_node *node)
{
    if (list->size == 0)
        return list;
    struct dll_node *prev = node->left;
    struct dll_node *next = node->right;
    if (prev != NULL && next != NULL)
    {
        prev->right = next;
        next->left = prev;
        node->left = node->right = NULL;
        free(node);
    }
    else if (prev != NULL)
    {
        prev->right = NULL;
        node->left = NULL;
        list->tail = prev;
        free(node);
    }
    else if (next != NULL)
    {
        next->left = NULL;
        list->head = next;
        node->right = NULL;
        free(node);
    }
    else
    {
        list->head = NULL;
        list->tail = NULL;
        free(node);
    }
    list->size--;
    return list;
}
void printLL(struct dll *list)
{
    struct dll_node *curr = list->head;
    while (curr != NULL)
    {
        printf("[ %s, %d] ", curr->key, curr->val);
        curr = curr->right;
    }
    printf("\n");
    return;
}
//----------------------------------------------------------------------------------
//--------------------------------------Hash Table----------------------------------
struct hash_table *getNewHashTable()
{
    struct hash_table *newn = (struct hash_table *)malloc(sizeof(struct hash_table));
    newn->arr = (struct dll **)malloc(9973 * sizeof(struct dll));
    for (int i = 0; i < 9973; i++)
        newn->arr[i] = getNewDLL();
    return newn;
}
int hash_function(char *str)
{
    int len = strlen(str);
    int res = 0;
    for (int i = len - 1; i >= 0; i--)
    {
        int temp = (53 * res) % 9973;
        res = (temp + (str[i] - 'a')) % 9973;
    }
    return res;
}
struct dll_node *search(struct hash_table *map, char *key)
{
    int hash = hash_function(key);
    struct dll_node *curr = map->arr[hash]->head;
    while (curr != NULL)
    {
        if (strcmp(key, curr->key) == 0)
            return curr;
        curr = curr->right;
    }
    return NULL;
}
struct hash_table *put(struct hash_table *map, char *key, int val)
{
    struct dll_node *res = search(map, key);
    int hash = hash_function(key);
    if (res == NULL)
    {
        map->arr[hash] = push_back(map->arr[hash], key, val);
    }
    else
    {
        res->val = val;
    }
    return map;
}
struct hash_table *removeKey(struct hash_table *map, char *key)
{
    struct dll_node *res = search(map, key);
    if (res == NULL)
        return map;
    else
    {
        int hash = hash_function(key);
        map->arr[hash] = erase(map->arr[hash], res);
    }
    return map;
}
void printMap(struct hash_table *map)
{
    for (int i = 0; i < 9973; i++)
    {
        if (map->arr[i]->size > 0)
        {
            printf("%d---> ", i);
            printLL(map->arr[i]);
        }
    }
    return;
}
//----------------------------------------------------------------------------------
//------------------------------Abstract Syntax Tree Declarations-----------------------------
struct AST_Node *makeTreeNode(int typeClass, int nodeType, char *varname, int operator, int val, char *s, struct AST_Node *l, struct AST_Node *r)
{
    struct AST_Node *newn = (struct AST_Node *)malloc(sizeof(struct AST_Node));
    newn->typeClass = typeClass;
    newn->nodeType = nodeType;
    if (varname != NULL)
    {
        newn->varname = (char *)malloc(strlen(varname) * sizeof(char));
        newn->varname = strdup(varname);
    }
    else
        newn->varname = NULL;
    newn->oper = operator;
    newn->val = val;
    if (s != NULL)
    {
        newn->s = (char *)malloc(strlen(s) * sizeof(char));
        newn->s = strdup(s);
    }
    else
        newn->s = NULL;
    newn->left = l;
    newn->right = r;
    return newn;
}
struct AST_Node *ASTDelete(struct AST_Node *root)
{
    if (root)
    {
        ASTDelete(root->left);
        ASTDelete(root->right);
        root = NULL;
        free(root);
    }
    return NULL;
}
void ASTPrintTree(struct AST_Node *root)
{
    if (root)
    {
        if (root->oper == NOT)
            printf("%s ", root->s);
        ASTPrintTree(root->left);
        if (root->oper != NOT)
            printf("%s ", root->s);
        ASTPrintTree(root->right);
    }
}
//-------------------------------------------------------------------------------------------
//------------------------------------INTERPRETER--------------------------------------------
struct AST_Node *reduce(struct AST_Node *root, int printFlag)
{
    switch (root->typeClass)
    {
    case BOOLEAN:
        if (root->nodeType != VALUE)
        {
            if (printFlag == 1)
            {
                printf("\n=> ");
                ASTPrintTree(root);
            }
            root = reduceBooleanExpression(root);
            return reduce(root, printFlag);
        }
        break;
    case ARITHMETIC:
        if (root->nodeType != VALUE)
        {
            if (printFlag == 1)
            {
                printf("\n=> ");
                ASTPrintTree(root);
            }
            root = reduceArithmeticExpression(root);
            return reduce(root, printFlag);
        }
        break;
    case STATEMENT:
        if (root->nodeType != NONE)
        {
            if (printFlag == 1)
            {
                printf("\n=> ");
                ASTPrintTree(root);
                printf("\n");
                printMap(map);
            }
            root = reduceStatement(root);
            return reduce(root, printFlag);
        }
        break;
    default:
        break;
    }
    if (printFlag == 1)
    {
        if (root->typeClass != NONE)
        {
            printf("\n=> ");
            ASTPrintTree(root);
            printf("\n\n");
        }
        else
        {
            printf("\n");
            printMap(map);
            printf("\n");
        }
    }
    return root;
}
struct AST_Node *reduceBooleanExpression(struct AST_Node *root)
{
    switch (root->oper)
    {
    case AND:
        if (root->left->nodeType == VALUE)
        {
            struct AST_Node *curr;
            if (root->left->val == TRUE)
            {
                curr = root->right;
                root->right = NULL;
                ASTDelete(root);
                return curr;
            }
            else
            {
                curr = root->left;
                root->left = NULL;
                ASTDelete(root);
                return curr;
            }
        }
        else if (root->left->nodeType == EXPRESSION)
        {
            switch (root->left->typeClass)
            {
            case BOOLEAN:
                root->left = reduceBooleanExpression(root->left);
                break;
            default:
                break;
            }
            return root;
        }
        break;
    case OR:
        if (root->left->nodeType == VALUE)
        {
            struct AST_Node *curr;
            if (root->left->val == TRUE)
            {
                curr = root->left;
                root->left = NULL;
                ASTDelete(root);
                return curr;
            }
            else
            {
                curr = root->right;
                root->right = NULL;
                ASTDelete(root);
                return curr;
            }
        }
        else if (root->left->nodeType == EXPRESSION)
        {
            switch (root->left->typeClass)
            {
            case BOOLEAN:
                root->left = reduceBooleanExpression(root->left);
                break;
            default:
                break;
            }
            return root;
        }
        break;
    case NOT:
        if (root->left->nodeType == VALUE)
        {
            struct AST_Node *res;
            if (root->left->val == TRUE)
            {
                ASTDelete(root);
                res = makeTreeNode(BOOLEAN, VALUE, NULL, NONE, FALSE, "false", NULL, NULL);
                return res;
            }
            else
            {
                ASTDelete(root);
                res = makeTreeNode(BOOLEAN, VALUE, NULL, NONE, TRUE, "true", NULL, NULL);
                return res;
            }
        }
        else if (root->left->nodeType == EXPRESSION)
        {
            switch (root->left->typeClass)
            {
            case BOOLEAN:
                root->left = reduceBooleanExpression(root->left);
                break;
            default:
                break;
            }
            return root;
        }
        break;
    case EQUALITY:
        if (root->left->nodeType == VALUE)
        {
            struct AST_Node *curr;
            if (root->right->nodeType == VALUE)
            {
                int a = root->left->val;
                int b = root->right->val;
                if (a == b)
                    curr = makeTreeNode(BOOLEAN, VALUE, NULL, NONE, TRUE, "true", NULL, NULL);
                else
                    curr = makeTreeNode(BOOLEAN, VALUE, NULL, NONE, FALSE, "false", NULL, NULL);
                return curr;
            }
            else if (root->right->typeClass == ARITHMETIC)
            {
                root->right = reduceArithmeticExpression(root->right);
                return root;
            }
            else
            {
                struct AST_Node *curr;
                curr = makeTreeNode(NONE, NONE, NULL, NONE, NONE, NULL, NULL, NULL);
                return curr;
            }
        }
        else if (root->left->typeClass == ARITHMETIC)
        {
            root->left = reduceArithmeticExpression(root->left);
            return root;
        }
        else
        {
            struct AST_Node *curr;
            curr = makeTreeNode(NONE, NONE, NULL, NONE, NONE, NULL, NULL, NULL);
            return curr;
        }
        break;
    default:
        printf("nodeType not in Boolean expressions\n");
        break;
    }
    return root;
}

struct AST_Node *reduceArithmeticExpression(struct AST_Node *root)
{
    if (root->nodeType == VARIABLE)
    {
        struct dll_node *res = search(map, root->varname);
        struct AST_Node *curr;
        if (res == NULL)
        {
            printf("\nUsing variable before assigning\n");
            curr = makeTreeNode(NONE, NONE, NULL, NONE, NONE, NULL, NULL, NULL);
            ASTDelete(root);
            exit(1);
        }
        else
        {
            char *news[(((sizeof res) * CHAR_BIT) + 2) / 3 + 2];
            sprintf(news, "%d", res->val);
            curr = makeTreeNode(ARITHMETIC, VALUE, NULL, NONE, res->val, news, NULL, NULL);
            ASTDelete(root);
            return curr;
        }
    }
    else
    {
        switch (root->oper)
        {
        case PLUS:
            if (root->left->nodeType == VALUE)
            {
                struct AST_Node *curr;
                if (root->right->nodeType == VALUE)
                {
                    int a = root->left->val;
                    int b = root->right->val;
                    int res = a + b;
                    char *news[(((sizeof res) * CHAR_BIT) + 2) / 3 + 2];
                    sprintf(news, "%d", res);
                    curr = makeTreeNode(ARITHMETIC, VALUE, NULL, NONE, res, news, NULL, NULL);
                    ASTDelete(root);
                    return curr;
                }
                else
                {
                    root->right = reduceArithmeticExpression(root->right);
                    return root;
                }
            }
            else
            {
                root->left = reduceArithmeticExpression(root->left);
                return root;
            }
            break;
        case MINUS:
            if (root->left->nodeType == VALUE)
            {
                struct AST_Node *curr;
                if (root->right->nodeType == VALUE)
                {
                    int a = root->left->val;
                    int b = root->right->val;
                    int res = a - b;
                    char *news[(((sizeof res) * CHAR_BIT) + 2) / 3 + 2];
                    sprintf(news, "%d", res);
                    curr = makeTreeNode(ARITHMETIC, VALUE, NULL, NONE, res, news, NULL, NULL);
                    ASTDelete(root);
                    return curr;
                }
                else
                {
                    root->right = reduceArithmeticExpression(root->right);
                    return root;
                }
            }
            else
            {
                root->left = reduceArithmeticExpression(root->left);
                return root;
            }
            break;
        case MUL:
            if (root->left->nodeType == VALUE)
            {
                struct AST_Node *curr;
                if (root->right->nodeType == VALUE)
                {
                    int a = root->left->val;
                    int b = root->right->val;
                    int res = a * b;
                    char *news[(((sizeof res) * CHAR_BIT) + 2) / 3 + 2];
                    sprintf(news, "%d", res);
                    curr = makeTreeNode(ARITHMETIC, VALUE, NULL, NONE, res, news, NULL, NULL);
                    ASTDelete(root);
                    return curr;
                }
                else
                {
                    root->right = reduceArithmeticExpression(root->right);
                    return root;
                }
            }
            else
            {
                root->left = reduceArithmeticExpression(root->left);
                return root;
            }
            break;
        default:
            printf("nodeType not in Arithmetic expressions\n");
            break;
        }
        return root;
    }
}
struct AST_Node *reduceStatement(struct AST_Node *root)
{
    switch (root->oper)
    {
    case ASSIGN:
        if (root->right->nodeType == VALUE)
        {
            struct AST_Node *curr;
            map = put(map, root->left->varname, root->right->val);
            curr = makeTreeNode(NONE, NONE, NULL, NONE, NONE, NULL, NULL, NULL);
            ASTDelete(root);
            return curr;
        }
        else
        {
            root->right = reduceArithmeticExpression(root->right);
            return root;
        }
        break;
    case SEMICOLON:
        if (root->left->typeClass == NONE)
        {
            struct AST_Node *curr;
            curr = root->right;
            root->right = NULL;
            ASTDelete(root);
            return curr;
        }
        else
        {
            root->left = reduceStatement(root->left);
            if (root->left->typeClass == NONE)
            {
                struct AST_Node *curr;
                curr = root->right;
                root->right = NULL;
                ASTDelete(root);
                return curr;
            }
            return root;
        }
        break;
    default:
        printf("nodeType not in Statements\n");
        break;
    }
    return root;
}