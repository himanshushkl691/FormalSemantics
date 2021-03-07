%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "dataStructures.h"
    #include "y.tab.h"
%}

%%
true    {yylval.node = makeTreeNode(BOOLEAN,VALUE,NULL,NONE,TRUE,yytext,NULL,NULL);return _TRUE;}
false   {yylval.node = makeTreeNode(BOOLEAN,VALUE,NULL,NONE,FALSE,yytext,NULL,NULL);return _FALSE;}
"&&"    {return _AND;}
"||"    {return _OR;}
"!"     {return _NOT;}
"("     {return *yytext;}
")" 	{return *yytext;}
[ \t]   {}
[\n]    {return NULL;}
.       {yytext = NULL;}
%%
int yywrap()
{
    return 1;
}