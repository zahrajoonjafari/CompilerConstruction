// AST Print Infrastructure
//
// This module provides basic printing infrastructure for the AST data
// structure. The DOT printer enables easy visualisation of an AST.

#ifndef MCC_AST_PRINT_H
#define MCC_AST_PRINT_H

#include <stdio.h>

#include "mcc/ast.h"

const char *mcc_ast_print_unary_op(enum mcc_ast_unary_op op);

const char *mcc_ast_print_binary_op(enum mcc_ast_binary_op op);


// ---------------------------------------------------------------- DOT Printer

void mcc_ast_print_dot_expression(FILE *out, struct mcc_ast_expression *expression);

void mcc_ast_print_dot_function(FILE *out, struct mcc_ast_function *function);

void mcc_ast_print_dot_literal(FILE *out, struct mcc_ast_literal *literal);

void mcc_ast_print_dot_program(FILE *out, struct mcc_ast_program *program);

void mcc_ast_print_dot_identifier(FILE *out, struct mcc_ast_identifier *identifier);

// clang-format off

#define mcc_ast_print_dot(out, x) _Generic((x), \
		struct mcc_ast_expression *: 	mcc_ast_print_dot_expression, \
		struct mcc_ast_identifier *:    mcc_ast_print_dot_identifier, \
		struct mcc_ast_literal *:    	mcc_ast_print_dot_literal, \
		struct mcc_ast_function *: 		mcc_ast_print_dot_function, \
		struct mcc_ast_program *: 		mcc_ast_print_dot_program \
	)(out, x)

// clang-format on

#endif // MCC_AST_PRINT_H