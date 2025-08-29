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
#ifndef H_ASTREE_H
#define H_ASTREE_H

#include <bkasm.h>
#include <nodes.h>


STRUCT(ASTree)  /* Abstract syntax tree */
{
    NodeList *firstNode;
    NodeList *lastNode;
};

void ast_add_statement(Node *statement, ASTree *astree);
ASTree* ast_create();
void ast_init(ASTree* astree);
void ast_free(ASTree* astree);
void ast_destroy(ASTree* astree);
#endif
