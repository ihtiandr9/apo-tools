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
<lvalue> ::= <reg> | <expr>
<rvalue> ::= <reg> | <expr>
<expr> ::= <int>
<label> ::= <id> <colon> 
<id> ::= <alfa> { <alfa> }
<alfa>   ::= "a" | "b" | ... | "z"
<int>  ::= <digit> { <digit> }
<digit> ::= "0" | "1" | ... | "9" 
<colon> ::= ":"
<semicolon> ::= ";"
TODO define it
<term> ::= <id> | <int> | <paren-expr>
*/