#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include "mcc/ast.h"
#include "mcc/ast_print.h"
#include "mcc/parser.h"



void print_usage(const char *prg)
{
    printf("usage: %s <FILE>\n\n", prg);
    printf("  <FILE>        Input filepath or - for stdin\n");
}


void error_compiler(const char *msg, FILE *error, struct mcc_parser_result* result)

{
    fprintf(error, "%s", msg);

    for (int i = 0; i < result->errors[i].count; i++) {
        fprintf(error, "%s\n", result->errors->error[i].err_msg);
    }

}
int main(int argc, char *argv[]){

     FILE *in = stdin;
     FILE *error = stderr;

    if (argc < 2) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    // determine input source

    if (strcmp("-", argv[1]) == 0) {
        in = stdin;
    } else {
        in = fopen(argv[1], "r");
        if (!in) {
            perror("fopen");
            return EXIT_FAILURE;
        }
    }




    /* parsing phase */

    struct mcc_parser_result result = mcc_parse_file(in);
    fclose(in);
    if(result.status == MCC_PARSER_STATUS_UNKNOWN_ERROR){
        error_compiler("parsing_error\n", error, &result);

    }


    // TODO:
    // - run semantic checks
    // - create three-address code
    // - output assembly code
    // - invoke backend compiler

    // cleanup
    mcc_delete_result(&result);

    return EXIT_SUCCESS;
}