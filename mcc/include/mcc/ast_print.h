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



void mcc_ast_print_dot_function_def(FILE *out, struct mcc_ast_function *function);


/*
// clang-format off

#define mcc_ast_print_dot(out, x) _Generic((x),\

	struct mcc_ast_function*: 		mcc_ast_print_dot_function_def, \
	)(out, x)

// clang-format on*/

#endif // MCC_AST_PRINT_H