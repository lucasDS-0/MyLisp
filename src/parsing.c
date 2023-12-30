#include "mpc.h"

/* If we are compiling on Windows compile these functions */
#ifdef _WIN32
#include <string.h>

static char buffer[2048];

/* Fake readline function */
char* readline(char* prompt) {
    fputs(prompt, stdout);
    fgets(buffer, 2048, stdin);
    char* cpy = malloc(strlen(buffer)+1);
    strcpy(cpy, buffer);
    cpy[strlen(cpy)-1] = '\0';
    return cpy;
}

/* Fake add_history function */
void add_history(char* unused) {}

/* Otherwise include the editline headers */
#else
#include <editline/readline.h>
#include <editline/history.h>
#endif

int main(int argc, char** argv) {

    /* Create Some Parser */
    mpc_parser_t* Number = mpc_new("number");
    mpc_parser_t* Operator = mpc_new("operator");
    mpc_parser_t* Expr = mpc_new("expr");
    mpc_parser_t* Lips = mpc_new("lips");

    /* Define the with the following Language */
    mpca_lang(MPCA_LANG_DEFAULT,
    "                                                      \
        number   : /-?[0-9]+(.[0-9])?/ ;                            \
        operator : '-' | '+' | '*' | '/' | '%' ;                 \
        expr     : <number> | '(' <operator> <expr>+ ')' ; \
        lips     : /^/ <operator> <expr>+ /$/ ;            \
    ",
    Number, Operator, Expr, Lips);

    puts("Lips Version 0.0.0.0.2");
    puts("Press Ctrl+c to Exit this program\n");
    
    while (1) {

        /* Now in either case readline will be correctly defined */
        char* input = readline("lips> ");
        add_history(input);
                
        //printf("Indeed, %s\n", input);
        /* Attempt to Parse the user Input */
        mpc_result_t r;
        if (mpc_parse("<stdin>", input, Lips, &r)) {
            /* On Success Print the AST */
            mpc_ast_print(r.output);
            mpc_ast_delete(r.output);
        } else {
            /* Otherwise Print the Error */
            mpc_err_print(r.error);
            mpc_err_delete(r.error);
        }
        free(input);
        
    }
    
    /* Udefine and Delete our Parsers */
    mpc_cleanup(4, Number, Operator, Expr, Lips);
    
    return 0;
}

