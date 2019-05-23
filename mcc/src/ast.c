

#include "mcc/ast.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <mcc/symtab.h>






// ---------------------------------------------------------------- Expressions

struct mcc_ast_expression * mcc_ast_new_expression_binary_op(enum mcc_ast_binary_op op,
                                                             struct  mcc_ast_expression *lhs,
                                                             struct mcc_ast_expression *rhs)
{
    assert(lhs);
    assert(rhs);

    struct mcc_ast_expression *expr = malloc(sizeof(*expr));
    if (!expr) {
        return NULL;
    }

    expr->type = MCC_AST_EXPRESSION_TYPE_BINARY_OP;
    expr->op = op;
    expr->lhs = lhs;
    expr->rhs = rhs;
    return expr;
}

struct mcc_ast_expression *mcc_ast_new_expression_literal(struct mcc_ast_literal *literal)
{
    assert(literal);

    struct mcc_ast_expression *expr = malloc(sizeof(*expr));
    if (!expr) {
        return NULL;
    }

    expr->type = MCC_AST_EXPRESSION_TYPE_LITERAL;
    expr->literal=literal;
    expr ->data_type = -1;

    return expr;
}





struct mcc_ast_expression *mcc_ast_new_expression_unary_op(enum mcc_ast_unary_op unary_op,
                                                           struct mcc_ast_expression *rhs)
{

    assert(rhs);

    struct mcc_ast_expression *expr = malloc(sizeof(*expr));
    if (!expr) {
        return NULL;
    }

    expr -> type = MCC_AST_EXPRESSION_TYPE_UNARY_OP,
    expr -> unary_op = unary_op;
    expr -> unary_expression = rhs;
    return expr;


}
struct mcc_ast_expression *mcc_ast_new_expression_identifier(struct  mcc_ast_identifier *identifier,
                                                             struct mcc_ast_expression *expression)


{

    assert(identifier);
    assert(expression);

    struct mcc_ast_expression *expr = malloc(sizeof(*expr));
    if(!expr) {
        return NULL;
    }

    expr -> type = MCC_AST_EXPRESSION_TYPE_IDENTIFIER;
    expr -> identifier = identifier;
    expr -> id_expr = expression;
    expr ->data_type = -1;
    expr->ident_symbol = NULL;

    return expr;


}

struct mcc_ast_expression *mcc_ast_new_expression_call_expr(struct mcc_ast_identifier *identifier_call,
                                                            struct mcc_ast_arguments *arguments )

{
    assert(identifier_call);
    assert(arguments);
    struct mcc_ast_expression *expr = malloc(sizeof(*expr));
    if (!expr) {
        return NULL;
    }

    expr->type = MCC_AST_EXPRESSION_TYPE_CALL_EXPR;
    expr->identifier_call = identifier_call;
    expr->arguments = arguments;
    expr ->data_type = -1;
    expr->call_symbol = NULL;

    return expr;

}


struct mcc_ast_expression *mcc_ast_new_expression_parenth(struct mcc_ast_expression *expression)
{
    assert(expression);

    struct mcc_ast_expression *expr = malloc(sizeof(*expr));
    if (!expr) {
        return NULL;
    }

    expr->type = MCC_AST_EXPRESSION_TYPE_PARENTH;
    expr->expression = expression;
    expr ->data_type = -1;

    return expr;
}


//---------------------------------------------


void mcc_ast_delete_expression(struct mcc_ast_expression *expression) {

    switch (expression->type) {

        case MCC_AST_EXPRESSION_TYPE_BINARY_OP:

            if(expression->rhs){mcc_ast_delete_expression(expression->rhs);}
            if(expression->lhs){mcc_ast_delete_expression(expression->rhs);}

            break;

        case MCC_AST_EXPRESSION_TYPE_LITERAL:
            mcc_ast_delete_literal(expression->literal);
            break;

        case MCC_AST_EXPRESSION_TYPE_PARENTH:
            mcc_ast_delete_expression(expression->expression);
            break;


        case MCC_AST_EXPRESSION_TYPE_UNARY_OP:
            mcc_ast_delete_expression(expression->unary_expression);
            break;

        case MCC_AST_EXPRESSION_TYPE_IDENTIFIER:
            free(expression->identifier);
            if(expression->ident_symbol){
                free(expression->ident_symbol);
            }
            mcc_ast_delete_expression(expression->id_expr);
            break;

        case MCC_AST_EXPRESSION_TYPE_CALL_EXPR:
            free(expression->identifier_call);
            if(expression->call_symbol){
                free(expression->call_symbol);
            }
            mcc_ast_delete_arguments(expression->arguments);
            break;

    }

    free(expression);
}

//--------------------------------------------------------------------------Statements



struct mcc_ast_statement *mcc_ast_new_statement_expression (struct mcc_ast_expression *expression){

    assert(expression);

    struct mcc_ast_statement *stmt= malloc(sizeof(stmt));

    if(!stmt) {
        return NULL;
    }

    stmt->type = MCC_AST_STATEMENT_TYPE_EXPRESSION;
    stmt->expression =expression;
    stmt -> returns = false;
    return  stmt;
}


struct mcc_ast_statement *mcc_ast_new_statement_if(struct mcc_ast_expression *if_cond,
                                                   struct mcc_ast_statement *if_stmt,
                                                   struct mcc_ast_statement *else_stmt)

{struct mcc_ast_statement *stmt= malloc(sizeof(stmt));

    if(!stmt) {
        return NULL;
    }


    stmt->type = MCC_AST_STATEMENT_TYPE_IF;
    stmt -> if_cond = if_cond;
    stmt->if_stmt = if_stmt;
    stmt->else_stmt = else_stmt;
    stmt->returns = false;


    return stmt;


}



struct mcc_ast_statement *mcc_ast_new_statement_while(struct mcc_ast_expression *while_cond,
                                                      struct mcc_ast_statement *while_stmt)
{

    struct mcc_ast_statement *stmt= malloc(sizeof(stmt));

    if(!stmt) {
        return NULL;
    }

    stmt->type=MCC_AST_STATEMENT_TYPE_WHILE;
    stmt->while_cond = while_cond;
    stmt->while_stmt = while_stmt;
    stmt->returns = false;


    return  stmt;

}

struct mcc_ast_statement *mcc_ast_new_statement_assignment(struct mcc_ast_assignment *assignment)
{

    struct mcc_ast_statement *stmt= malloc(sizeof(stmt));

    if(!stmt) {
        return NULL;
    }



    stmt->type = MCC_AST_STATEMENT_TYPE_ASSIGN;
    stmt->assignment = assignment;
    stmt->returns = false;

    return stmt;


}


struct mcc_ast_statement *mcc_ast_new_statement_declaration(struct mcc_ast_declaration *declaration)
{


    assert(declaration);


    struct mcc_ast_statement *stmt= malloc(sizeof(stmt));

    if(!stmt) {
        return NULL;
    }



    stmt->type = MCC_AST_STATEMENT_TYPE_DECL;
    stmt->declaration= declaration;
    stmt -> returns = false;
    return stmt;

}

struct mcc_ast_statement *mcc_ast_new_statement_compound_single(struct mcc_ast_statement *statement){

    assert(statement);

    struct mcc_ast_statement *new_stmt = malloc(sizeof(new_stmt));
    if(!new_stmt){

        return NULL;
    }

  new_stmt->stmt = &statement;
   new_stmt->max_stmt= 1;

    return new_stmt;

}



struct mcc_ast_statement *mcc_ast_new_statement_compound(struct mcc_ast_statement *compound_stmt,
                                                         struct mcc_ast_statement *statement)
{

    if(compound_stmt->num_stmt >= compound_stmt->max_stmt){

        void *new_stmt = realloc(compound_stmt->stmt, sizeof(struct mcc_ast_statement *) * compound_stmt->max_stmt * 2);

        compound_stmt-> stmt = new_stmt;
        compound_stmt ->max_stmt *= 2;

    }

    compound_stmt->stmt[compound_stmt->num_stmt] = statement;
    compound_stmt->num_stmt++;

    //struct mcc_ast_statement *stmt = malloc(sizeof(stmt));


    return compound_stmt;
}

struct mcc_ast_statement *mcc_ast_new_statement_single(struct mcc_ast_statement *statement){


    struct mcc_ast_statement *stmt= malloc(sizeof(stmt));

    if(!stmt) {
        return NULL;
    }

    stmt->type = MCC_AST_STATEMENT_TYPE_COMPOUND_STMT;
    stmt-> max_stmt = 1;
    stmt-> num_stmt= 0;
    stmt ->returns = false;
    stmt->stmt = malloc(sizeof(*stmt) * stmt->max_stmt);
    stmt->stmt[0] = statement;

    return  stmt;


}


struct mcc_ast_statement *mcc_ast_new_statement_ret(struct mcc_ast_expression* ret_expr)
{

    struct mcc_ast_statement *stmt= malloc(sizeof(stmt));

    if(!stmt) {
        return NULL;
    }

    stmt-> type = MCC_AST_STATEMENT_TYPE_RET;
    stmt -> ret_expr = ret_expr;
    stmt -> returns = true;
    stmt->ret_symbol = NULL;

    return stmt;

}

struct mcc_ast_statement *mcc_ast_new_statement_empty()
{

    struct mcc_ast_statement *stmt = malloc(sizeof(*stmt));
    if (!stmt) {
        return NULL;
    }

    stmt->type = MCC_AST_STATEMENT_TYPE_COMPOUND_STMT;
    stmt->max_stmt = 1;
    stmt->stmt = malloc(sizeof(*stmt) * stmt->max_stmt);
    stmt->num_stmt = 0;
    stmt->ret_expr =  false;

    return stmt;
}



void mcc_ast_delete_statement(struct mcc_ast_statement *statement)
{
    switch (statement->type){


        case (MCC_AST_STATEMENT_TYPE_EXPRESSION):
            if(statement->expression){
                mcc_ast_delete_expression(statement->expression);}
            break;

        case (MCC_AST_STATEMENT_TYPE_ASSIGN):
            mcc_ast_delete_assignment(statement->assignment);
            break;

        case (MCC_AST_STATEMENT_TYPE_COMPOUND_STMT):
            for(int i = 0; i < statement->num_stmt; i++){
                mcc_ast_delete_statement(statement->stmt[i]);

            }

            break;

        case (MCC_AST_STATEMENT_TYPE_IF):
            mcc_ast_delete_statement(statement->if_stmt);
            mcc_ast_delete_statement(statement->else_stmt);
            mcc_ast_delete_expression(statement->if_cond);

            break;
        case (MCC_AST_STATEMENT_TYPE_DECL):
            mcc_ast_delete_declaration(statement->declaration);
            break;

        case (MCC_AST_STATEMENT_TYPE_WHILE):
            mcc_ast_delete_statement(statement->while_stmt);
            mcc_ast_delete_expression(statement->while_cond);
            break;

        case (MCC_AST_STATEMENT_TYPE_RET):
            mcc_ast_delete_expression(statement->ret_expr);
            break;
    }



}

//-------------------------------------------------------------------- Identifier




struct  mcc_ast_identifier *mcc_ast_new_identifier(char *val_identifier, enum mcc_ast_data_type data_type)
{
    assert(val_identifier);

    struct mcc_ast_identifier *id = malloc(sizeof(id));

    if (!id) {
        return NULL;
    }
    id->value=val_identifier;
    char *new_value = malloc(strlen(val_identifier) +1 * sizeof(char) *2);
    strcpy(new_value, val_identifier);
    id->value = new_value;
    id->data_type = data_type;


    return id;
}

void mcc_ast_delete_identifier(struct mcc_ast_identifier *identifier)
{
    assert(identifier);

    free(identifier->value);
    free(identifier);
}


//-------------------------------------------------------------------- Arguments


struct mcc_ast_arguments *mcc_ast_new_arguments(struct mcc_ast_expression *arg_expr)
{

    assert(arg_expr);

    struct mcc_ast_arguments *arg = malloc(sizeof(arg));

    if (!arg) {

        return  NULL;
    }

    arg -> arg_expr[0] = arg_expr;
    arg -> argument_count = 1 ;
    arg -> argument_size = 0;


    return arg;
}


struct mcc_ast_arguments *mcc_ast_new_arguments_expr(struct mcc_ast_arguments *arguments,
                                                     struct mcc_ast_expression *arg_expr) {
    assert(arguments);
    assert(arg_expr);

    // allocate more memory

    if (arguments->argument_size > arguments->argument_count) {

        void *new_arg = realloc(arguments,sizeof(struct mcc_ast_expression *) * arguments->argument_count * 2);

        if (!new_arg) {

            return NULL;
        }
        arguments -> arg_expr = new_arg;
        arguments->argument_count *= 2;

    }

    arguments->argument_size++;


    return arguments;
}

void mcc_ast_delete_arguments(struct mcc_ast_arguments *arguments){

    assert(arguments);
    for(int i = 0; i < arguments->argument_count; i++){

        mcc_ast_delete_expression(arguments->arg_expr[i]);

    }

    free(arguments->arg_expr);
    free(arguments);
}

// ------------------------------------------------------------------- Literals

struct mcc_ast_literal *mcc_ast_new_literal_int(long value)
{
    struct mcc_ast_literal *lit = malloc(sizeof(*lit));
    if (!lit) {
        return NULL;
    }

    lit->type = MCC_AST_LITERAL_TYPE_INT;
    lit->i_value = value;
    return lit;
}

struct mcc_ast_literal *mcc_ast_new_literal_float(double value)
{
    struct mcc_ast_literal *lit = malloc(sizeof(*lit));
    if (!lit) {
        return NULL;
    }

    lit->type = MCC_AST_LITERAL_TYPE_FLOAT;
    lit->f_value = value;
    return lit;
}

struct mcc_ast_literal *mcc_ast_new_literal_string(const char* value)
{
    struct mcc_ast_literal *lit = malloc(sizeof(*lit));


    if (!lit) {
        return NULL;
    }

    lit -> type = MCC_AST_LITERAL_TYPE_STRING;
    lit -> s_value = value;

    return lit;
}

struct mcc_ast_literal *mcc_ast_new_literal_bool(bool value)
{
    struct mcc_ast_literal *lit = malloc(sizeof(*lit));

    if (!lit) {
        return NULL;
    }

    lit -> type = MCC_AST_LITERAL_TYPE_BOOL;
    lit -> b_value = value;

    return lit;
}



void mcc_ast_delete_literal(struct mcc_ast_literal *literal)
{
    assert(literal);

    free(literal);

}


//--------------------------------------------------------------------------Declaration


struct mcc_ast_declaration *mcc_ast_new_declaration
        ( struct mcc_ast_literal *decl_lit, struct mcc_ast_identifier *ident, int decl_arr_size)
{

    assert(ident);

    struct mcc_ast_declaration * decl =malloc(sizeof(decl));

    if(!decl){

        return  NULL;
    }

    decl->decl_arr_size = decl_arr_size;
    decl->decl_lit = decl_lit;
    decl->ident=ident;
    decl ->decl_symbol = NULL;

    return  decl;

}


void mcc_ast_delete_declaration(struct mcc_ast_declaration *decl)
{
    assert(decl);

    mcc_ast_delete_identifier(decl->ident);
    if (decl->decl_lit) {
        mcc_ast_delete_literal(decl->decl_lit);
    }
    free(decl);
}

//-------------------------------------------------------------------------Assignments

struct mcc_ast_assignment *
mcc_ast_new_assignment(struct mcc_ast_identifier *identifier,
                       struct mcc_ast_expression *rhs)
{

    assert(rhs);
    assert(identifier);

    struct mcc_ast_assignment *assign = malloc(sizeof(assign));

    if(!assign){

        return NULL;
    }

    assert(assign);
    assign-> type = MCC_AST_ASSIGNMENT_TYPE_SINGLE;
    assign->identifier=identifier;
    assign->single_assign.rhs = rhs;
    assign -> assign_symbol = NULL;
    assign ->ass_counter = NULL;
    return assign;

}


struct mcc_ast_assignment *
mcc_ast_new_array_assignment(struct mcc_ast_identifier *identifier,
                             struct mcc_ast_expression *assign_expr,
                             struct mcc_ast_expression *rhs)

{
    struct mcc_ast_assignment *assign = malloc(sizeof(assign));
    if(!assign){

        return NULL;

    }

    assign ->type=MCC_AST_ASSIGNMENT_TYPE_ARRAY;
    assign->array_assign.rhs= rhs;
    assign->identifier=identifier;
    assign->array_assign.assign_expr = assign_expr;
    assign ->ass_counter = assign_expr;

    return assign;

}

void mcc_ast_delete_assignment(struct mcc_ast_assignment *assignment)
{
    assert(assignment);

    mcc_ast_delete_identifier(assignment->identifier);
    if (assignment->type == MCC_AST_ASSIGNMENT_TYPE_SINGLE) {
        mcc_ast_delete_expression(assignment->single_assign.rhs);
    } else {
        mcc_ast_delete_expression(assignment->array_assign.assign_expr);
        mcc_ast_delete_expression(assignment->array_assign.rhs);
    }
    free(assignment);
}

//-----------------------------------------------------------------------------Parameters

struct mcc_ast_parameter *mcc_ast_new_parameter(struct mcc_ast_statement *decl_parameter, struct mcc_ast_parameter *parameter)
{
    assert(decl_parameter);



    if(parameter == NULL){

        struct mcc_ast_parameter *params = malloc(sizeof(*params) + sizeof(struct mcc_ast_declaration*) * 4);
        params -> param_size = 1;
        params-> param_len = 4 ;
        params -> decl_parameter[0] = decl_parameter;
        return params;

    }



    if ((parameter -> param_size) == (parameter -> param_len)) {

        int next_param_len = parameter -> param_size + 4;
        int next_param_size = parameter -> param_size;
        struct mcc_ast_parameter *new_params = realloc(parameter, sizeof(*parameter) + sizeof(struct mcc_ast_declaration*) * next_param_len);
        new_params -> decl_parameter[next_param_size -1] = decl_parameter;
        new_params -> param_size += 1;
        new_params -> param_len = next_param_len;
    } else {
        parameter -> decl_parameter[(parameter -> param_size) - 1] = decl_parameter;
        parameter -> param_size += 1;
    }

    return parameter;
}

struct mcc_ast_parameter *mcc_ast_new_empty_parameter()
{
    struct mcc_ast_parameter *new_param = malloc(sizeof(new_param));

    if(new_param){

        return NULL;
    }

    new_param->decl_parameter = NULL;
    new_param->param_size = 0;

    return new_param;
}


void mcc_ast_delete_parameter(struct mcc_ast_parameter *parameter) {
    if (parameter != NULL) {
        for (int i = 0; i < parameter -> param_size; i++) {
            mcc_ast_delete_statement(parameter->decl_parameter[i]);
        }

        free(parameter);
    }
}
//----------------------------------------------------------------------------Function






struct mcc_ast_function_def *mcc_ast_new_function_def_type(enum mcc_ast_data_type data_type,
                                                           struct mcc_ast_identifier *identifier,
                                                           struct mcc_ast_parameter *parameter,
                                                           struct mcc_ast_statement *compound_stmt)

{
    assert(identifier);
    assert(parameter);
    assert(compound_stmt);

    struct mcc_ast_function_def *fun = malloc(sizeof(fun));
    if(!fun){
        return NULL;
    }

    fun->parameter= parameter;
    fun->identifier= identifier;
    fun->data_type = data_type;
    fun->compound_stmt= compound_stmt;




    return fun;
}

struct mcc_ast_function *mcc_ast_new_function(struct mcc_ast_function *fun,
                                              struct mcc_ast_function_def *fun_def){

    assert(fun);
    assert(fun_def);

    if(fun->size_function >= fun->max_function){

    void * new_fun = realloc(fun, sizeof(struct mcc_ast_function *)*fun->max_function * 4);
      fun->fun_def= new_fun;
      fun->size_function *= 4 ;

    }

    //fun->fun_def[fun->max_function] = fun_def;
    fun->size_function++;
    return fun;
}

struct mcc_ast_function *mcc_ast_new_function_def(struct mcc_ast_function_def *fun){

  assert(fun);
  struct mcc_ast_function *func = malloc(sizeof(func));
  if(!func){
      return NULL;
  }

  func->max_function = 1;
  func->fun_def = fun;
  func->fun_def = malloc(sizeof(struct mcc_ast_function * )* func->max_function);
  func->size_function= 0;

    return func;
}


struct mcc_ast_function *mcc_ast_pro_func_def(struct mcc_ast_expression *expr){

    assert(expr);

    struct mcc_ast_function_def* fun = malloc(sizeof(fun));

    if(!fun){

        return NULL;
    }

    fun->compound_stmt= mcc_ast_new_statement_compound_single(mcc_ast_new_statement_expression(expr));
    fun->identifier->data_type = MCC_AST_DATA_TYPE_VOID;
    fun->parameter = mcc_ast_new_empty_parameter();
    //fun->identifier = mcc_ast_new_identifier();
    fun->data_type = MCC_AST_DATA_TYPE_VOID;

    struct mcc_ast_function *function = malloc(sizeof(function));

    function->max_function = 1;
    function->fun_def = fun;
    return function;

}
void mcc_ast_delete_function(struct mcc_ast_function *function) {
    assert(function);

    mcc_ast_delete_function(function);

    if (function ->fun_def != NULL) {
        mcc_ast_delete_parameter(function -> fun_def->parameter);
        mcc_ast_delete_statement(function->fun_def->compound_stmt);
        mcc_ast_delete_identifier(function->fun_def->identifier);

    }

    free(function);
}


//----------------------------------------------------------------------------Program
/*
struct mcc_ast_program *mcc_ast_new_program_empty()
{
    struct mcc_ast_program *pro=malloc(sizeof(pro));
    pro->max_function = 1;

    pro->function = malloc(sizeof(struct mcc_ast_function *)*pro->max_function);
    pro->size_function = 0;

    return pro;


}


struct mcc_ast_program *mcc_ast_new_program(struct mcc_ast_program *program,
                                            struct mcc_ast_function *function)
{

    assert(program);
    assert(function);


    //struct mcc_ast_program *pro = malloc(sizeof(pro));
    //program->max_function = 1;
    //program->size_function = 0;
    //pro->function = malloc(sizeof(struct mcc_ast_function *)* pro->max_function);

      //  program = pro;


    if(program->size_function >= program->max_function){

        void *new_func = realloc(program->function, sizeof(struct mcc_ast_function*)* program-> max_function * 4);
        program->function = new_func;
        program->size_function *= 4;

    }

    program->function[program->size_function] = function;
    program->size_function++;


    return program;


}


void mcc_ast_delete_program(struct mcc_ast_program *pro)
{
    if (pro != NULL) {
        for (int i = 0; i < pro->size_function; i++) {
            mcc_ast_delete_function(pro->function[i]);
        }

        free(pro->function);
        free(pro);
    }
}*/