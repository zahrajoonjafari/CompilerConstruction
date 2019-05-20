#include <CuTest.h>
#include <mcc/ast.h>
#include <stdlib.h>

#include "mcc/parser.h"

// Threshold for floating point comparisons.


static const double EPS = 1e-3;

/*
void BinaryOp_1(CuTest *tc)
{
   const char input[] = "4 + 3.14";
   struct mcc_parser_result result = mcc_parse_string(input);

   CuAssertIntEquals(tc, MCC_PARSER_STATUS_OK, result.status);

   struct mcc_ast_expression *expr = result.expression;

   // root

   CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_BINARY_OP, expr->type);
   CuAssertIntEquals(tc, MCC_AST_BINARY_OP_ADD, expr->op);

   // root -> lhs
   CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_LITERAL, expr->lhs->type);

   // root -> lhs -> literal
   CuAssertIntEquals(tc, MCC_AST_LITERAL_TYPE_INT, expr->lhs->literal->type);
   CuAssertIntEquals(tc, 4, expr->lhs->literal->i_value);

   // root -> rhs
   CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_LITERAL, expr->rhs->type);

   // root -> rhs -> literal
   CuAssertIntEquals(tc, MCC_AST_LITERAL_TYPE_FLOAT, expr->literal->type);
   CuAssertDblEquals(tc, 3.14, expr->literal->f_value, EPS);

   mcc_ast_delete(expr);
}


void BinaryOp_2(CuTest *tc)
{
   const char input[] = "7 + 3;";
   struct mcc_parser_result result = mcc_parse_string(input);
   CuAssertIntEquals(tc, MCC_PARSER_STATUS_OK, result.status);


   struct mcc_ast_statement  *stmt = result.statement;

   // root
   CuAssertIntEquals(tc, MCC_AST_STATEMENT_TYPE_EXPRESSION, stmt->type);
   CuAssertIntEquals(tc, MCC_AST_BINARY_OP_ADD, stmt->expression->op);

   // root -> lhs
   CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_LITERAL, stmt->expression->lhs->type);

   // root -> lhs -> literal
   CuAssertIntEquals(tc, MCC_AST_LITERAL_TYPE_INT, stmt->expression->lhs->literal->type);
   CuAssertIntEquals(tc, 1, stmt->expression->lhs->literal->i_value);

   // root -> rhs
   CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_LITERAL, stmt->expression->rhs->literal->type);

   // root -> rhs -> literal
   CuAssertIntEquals(tc, MCC_AST_LITERAL_TYPE_FLOAT, stmt->expression->literal->type);
   CuAssertIntEquals(tc, 3, stmt->expression->literal->i_value);

   mcc_ast_delete(stmt);
}

void NestedExpression_1(CuTest *tc)
{
   const char input[] = "3 * (192 + 3.14)";
   struct mcc_parser_result result = mcc_parse_string(input);

   CuAssertIntEquals(tc, MCC_PARSER_STATUS_OK, result.status);

   struct mcc_ast_expression *expr = result.expression;

   // root
   CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_BINARY_OP, expr->type);
   CuAssertIntEquals(tc, MCC_AST_BINARY_OP_MUL, expr->op);

   // root -> lhs
   CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_LITERAL, expr->lhs->type);

   // root -> lhs -> literal
   CuAssertIntEquals(tc, MCC_AST_LITERAL_TYPE_INT, expr->lhs->literal->type);
   CuAssertIntEquals(tc, 3,  expr->lhs->literal->i_value);

   // root -> rhs
   CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_PARENTH, expr->rhs->type);

   struct mcc_ast_expression *subexpr = expr->rhs->expression;

   // subexpr
   CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_BINARY_OP, subexpr->type);
   CuAssertIntEquals(tc, MCC_AST_BINARY_OP_ADD, subexpr->op);

   // subexpr -> lhs
   CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_LITERAL, subexpr->lhs->type);

   // subexpr -> lhs -> literal
   CuAssertIntEquals(tc, MCC_AST_LITERAL_TYPE_INT, subexpr->lhs->literal->type);
   CuAssertIntEquals(tc, 192, subexpr->lhs->literal->i_value);

   // subexpr -> rhs
   CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_LITERAL, subexpr->rhs->type);

   // subexpr -> rhs -> literal
   CuAssertIntEquals(tc, MCC_AST_LITERAL_TYPE_FLOAT, subexpr->rhs->literal->type);
   CuAssertDblEquals(tc, 3.14, subexpr->rhs->literal->f_value, EPS);

   mcc_ast_delete(expr);
}
*/
/* void BinaryOp_3(CuTest *tc)
{
	const char input[] = "4.6 + 5 * 7.1";
    struct mcc_parser_result result = mcc_parse_string(input);

	CuAssertIntEquals(tc, MCC_PARSER_STATUS_OK, result.status);

	struct mcc_ast_expression *expr = result.expression;
	// root
	CuAssertIntEquals(tc,MCC_AST_EXPRESSION_TYPE_BINARY_OP, expr->type);
	CuAssertIntEquals(tc, MCC_AST_BINARY_OP_ADD, expr->op);

	// root -> lhs
    CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_LITERAL, expr->lhs->type);
    CuAssertIntEquals(tc, MCC_AST_LITERAL_TYPE_FLOAT, expr->lhs->literal->type);
    CuAssertDblEquals(tc, 4.6, expr->lhs->literal->f_value, EPS);

	// root -> rhs
	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_BINARY_OP, expr->rhs->type);
	CuAssertIntEquals(tc, MCC_AST_BINARY_OP_MUL, expr->rhs->op);

	// root -> rhs -> lhs
	CuAssertIntEquals(tc, MCC_AST_LITERAL_TYPE_INT, expr->rhs->lhs->literal->type);
	CuAssertIntEquals(tc, 5, expr->rhs->lhs->literal->i_value);

	// root -> rhs -> rhs
	CuAssertIntEquals(tc, MCC_AST_LITERAL_TYPE_FLOAT, expr->rhs->rhs->literal->type);
	CuAssertDblEquals(tc, 7.1, expr->rhs->rhs->literal->f_value, EPS);
}

void BinaryOp_4(CuTest *tc)
{
      const char input[] = "1 - 2 - 3 * 4 / 5";
    struct mcc_parser_result result = mcc_parse_string(input);

    CuAssertIntEquals(tc, MCC_PARSER_STATUS_OK, result.status);

    struct mcc_ast_expression *expr = result.expression;

// root
CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_BINARY_OP, expr->type);
CuAssertIntEquals(tc, MCC_AST_BINARY_OP_SUB, expr->op);

// root -> lhs
CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_BINARY_OP, expr->lhs->type);
CuAssertIntEquals(tc, MCC_AST_BINARY_OP_SUB, expr->lhs->op);

// root -> lhs -> lhs
CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_LITERAL, expr->lhs->lhs->type);
CuAssertIntEquals(tc, MCC_AST_LITERAL_TYPE_INT, expr->lhs->lhs->literal->type);
CuAssertIntEquals(tc, 1, expr->lhs->lhs->literal->i_value);

// root -> lhs -> rhs
CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_LITERAL, expr->lhs->rhs->type);
CuAssertIntEquals(tc, MCC_AST_LITERAL_TYPE_INT, expr->lhs->rhs->literal->type);
CuAssertIntEquals(tc, 2, expr->lhs->rhs->literal->i_value);

// root -> rhs
CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_BINARY_OP, expr->rhs->type);
CuAssertIntEquals(tc, MCC_AST_BINARY_OP_DIV, expr->rhs->op);

// root -> rhs -> rhs
CuAssertIntEquals(tc, MCC_AST_LITERAL_TYPE_INT, expr->rhs->rhs->literal->type);
CuAssertIntEquals(tc, 5, expr->rhs->rhs->literal->i_value);

// root -> rhs -> lhs
CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_BINARY_OP, expr->rhs->lhs->type);
CuAssertIntEquals(tc, MCC_AST_BINARY_OP_MUL, expr->rhs->lhs->op);

// root -> rhs -> lhs -> lhs
CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_LITERAL, expr->rhs->lhs->lhs->type);
CuAssertIntEquals(tc, MCC_AST_LITERAL_TYPE_INT, expr->rhs->lhs->lhs->literal->type);
CuAssertIntEquals(tc, 3, expr->rhs->lhs->lhs->literal->i_value);

// root -> rhs -> lhs -> rhs
CuAssertIntEquals(tc, MCC_AST_LITERAL_TYPE_INT, expr->rhs->lhs->rhs->literal->type);
CuAssertIntEquals(tc, 4, expr->rhs->lhs->rhs->literal->i_value);
}

void UnaryOp_1(CuTest *tc)
{
    const char input[] = "-9";
    struct mcc_parser_result result = mcc_parse_string(input);
    CuAssertIntEquals(tc, MCC_PARSER_STATUS_OK, result.status);

    struct mcc_ast_expression *expr = result.program->function[ ]->expression;
  CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_UNARY_OP, expr->type);
  CuAssertIntEquals(tc, MCC_AST_UNARY_OP_MINUS, expr->unary_op);
  CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_LITERAL, expr->type);
}

void UnaryOp_2(CuTest *tc)
{
    const char input[] = "-j";
    struct mcc_parser_result result = mcc_parse_string(input);
    CuAssertIntEquals(tc, MCC_PARSER_STATUS_OK, result.status);
    struct mcc_ast_expression *expr = result.expression;
    CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_UNARY_OP, expr->type);
    CuAssertIntEquals(tc, MCC_AST_UNARY_OP_MINUS, expr->unary_op);
    CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_LITERAL, expr->type);
}*/

/*void UnaryOp_3(CuTest *tc) {
    const char input[] = "i = !false;";
    struct mcc_parser_result result = mcc_parse_string(input);
    CuAssertIntEquals(tc, MCC_PARSER_STATUS_OK, result.status);

    struct mcc_ast_statement *stmt = result.statement;
    CuAssertIntEquals(tc, MCC_AST_STATEMENT_TYPE_ASSIGN, stmt->type);
    CuAssertStrEquals(tc, "i", stmt->assignment->identifier->value);
    CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_UNARY_OP, stmt->assignment->type);
    CuAssertIntEquals(tc, MCC_AST_UNARY_OP_NOT, stmt->assignment->array_assign.assign_expr->unary_op);
    CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_LITERAL, stmt->assignment->array_assign.assign_expr->type);

    CuAssertTrue(tc, stmt->assignment->array_assign.assign_expr->literal->b_value);
}


void MissingClosingParenthesis_1(CuTest *tc)
{
	// TODO: fix memory leak

	const char input[] = "(42";
	struct mcc_parser_result result = mcc_parse_string(input);

	CuAssertTrue(tc, MCC_PARSER_STATUS_OK != result.status);

	struct mcc_ast_expression *expr = result.program->function[];
	CuAssertIntEquals(tc, MCC_AST_STATEMENT_TYPE_EXPRESSION, program->function->);
	CuAssertTrue(tc, NULL == result.program);
}

void SourceLocation_SingleLineColumn(CuTest *tc)
{
	const char input[] = "(8 + 192)";
	struct mcc_parser_result result = mcc_parse_string(input);

    CuAssertIntEquals(tc, MCC_PARSER_STATUS_OK, result.status);

	struct mcc_ast_expression *expr = result.expression;

	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_PARENTH, expr->type);
	CuAssertIntEquals(tc, 1, expr->node.sloc.start_col);

	CuAssertIntEquals(tc, MCC_AST_EXPRESSION_TYPE_BINARY_OP, expr->type);
	CuAssertIntEquals(tc, 2, expr->node.sloc.start_col);

	CuAssertIntEquals(tc, MCC_AST_LITERAL_TYPE_INT, expr->lhs->literal->type);
	CuAssertIntEquals(tc, 2, expr->lhs->literal->node.sloc.start_col);

	CuAssertIntEquals(tc, MCC_AST_LITERAL_TYPE_INT, expr->rhs->literal->type);
	CuAssertIntEquals(tc, 1, expr->rhs->literal->node.sloc.start_col);

	mcc_ast_delete(expr);
}
void StatementWhile(CuTest *tc)
{

    const char input[] = "while (i <= 2) { i = i + 1}" ;

    struct mcc_parser_result result = mcc_parse_string(input);

    CuAssertIntEquals(tc, MCC_PARSER_STATUS_OK, result.status);

    struct mcc_ast_statement *stmt = result.statement;

    CuAssertIntEquals(tc, MCC_AST_STATEMENT_TYPE_WHILE, stmt -> type );
    CuAssertTrue(tc, NULL == result.statement);

}
void StatementIf (CuTest *tc)
{

    const char input[] = "if (i == 2) { i = i + 1}" ;

    struct mcc_parser_result result = mcc_parse_string(input);

    CuAssertIntEquals(tc, MCC_PARSER_STATUS_OK, result.status);

    struct mcc_ast_statement *stmt = result.statement;

    CuAssertIntEquals(tc, MCC_AST_STATEMENT_TYPE_IF, stmt -> type );
    CuAssertTrue(tc, NULL == result.statement);

    struct mcc_ast_statement *expr = result.statement;

    CuAssertIntEquals(tc, MCC_AST_STATEMENT_TYPE_IF, expr -> type );
    CuAssertTrue(tc, NULL == result.expression);
}
*/

/*void StatementIfElse(CuTest *tc){


    const char input[] = "if (i != 2) i = i + 1; else i == 0;" ;

    struct mcc_parser_result result = mcc_parse_string(input);

    struct mcc_ast_statement *stmt = result.statement;

    CuAssertIntEquals(tc, MCC_AST_STATEMENT_TYPE_IF, stmt -> type );

    CuAssertTrue(tc, NULL == result.statement);

}

void StatementDeclarationInt(CuTest *tc){

 	const char input[] = "int name" ;

 	struct mcc_parser_result result = mcc_parse_string(input);

	CuAssertIntEquals(tc, MCC_PARSER_STATUS_OK, result.status);

	struct mcc_ast_statement *stmt = result.statement;

 	CuAssertIntEquals(tc, MCC_AST_STATEMENT_TYPE_DECL, stmt -> type );
 	CuAssertIntEquals(tc, stmt -> type, MCC_AST_LITERAL_TYPE_INT );
 	CuAssertStrEquals(tc, stmt ->expression->identifier->value , "name");

 }

 void StatementDeclarationFloat(CuTest *tc){

 	const char input[] = "float bar" ;

 	struct mcc_parser_result result = mcc_parse_string(input);

 	CuAssertIntEquals(tc, MCC_PARSER_STATUS_OK, result.status);

     struct mcc_ast_statement *stmt = result.statement;

 	CuAssertIntEquals(tc, MCC_AST_STATEMENT_TYPE_DECL, stmt-> type );
 	CuAssertIntEquals(tc, stmt -> type, MCC_AST_LITERAL_TYPE_FLOAT);
	CuAssertStrEquals(tc, stmt->declaration->ident->value , "bar");

 }
 void StatementDeclarationString(CuTest *tc){

 	const char input[] = "string hello" ;

 	struct mcc_parser_result result = mcc_parse_string(input);

 	CuAssertIntEquals(tc, MCC_PARSER_STATUS_OK, result.status);

 	struct mcc_ast_declaration *decl = result.statement->declaration;

 	CuAssertIntEquals(tc, MCC_AST_STATEMENT_TYPE_DECL, decl->decl_lit->type);
 	CuAssertIntEquals(tc, decl -> decl_lit->type, MCC_AST_LITERAL_TYPE_STRING);
 	CuAssertStrEquals(tc, decl->ident->value , "string");

 }

void StatementRet(CuTest *tc){

    const char input[] = "return 1" ;

    struct mcc_parser_result result = mcc_parse_string(input);

    CuAssertIntEquals(tc, MCC_PARSER_STATUS_OK, result.status);

    struct mcc_ast_statement *stmt = result.statement;

    CuAssertIntEquals(tc, MCC_AST_STATEMENT_TYPE_DECL, stmt -> type );
    CuAssertIntEquals(tc, stmt -> type, MCC_AST_LITERAL_TYPE_INT);
    CuAssertTrue(tc, NULL == result.statement);

}

void StatementAssignment(CuTest *tc) {

    const char input[] = "a = 12";

    struct mcc_parser_result result = mcc_parse_string(input);

    CuAssertIntEquals(tc, MCC_PARSER_STATUS_OK, result.status);

    struct mcc_ast_statement *stmt = result.statement;

    CuAssertIntEquals(tc, MCC_AST_STATEMENT_TYPE_ASSIGN, stmt->type);

    CuAssertIntEquals(tc, stmt -> type, MCC_AST_EXPRESSION_TYPE_IDENTIFIER );
    CuAssertStrEquals(tc, stmt ->expression->identifier->value , "a");


    CuAssertIntEquals(tc, MCC_AST_STATEMENT_TYPE_EXPRESSION, stmt->expression->literal->type);
    CuAssertIntEquals(tc, stmt->expression->literal->i_value, 12 );

    CuAssertTrue(tc, NULL != result.statement);


}*/



void ast_generation(CuTest *tc){

    const char input[] =
            "int func1() {192 + 3.14;} "
            "int func2() {192 + 3.14;}"
            "void func3() {192 + 3.14;"
            "int a;"
            "a = 10;"
            "if (a == 10)"
            "   return 10;"
            "}";

    struct mcc_parser_result  result =  mcc_parse_string(input);
    CuAssertIntEquals(tc,  MCC_PARSER_STATUS_OK ,result.status);

    struct mcc_ast_program  *program = malloc(sizeof(program));
    struct mcc_ast_function **function = result.program->function;
    for(int i = 0; i < program->max_function; i++ ){

     CuAssertTrue(tc, MCC_AST_DATA_TYPE_VOID == function[i]->function_def->data_type);
    }

    CuAssertIntEquals(tc, MCC_AST_DATA_TYPE_VOID ,function[program->max_function -1]->function_def->data_type);

    CuAssertIntEquals(tc, 4, function[program->max_function -1]->statement->max_stmt);

   CuAssertIntEquals(tc, MCC_AST_STATEMENT_TYPE_DECL, function[program->max_function -1]->statement[1].type);

     mcc_ast_delete(program);


}



void Literal (CuTest *tc)
{
   struct mcc_ast_literal *i_value = mcc_ast_new_literal_int(10);
   struct mcc_ast_literal *f_value = mcc_ast_new_literal_float(1.5);

   char test[] = "String";
   struct mcc_ast_literal *s_value = mcc_ast_new_literal_string(test);

  CuAssertTrue(tc, i_value->i_value == MCC_AST_LITERAL_TYPE_INT);
  CuAssertIntEquals(tc, i_value->i_value, 10);

   CuAssertTrue(tc, (f_value->f_value == MCC_AST_LITERAL_TYPE_FLOAT));
   CuAssertDblEquals(tc,  1.5, f_value->f_value, EPS);

   CuAssertTrue(tc,s_value->type == MCC_AST_LITERAL_TYPE_STRING);
   CuAssertStrEquals(tc, s_value->s_value, "String");

   mcc_ast_delete_literal(s_value);
    mcc_ast_delete_literal(i_value);
    mcc_ast_delete_literal(f_value);
}

#define TESTS \
	TEST(ast_generation) \
	TEST(Literal)\

#include "main_stub.inc"
#undef TESTS
