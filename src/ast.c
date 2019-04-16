

#include "mcc/ast.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>



// ---------------------------------------------------------------- Expressions

struct mcc_ast_expression *mcc_ast_new_expression_binary_op(enum mcc_ast_binary_op op,
															struct  mcc_ast_single_expression *lhs,
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

struct mcc_ast_expression *mcc_ast_new_expression_single(struct mcc_ast_single_expression *single_expr) {

	assert(single_expr);

	struct mcc_ast_expression *expr = malloc(sizeof(expr));

	if (!expr) {

		return NULL;
	}

	expr->type = MCC_AST_EXPRESSION_TYPE_SINGLE;
	expr->single_expr = single_expr;

	return  expr;

}


//--------------------------------------Single

struct mcc_ast_single_expression *mcc_ast_new_single_expression_literal(struct mcc_ast_literal *literal)
{
	assert(literal);

	struct mcc_ast_single_expression *expr = malloc(sizeof(*expr));
	if (!expr) {
		return NULL;
	}

	expr->type = MCC_AST_SINGLE_EXPRESSION_TYPE_LITERAL;
	expr->data_type = MCC_AST_DATA_TYPE_VOID;
	expr->literal=literal;
	return expr;
}





struct mcc_ast_single_expression *mcc_ast_new_single_expression_unary_op(enum mcc_ast_unary_op op,
														   struct mcc_ast_expression *rhs)
{

	assert(rhs);

	struct mcc_ast_single_expression *expr = malloc(sizeof(*expr));
	if (!expr) {
		return NULL;
	}

	expr -> type = MCC_AST_SINGLE_EXPRESSION_TYPE_UNARY_OP,
	expr -> unary_op = op;
	expr -> unary_expression = rhs;
	return expr;


}
struct mcc_ast_single_expression *mcc_ast_new_single_expression_identifier(struct  mcc_ast_identifier *identifier,
													   struct mcc_ast_expression *expression)


{

	assert(identifier);
	assert(expression);

	struct mcc_ast_single_expression *expr = malloc(sizeof(*expr));
	if(!expr) {
		return NULL;
	}

	expr -> type = MCC_AST_SINGLE_EXPRESSION_TYPE_IDENTIFIER;
	expr -> identifier = identifier;
    expr -> id_expr = expression;

	return expr;


}

struct mcc_ast_singel_expression *mcc_ast_new_single_expression_call_expr(struct mcc_ast_identifier *identifier_call,
																		  struct mcc_ast_arguments *arguments )

{
	assert(identifier_call);
	assert(arguments);
	struct mcc_ast_single_expression *expr = malloc(sizeof(*expr));
	if (!expr) {
		return NULL;
	}

	expr->type = MCC_AST_SINGLE_EXPRESSION_TYPE_CALL_EXPR;
	expr->identifier_call = identifier_call;
	expr->arguments = arguments;

	return expr;

}


struct mcc_ast_single_expression *mcc_ast_new_single_expression_parenth(struct mcc_ast_expression *expression)
{
	assert(expression);

	struct mcc_ast_single_expression *expr = malloc(sizeof(*expr));
	if (!expr) {
		return NULL;
	}

	expr->type = MCC_AST_SINGLE_EXPRESSION_TYPE_PARENTH;
	expr->expression = expression;
	return expr;
}

void mcc_ast_delete_expression(struct mcc_ast_expression *expression)

{
	assert(expression);

	switch (expression -> type) {
		case MCC_AST_EXPRESSION_TYPE_BINARY_OP:
			mcc_ast_delete_expression(expression->rhs);
			mcc_ast_delete_expression(expression->lhs);
			break;

		case MCC_AST_EXPRESSION_TYPE_SINGLE:
			mcc_ast_delete_expression(expression->single_expr->expression);
			break;


	}

	free(expression);
}

void mcc_ast_delete_single_expression(struct mcc_ast_single_expression *single_expr) {
	switch (single_expr->type) {

		case MCC_AST_SINGLE_EXPRESSION_TYPE_LITERAL:
			mcc_ast_delete_literal(single_expr->literal);
			break;

		case MCC_AST_SINGLE_EXPRESSION_TYPE_PARENTH:
			mcc_ast_delete_expression(single_expr->expression);
			break;


		case MCC_AST_SINGLE_EXPRESSION_TYPE_UNARY_OP:
			mcc_ast_delete_expression(single_expr->unary_expression);
			break;

		case MCC_AST_SINGLE_EXPRESSION_TYPE_IDENTIFIER:
			mcc_ast_delete_expression(single_expr->id_expr);
			break;

		case MCC_AST_SINGLE_EXPRESSION_TYPE_CALL_EXPR:
			mcc_ast_delete_expression(single_expr->arguments);
			mcc_ast_delete_expression(single_expr->identifier_call);
	}

	free(single_expr);
}

//-------------------------------------------------------------------- Identifier




struct  mcc_ast_identifier *mcc_ast_new_identifier(char *name)
{
    assert(name);

    struct mcc_ast_identifier *id = malloc(sizeof(id));

    if (!id) {
        return NULL;
    }

    id->name = name;
    id->data_type = MCC_AST_DATA_TYPE_VOID;


    return id;
}

void mcc_ast_delete_identifier(struct mcc_ast_identifier *identifier)
{
    assert(identifier);

    free(identifier->name);
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


	arg -> argument_count = 1;
	arg -> argument_size = 0;
    arg ->arg_expr = arg_expr;

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



}

void mcc_ast_delete_arguments(struct mcc_ast_arguments *arguments){

	assert(arguments);


	//free(arguments ->argument_size);
	//free(arguments ->argument_count);
	free(arguments ->arg_expr);

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

struct mcc_ast_literal *mcc_ast_new_literal_string(char* value)
{
	struct mcc_ast_literal *lit = malloc(sizeof(*lit));


	if (!lit) {
		return NULL;
	}

	lit -> type = MCC_AST_LITERAL_TYPE_STRING;
	lit -> s_value = value;

	return lit;
}

struct mcc_ast_literal *mcc_ast_new_literal_bool(char value)
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