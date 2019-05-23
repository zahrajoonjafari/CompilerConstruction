

#include <assert.h>
#include <stdlib.h>

#include <zlib.h>

#include "mcc/symtab.h"
#include "mcc/error.h"
#include <mcc/ast.h>
#include <mcc/ast_visit.h>






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

void  add_function_scope(struct mcc_ast_function_def *fun, void *data);

void  del_function_scope(struct mcc_ast_function_def *fun, void *data);

void symtab_call_expr_decl(struct mcc_ast_expression *expr, void *data);

struct  mcc_ast_curr_function *build_func_node();

void delete_curt_function_node(struct mcc_ast_curr_function *f);

struct mcc_ast_symbol *symtab_insert( struct mcc_ast_symbol_table *st, char *elem,
                                      int type,
                                      int num_params,
                                      int symbol_size);

enum mcc_ast_data_type symtab_identifier_type(struct mcc_ast_symbol_table *st, char *id);

struct mcc_ast_visitor create_symtab(struct mcc_ast_symbol_table *data);


//----------------------------------------------------------------------------------Definition

struct mcc_ast_visitor create_symtab(struct mcc_ast_symbol_table *data){

    assert(data);

    return (struct mcc_ast_visitor){

            .traversal = MCC_AST_VISIT_DEPTH_FIRST,
            .order = MCC_AST_VISIT_PRE_ORDER,

            .userdata = data,

                     .identifier = symbol_table_identifier,
                     .statement_declaration = symtab_add_decl_stmt,
                     .expression_identifier = symtab_expression_decl,
                     .expression_call = symtab_call_expr_decl,
                     .statement_assignment = symtab_assignment_decl,
                     .statement_ret = symtab_return_decl,
                     .statement_compound = add_statement_scope,
                     .close_statement_compound = del_statement_scope,
                     .function_def = add_function_scope,
                     .close_function_def = del_function_scope


    };
}

struct  mcc_ast_symbol_table *build_symtab_node(){



    table = malloc(sizeof(struct mcc_ast_symbol_table));
    table->next = NULL;
    table->before = NULL;
    table->counter_sym = 0;
    table->symbol = NULL;
    return table;

}

struct mcc_ast_symbol_table  *symtab_add_function_name(){



    char *print = "print";
    char *print_nl = "print_nl";
    char *print_int = "print_int";
    char *print_float = "print_float";
    char *read_int = "read_int";
    char *read_float = "read_float";
    struct mcc_ast_symbol_table *temp = malloc(sizeof(temp));
    symtab_insert(table, print,  MCC_AST_DATA_TYPE_VOID, 1, 0);
    symtab_insert(table, print_nl, MCC_AST_DATA_TYPE_VOID, 0 , 0);
    symtab_insert(table, print_int, MCC_AST_DATA_TYPE_VOID, 1, 0);
    symtab_insert(table, print_float, MCC_AST_DATA_TYPE_VOID, 1, 0);
    symtab_insert(table, read_int, MCC_AST_DATA_TYPE_INT,  0, 0);
    symtab_insert(table, read_float,  MCC_AST_DATA_TYPE_FLOAT, 0, 0);


    return temp;

}

struct mcc_ast_symbol *symtab_insert( struct mcc_ast_symbol_table *st, char *elem,
                                      int type,
                                      int num_params,
                                      int symbol_size) {
    assert(st);
    assert(elem);


    struct mcc_ast_symbol *sym = malloc(sizeof(sym));

    sym->symbol_size = symbol_size;
    sym->type = type;
    sym->param_count = num_params;
    sym->elem = elem;
    sym->elem = realloc(st->symbol, sizeof(struct mcc_ast_symbol)* (st->counter_sym +1));
    memcpy(&(st->symbol[st->counter_sym]), sym, sizeof(*sym));


    st->counter_sym++;

    free(sym);


    return st->symbol;// TODO

}

/*
struct  mcc_ast_curr_function *build_func_node(){

struct mcc_ast_curr_function *node = malloc(sizeof(node));

node->before = NULL;
node->next = NULL;
node->type = MCC_AST_DATA_TYPE_VOID;

    return node;
}


void delete_curr_function(struct mcc_ast_curr_function *f){

assert(f);
   while (f->before != NULL){

       f = f->before;

   }
struct mcc_ast_curr_function *current = f;
   while(current != NULL){

       struct mcc_ast_curr_function *new = current->next;
       free(current);
       current = new;
   }

}*/
void symtab_return_decl(struct mcc_ast_statement *stmt, void *data)

{
    assert(stmt);
    assert(data);

    struct mcc_ast_symbol_table *current = table;
    while(current-> before != NULL){

        current = current-> before;
    }

    stmt->ret_symbol = current->symbol;
    //stmt->returns  = true;
    if(stmt->ret_expr == NULL && stmt->ret_symbol->type != MCC_AST_DATA_TYPE_VOID){

        if(exist_error){
        char err_msg[1024] = { 0 };

        printf(err_msg, sizeof(err_msg),ERROR_NO_RETURN);

        mcc_error_list(err_msg, stmt->node.sloc.start_line , st_result, stmt->ret_symbol->elem);

       }
    }

}


struct mcc_ast_symbol *identifier_lookup(struct mcc_ast_symbol_table *st, char *id){

    assert(st);
    assert(id);

    struct mcc_ast_symbol_table *current  = st;

    for (int i = 0; i < st->counter_sym; i++) {

         //char *current = st->symbol[i].elem;
        if (strcmp(current->symbol[i].elem , id) == 0) {


            return current->symbol;
        }
    }
    return NULL;
}



enum mcc_ast_data_type symtab_identifier_type(struct mcc_ast_symbol_table *st, char *id)
{
    assert(st);
    assert(id);

    while (st->symbol != NULL) {
        for (int i = 0; i < st->counter_sym; i++) {

            if (strcmp(st->symbol[i].elem, id) == 0 ) {

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

    if(strcmp(table->symbol[i].elem, identifier->value) == 0){

        stmt->declaration->decl_symbol = symtab_insert(table, identifier->value,
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
        sym = identifier_lookup(current, id);
       // expr->ident_symbol->new_symbol = sym;
        //expr->ident_symbol->param_count++;

    }
    if (sym == NULL)
    {   if(exist_error){
       char err_msg[1024] = {0};

        printf(err_msg, sizeof(err_msg), ERROR_ID_UNDEF, expr->identifier->value);

        mcc_error_list(err_msg, expr->node.sloc.start_line, sem_result);
        }
    } else
    {
        free(expr->identifier->value);
    }
}

int arg_num_lookup(struct mcc_ast_symbol_table *st , char *id){


    assert(st);
    assert(id);


    while(st){

        for(int i = 0; i < st->counter_sym; i++)

            if(strcmp(st->symbol[i].elem, id ) == 0){


                return st->symbol[i].param_count;
            }
          st = st->before;
    }


    return 0;

}
void symtab_call_expr_decl(struct mcc_ast_expression *expr, void *data){

    assert(expr);
    assert(data);

    int num_arg = arg_num_lookup(table, expr->identifier_call->value);

    if(expr->arguments != NULL){

      if(expr->arguments->argument_count != num_arg ){
         if(exist_error){
          char err_msg[1024] =  {0};

          printf(err_msg, sizeof(err_msg), ERROR_ARG_NUM, expr->identifier_call->value,
                                           num_arg, expr->arguments->argument_count);

          mcc_error_list(err_msg, expr->identifier_call->node.sloc.start_line, st_result);
         }
      }

    }

    if(num_arg != 0){
        if(exist_error){

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
    struct mcc_ast_symbol_table *temp = table;
    struct mcc_ast_symbol *symbol = identifier_lookup(table, identifier);

    while(symbol == NULL && temp->before){

        temp = temp->before;
        symbol = identifier_lookup(temp, identifier);
    }

    if(symbol != NULL && symbol->symbol_size != -1){


        stmt->assignment-> assign_symbol= symbol;
        stmt->assignment-> assign_symbol ->param_count ++;


    } else {

        if(exist_error){
        char err_msg[1024] = {0};

        printf(err_msg, sizeof(err_msg), ERROR_ID_UNDEF);

        mcc_error_list(err_msg, stmt->assignment->node.sloc.start_line, sem_result, identifier);
        }
    }

    if (stmt->assignment->type == MCC_AST_EXPRESSION_TYPE_IDENTIFIER ){


        if (symbol != NULL  ){


            if (stmt->assignment->array_assign.assign_expr == NULL){
                if(exist_error){

                char err_msg[1024] = {0};

                printf(err_msg, sizeof(err_msg), ERROR_ASSIGN_TYPE_MIS);

                mcc_error_list(err_msg, stmt->assignment->node.sloc.start_line, sem_result);

                }
            }
        }
    }

}




void  add_function_scope(struct mcc_ast_function_def *fun, void *data) {

    assert(fun);
    assert(data);

    struct mcc_ast_parameter *parameter = fun->parameter;

    char *identifier = fun->identifier->value;


    if (identifier_lookup(table, identifier) == NULL) {

        symtab_insert(table, fun->identifier->value,
                      fun->data_type, parameter->param_len, -1);

        struct mcc_ast_symbol *symbol = identifier_lookup(table, identifier);

        for (int i = 0; i < parameter->param_len; i++) {

            symbol->param_count = parameter->decl_parameter[i]->type;
        }
    } else {

        if(exist_error){

        char err_msg[1024] = {0};

        printf(err_msg, sizeof(err_msg), ERROR_FUNC_DUBL);

        mcc_error_list(err_msg, fun->node.sloc.start_line, sem_result, identifier);

        }
    }

    struct mcc_ast_symbol_table *tab_new = malloc(sizeof(*tab_new));

    tab_new->before = table;
    
    tab_new->counter_sym = 0;

    table = tab_new;


    for (int i = 0; i < parameter->param_len; i++) {

       char *id = parameter->decl_parameter[i]->declaration->ident->value;

        if (identifier_lookup(tab_new, id) == NULL) {

            parameter->decl_parameter[i]->declaration->decl_symbol =
                    symtab_insert(tab_new, id, parameter->decl_parameter[i]->declaration[i].decl_lit->type,
                                  parameter->decl_parameter[i]->declaration->decl_arr_size, 0);

            parameter->decl_parameter[i]->declaration->decl_symbol->param_count++;
        }
    }

}



void del_function_scope(struct mcc_ast_function_def *fun, void *data)

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
    st_result = result;
    exist_error = false;
    struct mcc_ast_function *function = result->function_parser;
     sem_result = 0;

    table = build_symtab_node();
    has_main = false;

    struct mcc_ast_curr_function *current_fun = build_func_node();

    struct mcc_ast_visitor visitor = create_symtab(table);

    symtab_add_function_name();
    mcc_ast_visit_function(function, &visitor);
    destroy_symtab_node(table);

    if(has_main == false){

        char err_msg[1024] = { 0};

        mcc_error_list(err_msg, 0, st_result, ERROR_NO_MAIN);
    }

     delete_curt_function_node(current_fun);
    return st_result;
}

