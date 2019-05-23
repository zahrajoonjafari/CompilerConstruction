#include <assert.h>
#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
#include "mcc/error.h"
#include <mcc/symtab.h>
#include <mcc/ast_visit.h>
#include <mcc/ast.h>
#include <parser.h>


void semantic_chk_expression(struct mcc_ast_expression *expr, void *data);

void semantic_chk_call_expr(struct mcc_ast_expression *expression, void *data);

void semantic_chk_assign_stmt_type(struct mcc_ast_statement *stmt,  void *data);

void semantic_chk_if_stmt_type(struct mcc_ast_statement*stmt,  void *data);

void semantic_chk_while_stmt_type(struct mcc_ast_statement*stmt,  void *data);

void semantic_chk_ret_stmt_type(struct mcc_ast_statement *stmt,  void *data);

void semantic_chk_compound_stmt(struct mcc_ast_statement *stmt, void *data);


void semantic_chk_function_returns(struct mcc_ast_function_def *fun, void *data);

struct mcc_parser_result *mcc_ast_semantic_check(struct mcc_parser_result *result);


enum mcc_ast_data_type get_dt(enum mcc_ast_literal_type type);



struct mcc_ast_visitor create_symtab(void *data);
//-------------------------------------------------------------


struct mcc_ast_visitor create_symtab(void *data){

    assert(data);
    return (struct mcc_ast_visitor){

            .traversal = MCC_AST_VISIT_DEPTH_FIRST,
            .order = MCC_AST_VISIT_POST_ORDER,

            .userdata = data,

            .expression = semantic_chk_expression,
            .expression_call = semantic_chk_call_expr,
            .statement_assignment = semantic_chk_assign_stmt_type,
            .statement_ret = semantic_chk_ret_stmt_type,
            .statement_compound = semantic_chk_compound_stmt,
            .statement_if = semantic_chk_if_stmt_type,
            .statement_while = semantic_chk_while_stmt_type,
            .function_def = semantic_chk_function_returns,

    };

}




 enum mcc_ast_data_type get_dt(enum mcc_ast_literal_type type) {

    switch (type) {

        case (MCC_AST_LITERAL_TYPE_INT):
            return MCC_AST_DATA_TYPE_INT;

        case (MCC_AST_LITERAL_TYPE_STRING):
            return MCC_AST_DATA_TYPE_STRING;

        case (MCC_AST_LITERAL_TYPE_BOOL):
            return MCC_AST_DATA_TYPE_BOOL;

        case (MCC_AST_LITERAL_TYPE_FLOAT):
            return MCC_AST_DATA_TYPE_FLOAT;
    }
    return MCC_AST_DATA_TYPE_VOID;

}


struct mcc_ast_symbol *identifier_lookup(struct mcc_ast_symbol_table *st, char *id){

    assert(st);
    assert(id);

    struct mcc_ast_symbol_table *current  = st;

    for (int i = 0; i < st->counter_sym; i++) {

        // char *current = st->symbol[i].elem;
        if (strcmp(current->symbol[i].elem , id) == 0) {

            return current->symbol;
        }
    }
    return NULL;
}



void semantic_chk_expression(struct mcc_ast_expression *expr, void *data) {

    assert(expr);
    assert(data);

    switch (expr->type) {

        case MCC_AST_EXPRESSION_TYPE_LITERAL:

            expr->data_type = get_dt(expr->literal->type);
            break;

        case MCC_AST_EXPRESSION_TYPE_IDENTIFIER:

            if (expr->identifier->value != NULL) {
                expr->type = expr->id_expr->type;
            }

            break;

        case MCC_AST_EXPRESSION_TYPE_UNARY_OP:
            expr->type = expr->expression->type;
            break;

        case MCC_AST_EXPRESSION_TYPE_CALL_EXPR:
            expr->type = expr->expression->type;
            break;

        case MCC_AST_EXPRESSION_TYPE_PARENTH:
            expr->type = expr->expression->type;
            break;

        case MCC_AST_EXPRESSION_TYPE_BINARY_OP:

            if (expr->lhs->data_type == expr->rhs->data_type) {
                if (expr->op == MCC_AST_BINARY_OP_ADD ||
                    expr->op == MCC_AST_BINARY_OP_SUB ||
                    expr->op == MCC_AST_BINARY_OP_MUL ||
                    expr->op == MCC_AST_BINARY_OP_DIV) {

                    expr->data_type = expr->lhs->data_type;
                    expr->data_type = expr->rhs->data_type;

                } else {
                    if (expr->lhs->data_type == expr->rhs->data_type) {
                        if (expr->op == MCC_AST_BINARY_OP_AND ||
                            expr->op == MCC_AST_BINARY_OP_OR ||
                            expr->op == MCC_AST_BINARY_OP_EQUALS ||
                            expr->op == MCC_AST_BINARY_OP_NOT_EQUALS ||
                            expr->op == MCC_AST_BINARY_OP_LESS ||
                            expr->op == MCC_AST_BINARY_OP_LESS_EQ ||
                            expr->op == MCC_AST_BINARY_OP_GREATER ||
                            expr->op == MCC_AST_BINARY_OP_GREATER_EQ) {


                            expr->data_type = MCC_AST_DATA_TYPE_BOOL;
                        }
                    }
                }
            } else {

                if(exist_error){
                char err_msg[1024] = {0};

                printf(err_msg, sizeof(err_msg), ERROR_BINARY_OP_TYPE, print_type(expr->lhs->data_type),
                                                     print_type(expr->rhs->data_type));
                mcc_error_list(err_msg, expr->node.sloc.start_line, sem_result);

                }
            }
        }

}

void semantic_chk_function_returns(struct mcc_ast_function_def *fun, void *data)

{
    assert(fun);
    assert(data);

    if (fun->data_type!= MCC_AST_DATA_TYPE_VOID &&
        !fun->compound_stmt->returns) {

          if(exist_error){
        char err_msg[1024] = {0};

        printf(err_msg, ERROR_NO_RETURN);

        mcc_error_list(err_msg,  fun->node.sloc.start_line, sem_result);

          }

    }
}


 void semantic_chk_call_expr(struct mcc_ast_expression *expression, void *data) {

    assert(expression);
    assert(data);

    char *id  = expression->identifier_call->value;
     struct mcc_ast_symbol *symbol = identifier_lookup(table, id);


     if (expression->arguments != NULL) {
         for (int i = 0; i < expression->arguments->argument_count; i++) {
             if (expression->identifier_call != NULL &&
                 (expression->arguments->arg_expr[i]->data_type  != symbol->type))
                  {   if(exist_error){

                 char error_msg[1024] = {0};
                 printf(error_msg, sizeof(error_msg), ERROR_ARG_TYPE,
                          expression->identifier_call->value, i+1 , print_type(expression->identifier_call->data_type),
                                  print_type(expression->arguments->arg_expr[i]->data_type));

                 mcc_error_list(error_msg, expression->identifier_call->node.sloc.start_line, sem_result);


                      }

             } else {

                        for (i = 0; i < expression->arguments->argument_count != symbol->param_count; i++){
                             if(exist_error){
                            char error_msg[1024] = {0};
                            printf(error_msg, sizeof(error_msg), ERROR_ARG_NUM);
                            mcc_error_list(error_msg, expression->arguments->node.sloc.start_line, sem_result);

                        }

                   }
             }
         }
     }
    }

void semantic_chk_assign_stmt_type(struct mcc_ast_statement *stmt,  void *data)
{

    assert(stmt);
    assert(data);

    stmt -> type = MCC_AST_STATEMENT_TYPE_ASSIGN;

    if( stmt->type == MCC_AST_ASSIGNMENT_TYPE_SINGLE || stmt->type == MCC_AST_ASSIGNMENT_TYPE_ARRAY) {

        if (stmt->assignment->assign_symbol != NULL) {

            if (stmt->assignment->assign_symbol->symbol_size != -1) {

                if (stmt->assignment->array_assign.assign_expr == NULL) {

                    if (exist_error) {
                        char err_msg[1024] = {0};

                        printf(err_msg, ERROR_ASSIGN_ARRAY);

                        mcc_error_list(err_msg, stmt->node.sloc.start_line, sem_result);
                        return;

                    }
                }

            }

            if (stmt->assignment->assign_symbol->type != stmt->assignment->array_assign.assign_expr->data_type) {

                if (exist_error) {

                    char err_msg[1024] = {0};

                    printf(err_msg, ERROR_ASSIGN_TYPE_MIS);

                    mcc_error_list(err_msg, stmt->node.sloc.start_line, sem_result);
                }
            }
        }

    }

}

void semantic_chk_if_stmt_type(struct mcc_ast_statement*stmt,  void *data){

    assert(stmt);
    assert(data);

    if(stmt->expression->data_type != MCC_AST_DATA_TYPE_BOOL){

        if(exist_error){

        char err_msg[1024] = {0};

        printf(err_msg,  ERROR_IF_DEF_MIS);

        mcc_error_list(err_msg, stmt->node.sloc.start_line,sem_result,
                print_type(stmt->if_stmt->expression->data_type));

        }
    }

    if(stmt->if_stmt && stmt->else_stmt && stmt->if_cond != NULL){

        stmt->returns = true;
    }

}


void semantic_chk_while_stmt_type(struct mcc_ast_statement *stmt,  void *data)
{
    assert(stmt);
    assert(data);

    if(stmt->expression->data_type != MCC_AST_DATA_TYPE_BOOL){

        if(exist_error){

        char err_msg[1024] = {0};

        printf(err_msg,  ERROR_WHILE_DEF_MIS);

        mcc_error_list(err_msg, stmt->node.sloc.start_line,sem_result,
                       print_type(stmt->while_stmt->expression->data_type));
        }
    }

    if(stmt->while_stmt && stmt->while_cond != NULL){

        stmt->returns = true;
    }
}




void semantic_chk_ret_stmt_type(struct mcc_ast_statement *stmt, void *data)

{
    assert(stmt);
    assert(data);

    if(stmt->ret_expr != NULL){
        if(exist_error){
       if(stmt->ret_expr-> data_type != stmt->ret_symbol->type) {

           char err_msg[1024] = {0};

           printf(err_msg,  ERROR_RETURN_TYPE);

           mcc_error_list(err_msg,0 , sem_result, print_type(stmt->ret_expr->data_type), "void");

           }
        }
      }
//TODO

}


void semantic_chk_compound_stmt(struct mcc_ast_statement *stmt, void *data)
{
    assert(stmt);
    assert(data);

    for (int i = 0; i < stmt->num_stmt; i++) {
        if (stmt->stmt[i]->returns) {
            stmt->returns = true;
            return;
        }
    }
}









struct mcc_parser_result *mcc_ast_semantic_check( struct mcc_parser_result *result){

assert(result);


sem_result = result;

struct mcc_ast_function *function = result->function_parser;
 struct mcc_ast_visitor visitor = create_symtab(result);

 mcc_ast_visit_function(function, &visitor);

    return sem_result;

}