#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include "mcc/ast.h"
#include "mcc/ast_print.h"
#include "mcc/parser.h"

void print_usage(const char *prg) {
    printf("usage: %s <FILE>\n\n", prg);
    printf("  <FILE>  Input filepath or - for stdin\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    // determine input source
    FILE *in;
    if (strcmp("-", argv[1]) == 0) {
        in = stdin;
    } else {
        in = fopen(argv[1], "r");
        if (!in) {
            perror("fopen");
            return EXIT_FAILURE;
        }
    }

    struct mcc_ast_program *program = NULL;

    // parsing phase
    {
        printf("Start parsing \n");

        struct mcc_parser_result result = mcc_parse_file(in, stderr);
        if (result.status != MCC_PARSER_STATUS_OK) {
            // print error message
            printf("%s", result.parser_error -> error_msg);
           // bool abort = result.status !=MCC_PARSER_STATUS_OK;




            return EXIT_FAILURE;
        }
        printf("Parsing ok \n");
        printf("---- Printing tree ----\n");
        program = result.program;
    }
    if (strcmp("mcc_ast_to_dot", basename(argv[0])) == 0)
    {
        mcc_ast_print_dot(stdout, program);
    }


    // cleanup
    mcc_ast_delete_program(program);

    // TODO free result

    return EXIT_SUCCESS;
}