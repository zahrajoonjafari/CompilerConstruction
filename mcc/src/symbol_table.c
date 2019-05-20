

#include <assert.h>
#include <stdlib.h>

#include <zlib.h>

#include "mcc/symtab.h"
#include "mcc/error.h"
#include <mcc/ast.h>
#include <mcc/ast_visit.h>







//----------------------------------------------------------------------------------Definition

struct mcc_ast_symbol_table *create_symtab(struct mcc_ast_program *program){

    assert(program);


       struct mcc_ast_visitor pre;
         memset(&pre, 0, sizeof(pre));

       struct mcc_ast_symbol_table *root = build_symtab_node();
               pre.userdata = &root;

                     pre.traversal = MCC_AST_VISIT_DEPTH_FIRST,
                     pre.order = MCC_AST_VISIT_POST_ORDER,
                     pre.identifier = symbol_table_identifier,
                     pre.statement_declaration = symtab_add_decl_stmt,
                     pre.expression_identifier = symtab_expression_decl,
                     pre.expression_call = symtab_call_expr_decl,
                     pre.statement_assignment = symtab_assignment_decl,
                     pre.statement_ret = symtab_return_decl,
                     pre.statement_compound = add_statement_scope,
                     pre.close_statement_compound = del_statement_scope,
                     pre.function_def = add_function_scope,
                     pre.close_function_def = del_function_scope;
                     mcc_ast_visit_program(program, &pre);

                     return root;

}

struct  mcc_ast_symbol_table *build_symtab_node(){



    table = malloc(sizeof(struct mcc_ast_symbol_table));
    table->next = NULL;
    table->before = NULL;
    table->counter_sym = 0;
    table->symbol = NULL;
    return table;

}



void symtab_return_decl(struct mcc_ast_statement *stmt, void *data)

{
    assert(stmt);
    assert(data);

    struct mcc_ast_symbol_table *current = table;
    while(current-> before != NULL){

        current = current-> before;
    }

    stmt->ret_symbol = current->symbol;
    if(stmt->ret_expr == NULL && stmt->ret_symbol->type != MCC_AST_DATA_TYPE_VOID){

        char err_msg[1024] = { 0 };

        printf(err_msg, sizeof(err_msg),ERROR_NO_RETURN);

        mcc_error_list(err_msg, stmt->node.sloc.start_line , st_result, stmt->ret_symbol->old_symbol);

    }

}





enum mcc_ast_data_type symtab_identifier_type(struct mcc_ast_symbol_table *st, char *id)
{
    assert(st);
    assert(id);

    while (st->symbol != NULL) {
        for (int i = 0; i < st->counter_sym; i++) {

            if (strcmp(st->symbol[i].old_symbol, id) == 0 ) {

                return st->symbol[i].type;
            }
        }
        st = st->before;
    }
    return MCC_AST_DATA_TYPE_VOID;
}


void symbol_table_identifier(struct mcc_ast_identifier *identifier, void *data)

{
    assert(identifier);
    assert(data);

    enum mcc_ast_data_type type = symtab_identifier_type(table, identifier->value);
    identifier->data_type = type;
}


void symtab_del_symbol(struct mcc_ast_symbol *symbol) {

    assert(symbol);

    if(symbol->param_count <= 1){

        free(symbol);

    } else{

        symbol->param_count--;
    }

}



void destroy_symtab_node(struct mcc_ast_symbol_table *st){


    while (st->before != NULL)
        st = st-> before;

    struct mcc_ast_symbol_table *current = st;
    while (current != NULL) {
        struct mcc_ast_symbol_table *next = current->next;
        if (current->symbol != NULL) {
            free(current->symbol);
        }
        free(current);
        current = next;
    }
}





void symtab_add_decl_stmt(struct mcc_ast_statement *stmt, void *data)
{

    assert(stmt);
    assert(data);

    struct mcc_ast_identifier * identifier = stmt->declaration->ident;

    while (table != NULL){

    for(int i = 0; i < table->counter_sym; i++)

    if(strcmp(table->symbol[i].old_symbol, identifier->value) == 0){

        stmt->declaration->decl_symbol = symtab_insert(table, identifier->value, identifier->value,
                                                       MCC_AST_STATEMENT_TYPE_DECL ,
                                                       0, stmt->declaration->decl_arr_size);


        stmt->declaration->decl_symbol->param_count++;

       }

    }
}




void symtab_expression_decl(struct mcc_ast_expression *expr , void *data){

    assert(expr);
    assert(data);

    char *id = expr->identifier->value;
    struct mcc_ast_symbol *sym = identifier_lookup(table, id);

    struct mcc_ast_symbol_table *current = table;

    while(sym == NULL && current->before != NULL){

        current = current->before;
        expr->ident_symbol = sym;
        expr->ident_symbol->param_count++;

    }
    if (sym == NULL)
    {
       char err_msg[1024] = {0};

        printf(err_msg, sizeof(err_msg), ERROR_ID_UNDEF, expr->identifier->value);

        mcc_error_list(err_msg, expr->node.sloc.start_line, sem_result);

    }
}

int arg_num_lookup(struct mcc_ast_symbol_table *st , char *id){


    assert(st);
    assert(id);


    while(st != NULL){

        for(int i = 0; i < st->counter_sym; i++)

            if(strcmp(st->symbol[i].old_symbol, id ) == 0){


                return st->symbol[i].param_count;
            }

    }




}
void symtab_call_expr_decl(struct mcc_ast_expression *expr, void *data){

    assert(expr);
    assert(data);

    int num_arg = arg_num_lookup(table, expr->identifier_call->value);

    if(expr->arguments != NULL){

      if(expr->arguments->argument_count != num_arg){

          char err_msg[1024] =  {0};

          printf(err_msg, sizeof(err_msg), ERROR_ARG_NUM, expr->identifier_call->value,
                                           num_arg, expr->arguments->argument_count);

          mcc_error_list(err_msg, expr->identifier_call->node.sloc.start_line, st_result);

      }

    }

}

void add_statement_scope(struct mcc_ast_statement * stmt, void *data){

    assert(stmt);
    assert(data);


    struct mcc_ast_symbol_table *tab = table;
    table ->next = tab;
    tab-> before = table;

    table = tab;

}

void del_statement_scope(struct mcc_ast_statement * stmt, void *data){

    assert(stmt);
    assert(data);

    if (table->before != NULL) {
        struct mcc_ast_symbol_table *temp = table;
        table = table->before;
        temp->before = NULL;
        destroy_symtab_node(temp);
        table->next = NULL;
    }
}



void symtab_assignment_decl(struct mcc_ast_statement *stmt, void *data){

    assert(stmt);
    assert(data);

    char *identifier = stmt->assignment->identifier->value;
    struct mcc_ast_symbol *symbol = identifier_lookup(table, identifier);

    if(symbol != NULL){

        stmt->assignment-> assign_symbol = symbol;
        stmt->assignment-> assign_symbol ->param_count ++;


    } else {

        char err_msg[1024] = {0};

        printf(err_msg, sizeof(err_msg), ERROR_ID_UNDEF);

        mcc_error_list(err_msg, stmt->assignment->node.sloc.start_line, sem_result, identifier);

    }

    if (stmt->assignment->type == MCC_AST_EXPRESSION_TYPE_IDENTIFIER ){


        if (symbol != NULL  ){


            if (stmt->assignment->array_assign.assign_expr == NULL){

                char err_msg[1024] = {0};

                printf(err_msg, sizeof(err_msg), ERROR_ASSIGN_TYPE_MIS);

                mcc_error_list(err_msg, stmt->assignment->node.sloc.start_line, sem_result);
            }
        }
    }

}




void  add_function_scope(struct mcc_ast_function *fun, void *data) {

    struct mcc_ast_parameter *parameter = fun->function_def->parameter;

    char *identifier = fun->function_def->identifier->value;

    if (identifier_lookup(table, identifier) == NULL) {

        symtab_insert(table, fun->function_def->identifier->value, NULL,
                      fun->function_def->data_type, parameter->param_len, -1);

        struct mcc_ast_symbol *symbol = identifier_lookup(table, identifier);

        for (int i = 0; i < parameter->param_len; i++) {

            symbol->param_count = parameter->decl_parameter[i]->type;
        }
    } else {


        char err_msg[1024] = {0};

        printf(err_msg, sizeof(err_msg), ERROR_FUNC_DUBL);

        mcc_error_list(err_msg, fun->node.sloc.start_line, sem_result, identifier);
    }

    struct mcc_ast_symbol_table *tab_new = malloc(sizeof(*tab_new));

    tab_new->before = *(struct mcc_ast_symbol_table **) data;
    
    tab_new->counter_sym = 0;

    table = tab_new;


    for (int i = 0; i < parameter->param_len; i++) {

       char *id = parameter->decl_parameter[i]->declaration->ident->value;

        if (identifier_lookup(tab_new, id) == NULL) {

            parameter->decl_parameter[i]->declaration->decl_symbol =
                    symtab_insert(tab_new, id, NULL, parameter->decl_parameter[i]->declaration[i].decl_lit->type,
                                  parameter->decl_parameter[i]->declaration->decl_arr_size, 0);

            parameter->decl_parameter[i]->declaration->decl_symbol->param_count++;
        }
    }

}



void del_function_scope(struct mcc_ast_function *fun, void *data)

{
    assert(fun);
    assert(data);

    if (table->before != NULL) {
        struct mcc_ast_symbol_table *temp = table;
        table = table->before;
        temp->before = NULL;
        destroy_symtab_node(temp);
        table->next = NULL;
    }

  }

struct mcc_parser_result *symbol_table(struct mcc_parser_result *result){

    assert(result);

    table = build_symtab_node();
    has_main = false;
    sem_result = 0;
    struct mcc_ast_visitor pre = ast_symtab_visitor(table);
    struct mcc_ast_program *program = result -> program;

    mcc_ast_visit_program( program, &pre);

    symtab_add_function_name();
    destroy_symtab_node(table);

    if(has_main == false){

        char err_msg[1024] = { 0};

        mcc_error_list(err_msg, 0, st_result, ERROR_NO_MAIN);
    }

    return st_result;
}

