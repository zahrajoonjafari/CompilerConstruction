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


void mcc_ast_visit_expression(struct mcc_ast_expression *expression, struct mcc_ast_visitor *visitor)
 {
    assert(expression);
    assert(visitor);


    switch (expression->type)
    {


		case MCC_AST_EXPRESSION_TYPE_SINGLE:
			visit_if_pre_order(expression, visitor->single_expr,visitor);
			mcc_ast_visit_single_expression(expression->single_expr, visitor);
			visit_if_post_order(expression, visitor->single_expr, visitor);
			break;

        case MCC_AST_EXPRESSION_TYPE_BINARY_OP:

            visit_if_pre_order(expression, visitor->single_expr,visitor);
             mcc_ast_visit_single_expression(expression->lhs, visitor);
            visit_if_post_order(expression, visitor->single_expr, visitor);


            visit_if_pre_order(expression, visitor->single_expr, visitor);
            mcc_ast_visit_expression(expression->rhs, visitor);
            visit_if_post_order(expression, visitor->single_expr, visitor);
            break;
     }



 }


void mcc_ast_visit_single_expression(struct mcc_ast_single_expression *single_expr, struct mcc_ast_visitor *visitor)
{
	assert(single_expr);
	assert(visitor);


	switch (single_expr->type) {

		case MCC_AST_SINGLE_EXPRESSION_TYPE_LITERAL:
			visit_if_pre_order(single_expr->literal, visitor->literal, visitor);
			mcc_ast_visit(single_expr->literal, visitor);
			visit_if_post_order(single_expr->literal, visitor->literal, visitor);
			break;

		case MCC_AST_SINGLE_EXPRESSION_TYPE_UNARY_OP:
			visit_if_pre_order(single_expr->expression, visitor->single_expr_unary_op, visitor);
			mcc_ast_visit(single_expr, visitor);
			mcc_ast_visit(single_expr-> unary_expression, visitor);
			visit_if_post_order(single_expr->unary_expression, visitor->expression, visitor);
			break;


		case MCC_AST_SINGLE_EXPRESSION_TYPE_PARENTH:
			visit_if_pre_order(single_expr->expression, visitor->expression, visitor);
			mcc_ast_visit(single_expr, visitor);
			visit_if_post_order(single_expr->expression, visitor->expression, visitor);
			break;

		case MCC_AST_SINGLE_EXPRESSION_TYPE_IDENTIFIER:
			visit_if_pre_order(single_expr->expression, visitor->single_expr_identifier, visitor);
			//if (expression->id_expr)
			mcc_ast_visit(single_expr->identifier, visitor);
			mcc_ast_visit(single_expr->id_expr, visitor);
			visit_if_post_order(single_expr->expression, visitor->single_expr,
								visitor);
			break;

		case MCC_AST_SINGLE_EXPRESSION_TYPE_CALL_EXPR:
			visit_if_pre_order(single_expr->expression, visitor->single_expr_identifier, visitor);
			mcc_ast_visit(single_expr->identifier_call , visitor);
			mcc_ast_visit(single_expr, visitor);
			visit_if_post_order(single_expr->expression, visitor ->single_expr_call, visitor);
            break;


	}


	//visit_if_post_order(single_expr, visitor->single_expr, visitor);
}


void mcc_ast_visit_identifier(struct mcc_ast_identifier *identifier, struct mcc_ast_visitor *visitor)
{
	assert(identifier);
	assert(visitor);

	visit(identifier, visitor -> identifier, visitor);
}


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


void mcc_ast_visit_declaration(struct mcc_ast_declaration *declaration, struct mcc_ast_visitor *visitor)
{

    assert(declaration);
    assert(visitor);

    visit_if_pre_order(declaration->decl_lit, visitor->literal, visitor);
    if(declaration->decl_lit){
        mcc_ast_visit(declaration->decl_lit, visitor);
    }

    mcc_ast_visit(declaration->ident, visitor);
    visit_if_post_order(declaration->ident, visitor->identifier, visitor);
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

                visit_if_post_order(statement, visitor->statement_if, visitor);
                break;


            case MCC_AST_STATEMENT_TYPE_WHILE:
                visit_if_pre_order(statement, visitor->statement_while, visitor);

                mcc_ast_visit_expression(statement->while_cond, visitor);
                mcc_ast_visit_statement(statement->while_stmt, visitor);

                visit_if_post_order(statement, visitor->statement_while, visitor);
                break;

            case MCC_AST_STATEMENT_TYPE_DECL:
                visit_if_pre_order(statement, visitor->statement_declaration,
                                   visitor);
                mcc_ast_visit_declaration(statement->declaration, visitor);
                visit_if_post_order(statement, visitor->statement_declaration,
                                    visitor);
                break;

            case MCC_AST_STATEMENT_TYPE_ASSIGN:
                visit_if_pre_order(statement, visitor->statement_assignment,
                                   visitor);
                mcc_ast_visit_identifier(statement->assignment->identifier, visitor);
                if (statement->assignment) {
                    mcc_ast_visit_expression(statement->assignment->array_assign.assign_expr, visitor);
                }
                mcc_ast_visit_expression(statement->assignment->single_assign.rhs, visitor);
                visit_if_post_order(statement, visitor->statement_assignment,
                                    visitor);
                break;

            case MCC_AST_STATEMENT_TYPE_EXPRESSION:
                visit_if_pre_order(statement, visitor->statement,
                                   visitor);
                mcc_ast_visit_expression(statement->expression, visitor);

                visit_if_post_order(statement, visitor->statement,
                                    visitor);
                break;


            case MCC_AST_STATEMENT_TYPE_COMPOUND_STMT:

                visit_if_pre_order(statement, visitor->statement_compound, visitor);

                for(int i = 0; i < statement->num_stmt; i++){
                    mcc_ast_visit_statement(statement->stmt[i],visitor);
                }

                visit_if_post_order(statement, visitor->statement, visitor);
                break;

            case (MCC_AST_STATEMENT_TYPE_RET):
                visit_if_pre_order(statement, visitor->statement_ret, visitor);
                mcc_ast_visit(statement->ret_expr, visitor);
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
        for (int i = 0; i < parameter->param_size; i++) {
            mcc_ast_visit_declaration(parameter->decl_parameter[i], visitor);
        }

         visit_if_post_order(parameter, visitor->parameter, visitor );

    }


}

void mcc_ast_visit_function(struct mcc_ast_function *function, struct mcc_ast_visitor *visitor)
{
    assert(visitor);
    assert(function);



    visit_if_pre_order(function, visitor->function_def, visitor);
      if(function->function_def->identifier){mcc_ast_visit_identifier(function->function_def->identifier, visitor);}
      if(function->function_def->parameter){ mcc_ast_visit_parameter(function->function_def->parameter, visitor);}
      if(function->statement){mcc_ast_visit(function->statement, visitor);}

    visit_if_post_order(function->statement, visitor->statement_compound, visitor);
}



void mcc_ast_visit_program(struct mcc_ast_program *program, struct mcc_ast_visitor *visitor)

{assert(program);
    assert(visitor);

    visit_if_pre_order(program, visitor->program, visitor);
    for (int i = 0; i < program->size_function; i++) {
        mcc_ast_visit_function(program->function[i], visitor);
    }
    visit_if_post_order(program, visitor->program, visitor);

}