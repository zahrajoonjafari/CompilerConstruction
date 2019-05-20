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




    struct mcc_ast_symbol_table *table;
    bool has_main;
    struct mcc_parser_result *st_result;
    struct mcc_parser_result *sem_result;


 struct mcc_ast_symbol_table{

       int counter_sym;
       struct mcc_ast_symbol_table *next;
       struct mcc_ast_symbol_table  *before;
       struct mcc_ast_symbol *symbol;
       struct mcc_ast_error *error;

    };

struct mcc_ast_error{

    const char *err_msg;
    int line_err;
    struct mcc_parser_result *result;


};
struct mcc_ast_symbol{

        char *old_symbol;
        char  *new_symbol;
        int symbol_size;
        int param_count;
        enum mcc_ast_data_type type;
         //struct mcc_ast_identifier *identifier;


    };


enum mcc_ast_data_type get_dt(enum mcc_ast_literal_type type);

void semantic_chk_expression(struct mcc_ast_expression *expr, void *data);

void semantic_chk_call_expr(struct mcc_ast_expression *expression, void *data);

void semantic_chk_assign_stmt_type(struct mcc_ast_statement *stmt,  void *data);

void semantic_chk_if_stmt_type(struct mcc_ast_statement*stmt,  void *data);

void semantic_chk_while_stmt_type(struct mcc_ast_statement*stmt,  void *data);

void semantic_chk_ret_stmt_type(struct mcc_ast_statement *stmt,  void *data);

void semantic_chk_compound_stmt(struct mcc_ast_statement *stmt, void *data);

void semantic_chk_main(struct mcc_ast_program* program, void *data);

void semantic_chk_function_returns(struct mcc_ast_function *fun, void *data);

struct mcc_parser_result *mcc_ast_semantic_chk(struct mcc_parser_result *result);

struct mcc_ast_visitor ast_symtab_visitor(struct mcc_ast_symbol_table *data);


struct mcc_ast_symbol *symtab_insert( struct mcc_ast_symbol_table *st, char *old_symbol,
                                      char *new_symbol,
                                      int type,
                                      int num_params,
                                      int symbol_size);

struct  mcc_ast_symbol_table *build_symtab_node();

struct  mcc_ast_symbol_table *symtab_add_function_name();

void symtab_return_decl(struct mcc_ast_statement *stmt, void *data);

void symbol_table_identifier(struct mcc_ast_identifier *identifier, void *data);

void symtab_del_symbol(struct mcc_ast_symbol *symbol);

void destroy_symtab_node(struct mcc_ast_symbol_table *st);

void symtab_add_decl_stmt(struct mcc_ast_statement *stmt, void *data);

void symtab_expression_decl(struct mcc_ast_expression *expr , void *data);

void add_statement_scope(struct mcc_ast_statement * stmt, void *data);

void del_statement_scope(struct mcc_ast_statement * stmt, void *data);

void symtab_assignment_decl(struct mcc_ast_statement *stmt, void *data);

void  add_function_scope(struct mcc_ast_function *fun, void *data);

void  del_function_scope(struct mcc_ast_function *fun, void *data);

void symtab_call_expr_decl(struct mcc_ast_expression *expr, void *data);

enum mcc_ast_data_type symtab_identifier_type(struct mcc_ast_symbol_table *st, char *id);

struct mcc_ast_symbol_table *create_symtab(struct mcc_ast_program *program);



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



struct mcc_ast_symbol *identifier_lookup(struct mcc_ast_symbol_table *st, char *id){

    assert(st);
    assert(id);

    struct mcc_ast_symbol_table *current  = st;

    for (int i = 0; i < st->counter_sym; i++) {

       // char *current = st->symbol[i].old_symbol;
        if (strcmp(current->symbol[i].old_symbol , id) == 0) {

            return current->symbol;
        }
    }
    return NULL;
}

struct mcc_ast_symbol *symtab_insert( struct mcc_ast_symbol_table *st, char *old_symbol,
                                      char *new_symbol,
                                      int type,
                                      int num_params,
                                      int symbol_size) {
    assert(st);
    assert(old_symbol);


    struct mcc_ast_symbol *sym = malloc(sizeof(sym));

    sym->symbol_size = symbol_size;
    sym->type = type;
    sym->param_count = num_params;
    sym->new_symbol = new_symbol;
    sym->old_symbol = old_symbol;
    sym->old_symbol = realloc(st->symbol, sizeof(struct mcc_ast_symbol)* (st->counter_sym +1));
    memcpy(&(st->symbol[st->counter_sym]), sym, sizeof(*sym));


    st->counter_sym++;

    free(sym);


    return st->symbol;// TODO

}
struct mcc_ast_symbol_table  *symtab_add_function_name(){



    char *print = "print";
    char *print_nl = "print_nl";
    char *print_int = "print_int";
    char *print_float = "print_float";
    char *read_int = "read_int";
    char *read_float = "read_float";
    struct mcc_ast_symbol_table *temp = malloc(sizeof(temp));
    symtab_insert(table, print,print,  MCC_AST_DATA_TYPE_VOID, 1, 0);
    symtab_insert(table, print_nl, print_nl, MCC_AST_DATA_TYPE_VOID, 0 , 0);
    symtab_insert(table, print_int, print_int, MCC_AST_DATA_TYPE_VOID, 1, 0);
    symtab_insert(table, print_float, print_float, MCC_AST_DATA_TYPE_VOID, 1, 0);
    symtab_insert(table, read_int, read_int, MCC_AST_DATA_TYPE_INT,  0, 0);
    symtab_insert(table, read_float, read_float,  MCC_AST_DATA_TYPE_FLOAT, 0, 0);


    return temp;

}



#ifdef __cplusplus
}
#endif

#endif


