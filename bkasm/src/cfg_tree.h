/*  Program grammar
<program> ::= <statement>
<statement> ::= { <space> } <semicolon> {...} |
                { <space> } <label> |
                { <space> } <label> <semicolon> {...} |
                { <space> } <label> { <space> } <keyword> <space> { <space> } <operands> { <space> } <semicolon> {...} |
                { <space> } <keyword> <space> { <space> } <operands> { <space> } <semicolon> {...} |
                { <space> } <keyword> <space> { <space> } <operands> |
                { <statment> }
<operands> ::= <lvalue> <comma> <rvalue> | <lvalue>
<lvalue>   ::= <reg> | <expr>
<rvalue>   ::= <reg> | <expr>
<expr>     ::= <int>
<label>    ::= <id> <colon>
<id>       ::= <alfa> { <alfa> }
<alfa>     ::= "a" | "b" | ... | "z"
<int>      ::= <digit> { <digit> }
<digit>    ::= "0" | "1" | ... | "9"
<colon>    ::= ":"
<semicolon>::= ";"
TODO define it
<term>     ::= <id> | <int> | <paren-expr>
*/
#ifndef H_CFG_TREE_H
#define H_CFG_TREE_H

#include <globals.h>
#include <parser.h>
#include <nodes.h>

STRUCT(CFG_Tree)
{
    Node node;
    CFG_Tree* next;
};

STRUCT(Program)
{
    CFG_Tree *first;
    CFG_Tree *last;
};

void cfg_tree_add_statement(Node *statement, Program *program);
Program* program_create();
void program_init(Program* prg);
void program_free(Program* prg);
void program_destroy(Program* prg);
void destroyCFGTree(CFG_Tree* tree);
#endif
