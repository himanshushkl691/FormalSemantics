#!/bin/bash
yacc -d parser.y --warning=none
lex lexicalAnalyzer.l
gcc lex.yy.c y.tab.c -o interpreter.exe -w
