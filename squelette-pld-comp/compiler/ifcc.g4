grammar ifcc;

axiom : prog EOF ;

prog : 'int' 'main' '(' ')' '{' statement* '}' ;

statement : return_stmt ';'
        | declaration_stmt ';'
        | assignation_stmt ';'
        | expression ';'
        ;

return_stmt: RETURN expression
            ;
declaration_stmt : TYPE VAR 
                | TYPE VAR '=' expression 
                ;
assignation_stmt : VAR '=' expression 
                ;

expression : VAR                                    # variable
            | CONST                                 # const
            | expression MULT_OPERATOR expression   # mult_expression
            | expression ADD_OPERATOR expression    # add_expression
            ;

ADD_OPERATOR : '+'
        | '-' 
        ;

MULT_OPERATOR : '*'
        | '/' 
        ;

RETURN : 'return' ;
CONST : '-'? [0-9]+ ;
COMMENT : '/*' .*? '*/' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN);
TYPE : 'int' ;
VAR : [a-zA-Z0-9]+ ;