#include "mcc/ast_visit.h"

#include <assert.h>

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
        case MCC_AST_EXPRESSION_TYPE_BINARY_OP:
          visit_if_pre_order(expression, visitor->expression,visitor);
          mcc_ast_visit(expression->lhs, visitor);
          mcc_ast_visit(expression->rhs, visitor);
          visit_if_post_order(expression, visitor->expression, visitor);

		case MCC_AST_EXPRESSION_TYPE_SINGLE:
			visit_if_pre_order(expression->single_expr, visitor->single_expr,visitor);
			mcc_ast_visit(expression->single_expr, visitor);
			visit_if_post_order(expression->single_expr, visitor->single_expr, visitor);
     }



 }


void mcc_ast_visit_single_expression(struct mcc_ast_single_expression *single_expr, struct mcc_ast_visitor *visitor)
{
	assert(single_expr);
	assert(visitor);


	switch (single_expr->type) {

		case MCC_AST_SINGLE_EXPRESSION_TYPE_LITERAL:
			visit_if_pre_order(single_expr, visitor->single_expr, visitor);
			mcc_ast_visit(single_expr->literal, visitor);
			visit_if_post_order(single_expr, visitor->single_expr, visitor);
			break;

		case MCC_AST_SINGLE_EXPRESSION_TYPE_UNARY_OP:
			visit_if_pre_order(single_expr, visitor->single_expr, visitor);
			mcc_ast_visit(single_expr-> unary_expression, visitor);
			visit_if_post_order(single_expr, visitor->single_expr, visitor);
			break;


		case MCC_AST_SINGLE_EXPRESSION_TYPE_PARENTH:
			visit_if_pre_order(single_expr, visitor->single_expr, visitor);
			mcc_ast_visit(single_expr->expression, visitor);
			visit_if_post_order(single_expr, visitor->single_expr, visitor);
			break;

		case MCC_AST_SINGLE_EXPRESSION_TYPE_IDENTIFIER:
			visit_if_pre_order(single_expr, visitor->single_expr, visitor);
			//if (expression->id_expr)
			mcc_ast_visit(single_expr->identifier, visitor);
			mcc_ast_visit(single_expr->id_expr, visitor);
			visit_if_post_order(single_expr, visitor->single_expr,
								visitor);
			break;

		case MCC_AST_SINGLE_EXPRESSION_TYPE_CALL_EXPR:
			visit_if_pre_order(single_expr, visitor->single_expr, visitor);
			mcc_ast_visit(single_expr->expression , visitor);
			mcc_ast_visit(single_expr ->arguments, visitor);
			visit_if_post_order(single_expr, visitor -> single_expr, visitor);



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

		case MCC_AST_LITERAL_TYPE_BOOL:
			visit(literal, visitor->literal_bool, visitor);

	}

	visit_if_post_order(literal, visitor->literal, visitor);
}


void mcc_ast_visit_arguments(struct mcc_ast_arguments *arguments, struct mcc_ast_visitor *visitor)
{


	assert(arguments);
	assert(visitor);

	visit(arguments, visitor->arguments, visitor);

}