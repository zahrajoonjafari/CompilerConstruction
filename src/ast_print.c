#include "mcc/ast_print.h"

#include <assert.h>
#include <stdio.h>
#include "mcc/ast_visit.h"
#include <../include/mcc/ast.h>

const char *mcc_ast_print_binary_op(enum mcc_ast_binary_op op)



{
	switch (op) {
	case MCC_AST_BINARY_OP_ADD:
		return "+";
	case MCC_AST_BINARY_OP_SUB:
		return "-";
	case MCC_AST_BINARY_OP_MUL:
		return "*";
	case MCC_AST_BINARY_OP_DIV:
		return "/";
	case MCC_AST_BINARY_OP_EQUALS:
			return "==";
	case MCC_AST_BINARY_OP_NOT_EQUALS:
			return "!=";
	case MCC_AST_BINARY_OP_GREATER:
			return ">";
	case MCC_AST_BINARY_OP_GREATER_EQ:
			return ">=";
	case MCC_AST_BINARY_OP_LESS:
			return "<";
	case MCC_AST_BINARY_OP_LESS_EQ:
			return "<=";
	case MCC_AST_BINARY_OP_AND:
			return "&&";
	case MCC_AST_BINARY_OP_OR:
			return "||";
	}

	return "unknown op";
}


const char *mcc_ast_print_unary_op(enum mcc_ast_unary_op op)

{




	switch (op) {
		case MCC_AST_UNARY_OP_MINUS: return "-";
		case MCC_AST_UNARY_OP_NOT: return "!";
	}

	return "unknown op";
}


const char *mcc_ast_print_data_type(enum mcc_ast_data_type dt)
{
	switch (dt) {
		case MCC_AST_DATA_TYPE_INT:
			return "INT";
		case MCC_AST_DATA_TYPE_STRING:
			return "STRING";
		case MCC_AST_DATA_TYPE_BOOL:
			return "BOOL";
		case MCC_AST_DATA_TYPE_FLOAT:
			return "FLOAT";
		case MCC_AST_DATA_TYPE_VOID:
			return "VOID";
	}

	return "unknown data type";
}
// ---------------------------------------------------------------- DOT Printer

#define LABEL_SIZE 64

static void print_dot_begin(FILE *out)
{
	assert(out);

	fprintf(out, "digraph \"AST\" {\n"
	             "\tnodesep=0.6\n");
}

static void print_dot_end(FILE *out)
{
	assert(out);

	fprintf(out, "}\n");
}

static void print_dot_node(FILE *out, const void *node, const char *label)
{
	assert(out);
	assert(node);
	assert(label);

	fprintf(out, "\t\"%p\" [shape=box, label=\"%s\"];\n", node, label);
}

static void print_dot_edge(FILE *out, const void *src_node, const void *dst_node, const char *label)
{
	assert(out);
	assert(src_node);
	assert(dst_node);
	assert(label);

	fprintf(out, "\t\"%p\" -> \"%p\" [label=\"%s\"];\n", src_node, dst_node, label);
}

//-----------------------------------------------------------------------Literal


static void print_dot_literal_int(struct mcc_ast_literal *literal, void *data)
{
	assert(literal);
	assert(data);

	char label[LABEL_SIZE] = {0};
	snprintf(label, sizeof(label), "%ld", literal->i_value);

	FILE *out = data;
	print_dot_node(out, literal, label);
}

static void print_dot_literal_float(struct mcc_ast_literal *literal, void *data)
{
	assert(literal);
	assert(data);

	char label[LABEL_SIZE] = {0};
	snprintf(label, sizeof(label), "%f", literal->f_value);

	FILE *out = data;
	print_dot_node(out, literal, label);
}

static void print_dot_literal_string(struct mcc_ast_literal *literal,
									 void *data)
{
	assert(literal);
	assert(data);

	char label[LABEL_SIZE] = { 0 };
	snprintf(label, sizeof(label), "%s", literal->s_value);

	FILE *out = data;
	print_dot_node(out, literal, label);
}

static void print_dot_literal_bool(struct mcc_ast_literal *literal, void *data)
{
	assert(literal);
	assert(data);

	char label[LABEL_SIZE] = { 0 };
	snprintf(label, sizeof(label), "%s", literal->b_value ? "true" : "false");

	FILE *out = data;
	print_dot_node(out, literal, label);
}



// ---------------------------------------------------------------- Expressions

static void print_dot_expression(struct mcc_ast_expression *expression, void *data)
{
	assert(expression);
	assert(data);

	FILE *out = data;

	if()
	print_dot_node(out, expression , "expr: lit");
	print_dot_edge(out, expression , expression->single_expr->literal, "literal");
}

static void print_dot_expression_binary_op(struct mcc_ast_expression *expression, void *data)
{
	assert(expression);
	assert(data);

	char label[LABEL_SIZE] = {0};
	snprintf(label, sizeof(label), "expr: %s", mcc_ast_print_binary_op(expression->op));

	FILE *out = data;
	print_dot_node(out, expression, label);
	print_dot_edge(out, expression, expression->lhs, "lhs");
	print_dot_edge(out, expression, expression->rhs, "rhs");
}

static void print_dot_expression_unary_op(struct mcc_ast_expression *expression,
										  void *data)
{
	assert(expression);
	assert(data);

	char label[LABEL_SIZE] = { 0 };
	snprintf(label, sizeof(label), "expr: %s",
			 mcc_ast_print_unary_op(expression->single_expr->unary_op));

	FILE *out = data;
	print_dot_node(out, expression, label);
	print_dot_edge(out, expression, expression->single_expr->unary_expression, "expr");
}


static void print_dot_expression_identifier(struct mcc_ast_expression *expression,
								void *data)
{
	assert(expression);
	assert(data);

	FILE *out = data;
	print_dot_node(out, expression, "expr: id");
	print_dot_edge(out, expression, expression->single_expr->identifier, "identifier");
}


static void print_dot_expression_parenth(struct mcc_ast_expression *expression, void *data)
{
	assert(expression);
	assert(data);

	FILE *out = data;
	print_dot_node(out, expression, "( )");
	print_dot_edge(out, expression, expression->single_expr , "expression");
}




//------------------------------------------------ Setup an AST Visitor for printing.
static struct mcc_ast_visitor print_dot_visitor(FILE *out)
{
	assert(out);

	return (struct mcc_ast_visitor){
	    .traversal = MCC_AST_VISIT_DEPTH_FIRST,
	    .order = MCC_AST_VISIT_PRE_ORDER,

	    .userdata = out,

			.expression = print_dot_expression,
			.single_expression = print_dot_expression_single,
			.call_expression = print_dot_call_expr,



		.literal_int = print_dot_literal_int,
		.literal_float = print_dot_literal_float,
		.literal_string = print_dot_literal_string,
		.literal_bool = print_dot_literal_bool,


	};
}

void mcc_ast_print_dot_expression(FILE *out, struct mcc_ast_expression *expression)
{
	assert(out);
	assert(expression);

	print_dot_begin(out);

	struct mcc_ast_visitor visitor = print_dot_visitor(out);
	mcc_ast_visit(expression, &visitor);

	print_dot_end(out);
}

void mcc_ast_print_dot_identifier(FILE *out, struct mcc_ast_identifier *identifier)
{
    assert(out);
    assert(identifier);

    print_dot_begin(out);

    struct mcc_ast_visitor visitor = print_dot_visitor(out);
    mcc_ast_visit(identifier, &visitor);

    print_dot_end(out);
}

void mcc_ast_print_dot_literal(FILE *out, struct mcc_ast_literal *literal)
{
	assert(out);
	assert(literal);

	print_dot_begin(out);

	struct mcc_ast_visitor visitor = print_dot_visitor(out);
	mcc_ast_visit(literal, &visitor);

	print_dot_end(out);
}
