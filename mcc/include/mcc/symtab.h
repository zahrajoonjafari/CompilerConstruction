#ifndef SYMTAB_H
#define SYMTAB_H

#ifdef __cplusplus
extern "C" {
#else
#include <stdbool.h>
#endif

#include <assert.h>
#include "mcc/ast.h"
#include "mcc/error.h"
#include <mcc/ast_visit.h>
#include <mcc/parser.h>    




 struct mcc_ast_symbol_table{

       int counter_sym;
       struct mcc_ast_symbol_table *next;
       struct mcc_ast_symbol_table  *before;
       struct mcc_ast_symbol *symbol;
       struct mcc_ast_error *error;

    };


struct mcc_ast_symbol{

        char *elem;
        int symbol_size;
        int param_count;
        enum mcc_ast_data_type type;
         //struct mcc_ast_identifier *identifier;


    };


struct mcc_ast_curr_function{

    struct mcc_ast_curr_function *before;
    struct mcc_ast_curr_function *next;
    struct mcc_ast_identifier *identifier;

    enum  mcc_ast_data_type  type;


};

const char* print_type(int type) {
    switch (type) {

        case MCC_AST_DATA_TYPE_BOOL:
            return "boolean";
        case MCC_AST_DATA_TYPE_INT:
            return "integer";
        case MCC_AST_DATA_TYPE_FLOAT:
            return "floating point";
        case MCC_AST_DATA_TYPE_STRING:
            return "string";
        case MCC_AST_DATA_TYPE_VOID:
            return "void";
        default:
            return "unknown";
    }
}



struct mcc_ast_symbol_table *table;
bool has_main;
bool exist_error;
struct mcc_parser_result *st_result;
struct mcc_parser_result *sem_result;
struct mcc_parser_result *mcc_ast_semantic_chk(struct mcc_parser_result *result);

//-------------------------------------------------------------------------




#ifdef __cplusplus
}
#endif

#endif


