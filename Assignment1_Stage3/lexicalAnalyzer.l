%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "dataStructures.h"
    #include "y.tab.h"
    int number;
%}

%%
true    {yylval.node = makeTreeNode(BOOLEAN,VALUE,NULL,NONE,TRUE,yytext,NULL,NULL);return _TRUE;}
false   {yylval.node = makeTreeNode(BOOLEAN,VALUE,NULL,NONE,FALSE,yytext,NULL,NULL);return _FALSE;}
"&&"    {return _AND;}
"||"    {return _OR;}
"!"     {return _NOT;}
"("     {return *yytext;}
")" 	{return *yytext;}
"+"     {return _PLUS;}
"-"     {return _MINUS;}
"*"     {return _MUL;}
"=="    {return _EQUALITY;}
([-]?(0|[1-9][0-9]*))  {
                            int sign = 1;
                            if(isdigit(*yytext) == 1)
                            {
                                number = atoi(yytext);
                            }
                            else
                            {
                                if(*yytext == '-')
                                    sign = -1;
                                number = sign * atoi(yytext + 1);
                            }
                            yylval.node = makeTreeNode(ARITHMETIC,VALUE,NULL,NONE,number,yytext,NULL,NULL);
                            return _NUMERIC_VALUE;
                        }
[ \t]   {}
[\n]    {return *yytext;}
.       {
            fprintf(stderr,"syntax error\n\n");
            exit(1);
        }
%%
int yywrap()
{
    return 1;
}