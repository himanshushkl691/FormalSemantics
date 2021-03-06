//----------------------Auxiliary Declaration-----------------
%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "dataStructures.h"
    #include "interpreter.cpp"
    int yylex(void);
    char *yytext;
    FILE *ft;
%}

%union{
    struct AST_Node *node;
};

/**/
%type <node> Boolean    Arithmetic  Statement   _TRUE _FALSE   _NUMERIC_VALUE  _VARIABLE    _SKIP
/*TOKENS*/
//operator
%token  _AND _OR _NOT    _TRUE   _FALSE  _NUMERIC_VALUE  _PLUS   _MINUS  _MUL    _EQUALITY
%token  _VARIABLE   _ASSIGN _IF _THEN   _ELSE   _WHILE  _DO _SKIP   _STORE
//Associativity
%left   _OR
%left   _AND
%left   _NOT
%left   _EQUALITY
%left   _PLUS _MINUS
%left   _MUL
/*STARTING Non-Terminal*/
%start Program

%%
//---------------------------Rules-------------------------------------
Program :   '\n'                        {return;}
|   _STORE   '\n'                       {printMap(map);printf("\n\n");return;}
|   Boolean '\n'                        {
                                            $1 = reduce($1,1);
                                            return;
                                        }
|   Arithmetic  '\n'                    {
                                            $1 = reduce($1,0);
                                            return;
                                        }
|   Statement   '\n'                    {
                                            $1 = reduce($1,0);
                                            return;
                                        }
;
Boolean : _TRUE                         {
                                            $$ = $1;
                                        }
|   _FALSE                              {
                                            $$ = $1;
                                        }
|   Boolean _AND Boolean                {
                                            $$ = makeTreeNode(BOOLEAN,EXPRESSION,NULL,AND,NONE,"&&",$1,$3);
                                        }
|   Boolean _OR Boolean                 {
                                            $$ = makeTreeNode(BOOLEAN,EXPRESSION,NULL,OR,NONE,"||",$1,$3);
                                        }
|   _NOT Boolean                        {
                                            $$ = makeTreeNode(BOOLEAN,EXPRESSION,NULL,NOT,NONE,"!",$2,NULL);
                                        }
|   '(' Boolean ')'                     {
                                            $$ = $2;
                                        }
|   Arithmetic _EQUALITY    Arithmetic  {
                                            $$ = makeTreeNode(BOOLEAN,EXPRESSION,NULL,EQUALITY,NONE,"==",$1,$3);
                                        }
;
Arithmetic  :   _NUMERIC_VALUE          {
                                            $$ = $1;
                                        }
|   Arithmetic _PLUS    Arithmetic      {
                                            $$ = makeTreeNode(ARITHMETIC,EXPRESSION,NULL,PLUS,NONE,"+",$1,$3);
                                        }
|   Arithmetic  _MINUS  Arithmetic      {
                                            $$ = makeTreeNode(ARITHMETIC,EXPRESSION,NULL,MINUS,NONE,"-",$1,$3);
                                        }
|   Arithmetic  _MUL    Arithmetic      {
                                            $$ = makeTreeNode(ARITHMETIC,EXPRESSION,NULL,MUL,NONE,"*",$1,$3);
                                        }
|   _VARIABLE                           {
                                            $$ = $1;
                                        }
|   '(' Arithmetic  ')'                 {
                                            $$ = $2;
                                        }
;
Statement   :   Statement ';' Statement {
                                            $$ = makeTreeNode(STATEMENT,SEQUENCE,NULL,SEMICOLON,NONE,";",$1,$3);
                                        }
|   _VARIABLE   _ASSIGN Arithmetic      {
                                            $$ = makeTreeNode(STATEMENT,ASSIGNMENT,NULL,ASSIGN,NONE,":=",$1,$3);
                                        }
|   _SKIP                               {
                                            $$ = $1;
                                        }
|   _IF '('Boolean')' _THEN '{'Statement'}' _ELSE '{'Statement'}'{
                                                                    struct AST_Node *temp = makeTreeNode(STATEMENT,ELSE,NULL,NONE,NONE,"else",$3,$11);
                                                                    $$ = makeTreeNode(STATEMENT,CONDITIONAL,NULL,NONE,NONE,"if",$7,temp);
                                                                }
|   _WHILE  '(' Boolean ')' _DO '{' Statement   '}'             {
                                                                    $$ = makeTreeNode(STATEMENT,WHILE,NULL,NONE,NONE,"while",$3,$7);
                                                                }
;
%%
//-------------------------Auxiliary Functions--------------------------
void yyerror(const char *err){
    fprintf(stderr,"%s\n\n",err);
    exit(1);
}

int main(int argc,char *argv[]){
    map = getNewHashTable();
    while(1)
    {
        printf("> ");
        yyparse();
    }
    return 1;
}
