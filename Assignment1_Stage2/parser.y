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
%type <node> Boolean _TRUE _FALSE
/*TOKENS*/
//operator
%token _AND _OR _NOT    _TRUE   _FALSE
//Associativity
%left   _OR
%left   _AND
%left   _NOT
/*STARTING Non-Terminal*/
%start Program

%%
//---------------------------Rules-------------------------------------
Program : Boolean                       {
                                            reduce($1);
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
%%
//-------------------------Auxiliary Functions--------------------------
void yyerror(const char *err){
    printf("%s\n",err);
    return;
}

int main(int argc,char *argv[]){
    while(1)
    {
        printf("> ");
        yyparse();
    }
    return 1;
}
