#include "mcc/ast_visit.h"

#include <assert.h>
#include <mcc/ast.h>

#include "string.h"

#define visit(node, callback, visitor) \
	do { \
		if (callback) { \
			(callback)(node, (visitor)->userdata); \
		} \
	} while (0)

#define visit_if(cond, node, callback, visitor) \
	do { \
		if (cond) { \
			visit(node, callback, visitor); \
		} \
	} while (0)


#define visit_if_pre_order(node, callback, visitor) \
	visit_if((visitor)->order == MCC_AST_VISIT_PRE_ORDER, node, callback, visitor)

#define visit_if_post_order(node, callback, visitor) \
	visit_if((visitor)->order == MCC_AST_VISIT_POST_ORDER, node, callback, visitor)


#define visit_for(order, max)                                                  \
	for (int i = (order == MCC_AST_VISIT_PRE_ORDER) ? 0 : (max - 1);       \
	     (order == MCC_AST_VISIT_PRE_ORDER) ? i < max : i >= 0;            \
	     (order == MCC_AST_VISIT_PRE_ORDER) ? i++ : i--)



void mcc_ast_visit_expression(struct mcc_ast_expression *expression, struct mcc_ast_visitor *visitor)
{
    assert(expression);
    assert(visitor);


    switch (expression->type) {

        case MCC_AST_EXPRESSION_TYPE_BINARY_OP:
            visit_if_pre_order(expression, visitor->expression_binary_op, visitor);
            mcc_ast_visit_expression(expression->lhs, visitor);
            mcc_ast_visit_expression(expression->rhs, visitor);
            visit_if_post_order(expression, visitor->expression_binary_op, visitor);
            break;

        case MCC_AST_EXPRESSION_TYPE_LITERAL:
            visit_if_pre_order(expression->literal, visitor->literal, visitor);
            mcc_ast_visit_literal(expression->literal, visitor);
            visit_if_post_order(expression->literal, visitor->literal, visitor);
            break;

        case MCC_AST_EXPRESSION_TYPE_UNARY_OP:
            visit_if_pre_order(expression, visitor->expression_unary_op, visitor);
            //mcc_ast_visit(expression, visitor);
            mcc_ast_visit_expression(expression-> unary_expression, visitor);
            visit_if_post_order(expression, visitor->expression_unary_op, visitor);
            break;


        case MCC_AST_EXPRESSION_TYPE_PARENTH:
            visit_if_pre_order(expression, visitor->expression, visitor);
            mcc_ast_visit_expression(expression, visitor);
            visit_if_post_order(expression, visitor->expression, visitor);
            break;

        case MCC_AST_EXPRESSION_TYPE_IDENTIFIER:
            visit_if_pre_order(expression, visitor-> expression_identifier, visitor);
            if (expression->id_expr)
            {//mcc_ast_visit(expression->identifier, visitor);
                //visit_if_post_order(expression->identifier, visitor->identifier, visitor);

                // visit_if_pre_order(expression->id_expr, visitor->expression_identifier, visitor);
                mcc_ast_visit_expression(expression->id_expr, visitor);
            }
            visit_if_post_order(expression->id_expr, visitor->expression_identifier,
                                visitor);
            break;

        case MCC_AST_EXPRESSION_TYPE_CALL_EXPR:
            visit_if_pre_order(expression, visitor->expression_call, visitor);
            for(int i =0; i < expression->arguments->argument_count; i++) {
                mcc_ast_visit_expression(expression->arguments->arg_expr[i], visitor);
            }
            visit_if_post_order(expression, visitor->expression_call, visitor);
            break;


    }


    //visit_if_post_order(single_expr, visitor->single_expr, visitor);
}


/*void mcc_ast_visit_identifier(struct mcc_ast_identifier *identifier, struct mcc_ast_visitor *visitor)
{
    assert(identifier);
    assert(visitor);

    visit(identifier, visitor -> identifier, visitor);
}*/


void mcc_ast_visit_literal(struct mcc_ast_literal *literal, struct mcc_ast_visitor *visitor)
{
    assert(literal);
    assert(visitor);

    visit_if_pre_order(literal, visitor->literal, visitor);

    switch (literal->type) {

        case MCC_AST_LITERAL_TYPE_INT:
            visit(literal, visitor->literal_int, visitor);
            break;

        case MCC_AST_LITERAL_TYPE_FLOAT:
            visit(literal, visitor->literal_float, visitor);
            break;

        case  MCC_AST_LITERAL_TYPE_STRING:
            visit(literal, visitor->literal_string, visitor);
            break;
        case MCC_AST_LITERAL_TYPE_BOOL:
            visit(literal, visitor->literal_bool, visitor);
            break;

    }

    visit_if_post_order(literal, visitor->literal, visitor);
}
/*void mcc_ast_visit_arguments(struct mcc_ast_arguments *arguments, struct mcc_ast_visitor *visitor)
{

    assert(arguments);
    assert(visitor);

    visit_if_pre_order(arguments->arg_expr, visitor->expression, visitor);
   for(int i = 0; i < arguments->argument_count; i++){

      mcc_ast_visit_expression(arguments->arg_expr, visitor);

  }

    visit_if_post_order(arguments->arg_expr, visitor->expression, visitor);
}*/

void mcc_ast_visit_declaration(struct mcc_ast_declaration *declaration, struct mcc_ast_visitor *visitor)
{

    assert(declaration);
    assert(visitor);

    visit_if_pre_order(declaration->decl_lit, visitor->literal, visitor);
    if(declaration->decl_lit){
        visit(declaration->decl_lit, visitor->literal, visitor);
    }
    visit(declaration->ident, visitor->identifier, visitor);
    visit_if_post_order(declaration->ident, visitor->identifier, visitor);
}


void mcc_ast_visit_assignment(struct mcc_ast_assignment *assignment, struct mcc_ast_visitor *visitor){

    assert(assignment);
    assert(visitor);

    visit(assignment->identifier, visitor->identifier, visitor);
    if (assignment->ass_counter != NULL) {
        visit_if_pre_order(assignment->ass_counter, visitor->expression, visitor);

        mcc_ast_visit_expression(assignment->ass_counter, visitor);
        visit_if_post_order(assignment->ass_counter, visitor->expression, visitor);
    }
    visit_if_pre_order(assignment->ass_counter, visitor->expression, visitor);
    mcc_ast_visit_expression(assignment->ass_counter->expression, visitor);
    visit_if_post_order(assignment->ass_counter->expression, visitor->expression, visitor);
 }


void mcc_ast_visit_statement(struct mcc_ast_statement *statement, struct mcc_ast_visitor *visitor)
{


    assert(visitor);

    visit_if_pre_order(statement, visitor->statement, visitor);

    switch (statement->type) {

        case MCC_AST_STATEMENT_TYPE_IF:
            visit_if_pre_order(statement, visitor->statement_if, visitor);

            mcc_ast_visit_expression(statement->if_cond, visitor);
            mcc_ast_visit_statement(statement->if_stmt, visitor);
            if(statement->else_stmt){

                mcc_ast_visit_statement(statement->else_stmt, visitor);
            }

            visit_if_post_order(statement, visitor->statement_if, visitor);
            break;


        case MCC_AST_STATEMENT_TYPE_WHILE:
            visit_if_pre_order(statement, visitor->statement_while, visitor);

            mcc_ast_visit_expression(statement->while_cond, visitor);
            mcc_ast_visit_statement(statement->while_stmt, visitor);

            visit_if_post_order(statement, visitor->statement_while, visitor);
            break;

        case MCC_AST_STATEMENT_TYPE_DECL:
            visit_if_pre_order(statement, visitor->statement_declaration, visitor);
            visit(statement, visitor->statement_declaration, visitor);

            mcc_ast_visit_declaration(statement->declaration, visitor);
            visit_if_post_order(statement, visitor->statement_declaration, visitor);

          /*  visit_if_pre_order(statement->declaration->decl_lit, visitor->literal, visitor);
            if(statement->declaration->decl_lit){
                visit(statement->declaration->decl_lit, visitor->literal, visitor);
            }

            visit(statement->declaration->ident, visitor->identifier, visitor);
            visit_if_post_order(statement->declaration->ident, visitor->identifier, visitor);*/
            break;

        case MCC_AST_STATEMENT_TYPE_ASSIGN:
            visit_if_pre_order(statement, visitor->statement_assignment, visitor);
            mcc_ast_visit_assignment(statement->assignment, visitor);

           /* mcc_ast_visit_identifier(statement->expression->identifier, visitor);
            if (statement->assignment) {
                mcc_ast_visit_expression(statement->assignment->array_assign.assign_expr, visitor);
                mcc_ast_visit_expression(statement->assignment->single_assign.rhs, visitor);
            }*/
            visit_if_post_order(statement, visitor->statement_assignment, visitor);

            break;

        case MCC_AST_STATEMENT_TYPE_EXPRESSION:
            visit_if_pre_order(statement, visitor->statement, visitor);

            mcc_ast_visit_expression(statement->expression, visitor);

            visit_if_post_order(statement, visitor->statement, visitor);

            break;


        case MCC_AST_STATEMENT_TYPE_COMPOUND_STMT:


           visit_if_pre_order(statement, visitor->statement_compound, visitor);
             mcc_ast_visit_statement(statement , visitor);
            for(int i = 0; i < statement->num_stmt; i++){
                mcc_ast_visit_statement(statement->stmt[i],visitor);
            }
            visit(statement, visitor->close_statement_compound, visitor);
            visit_if_post_order(statement, visitor->statement_compound, visitor);
            break;

        case (MCC_AST_STATEMENT_TYPE_RET):
            visit_if_pre_order(statement, visitor->statement_ret, visitor);

            visit(statement->ret_expr, visitor->expression, visitor);
            visit_if_post_order(statement, visitor->statement_ret, visitor);
            break;

    }

    visit_if_post_order(statement, visitor->statement, visitor);
}


void mcc_ast_visit_parameter(struct mcc_ast_parameter *parameter, struct mcc_ast_visitor *visitor)

{

    assert(visitor);

    if(parameter){

        visit_if_pre_order(parameter, visitor->parameter, visitor);
        for (int i = 0; i < parameter->param_len; i++) {
            mcc_ast_visit_statement(parameter->decl_parameter[i], visitor);
        }

        visit_if_post_order(parameter, visitor->parameter, visitor );

    }


}

void mcc_ast_visit_function(struct mcc_ast_function *function, struct mcc_ast_visitor *visitor)
{
    assert(visitor);
    assert(function);

     visit_for( visitor->order, function->max_function){

     struct mcc_ast_function_def *function_def = function->fun_def;

    visit_if_pre_order(function_def, visitor->function_def, visitor);

    mcc_ast_visit_function(function, visitor);

    visit_if_post_order(function_def, visitor->function_def, visitor);
    }
}

void mcc_ast_visit_function_def(struct mcc_ast_function_def *function_def, struct mcc_ast_visitor *visitor){

    assert(function_def);
    assert(visitor);

    visit(function_def->identifier,visitor->identifier,  visitor);
    visit_if_pre_order(function_def->parameter, visitor->parameter, visitor);
    mcc_ast_visit_parameter(function_def->parameter, visitor);
    visit_if_post_order(function_def->parameter, visitor->parameter, visitor);

    visit_if_pre_order(function_def->compound_stmt, visitor->statement_compound, visitor);
    mcc_ast_visit_statement(function_def->compound_stmt, visitor);
    visit(function_def->compound_stmt, visitor->close_statement_compound, visitor);
    visit_if_post_order(function_def->compound_stmt, visitor->statement_compound, visitor);

    visit(function_def, visitor->close_function_def, visitor);


}

