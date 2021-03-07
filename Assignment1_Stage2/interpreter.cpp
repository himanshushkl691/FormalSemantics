// #include "dataStructures.h"
//------------------------------Abstract Syntax Tree Declrations-----------------------------
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
    newn->s = (char *)malloc(strlen(s) * sizeof(char));
    newn->s = strdup(s);
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
void reduce(struct AST_Node *root)
{
    switch (root->typeClass)
    {
    case BOOLEAN:
        if (root->nodeType != VALUE)
        {
            printf("\n=> ");
            ASTPrintTree(root);
            root = reduceBooleanExpression(root);
            reduce(root);
        }
        else
        {
            printf("\n=> ");
            ASTPrintTree(root);
            printf("\n\n");
            ASTDelete(root);
        }
        break;
    default:
        break;
    }
    return;
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
            root->left = reduceBooleanExpression(root->left);
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
            root->left = reduceBooleanExpression(root->left);
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
            root->left = reduceBooleanExpression(root->left);
            return root;
        }
        break;
    default:
        printf("nodeType not in Boolean expressions\n");
        return root;
        break;
    }
}