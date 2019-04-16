%define api.prefix {mcc_parser_}

%define api.pure full
%lex-param   {void *scanner}
%parse-param {void *scanner} {struct mcc_ast_expression** result}

%define parse.trace
%define parse.error verbose

%code requires {
#include "mcc/parser.h"
#include <stdbool.h>
}

%{
#include <string.h>
#include <stdbool.h>
int mcc_parser_lex();
void mcc_parser_error();

#define loc(ast_node, ast_sloc) \
	(ast_node)->node.sloc.start_col = (ast_sloc).first_column;

%}

%define api.value.type union
%define api.token.prefix {TK_}

%locations

%token END 0 "EOF"

%token <long>   INT_LITERAL   "integer literal"
%token <double> FLOAT_LITERAL "float literal"
%token <char*>   STRING_LITERAL   "string literal"
%token <bool> BOOL_LITERAL "bool literal"

%token <char*> IDENTIFIER "identifier"


%token LPARENTH "("
%token RPARENTH ")"

%token LBRACKET "["
%token RBRACKET "]"

%token LBRACE "{"
%token RBRACE "}"

//%left PLUS MINUS
//%left ASTER SLASH

%token PLUS  "+"
%token MINUS "-"
%token ASTER "*"
%token SLASH "/"



%token ASSIGNMENT "="

%token LESS "<"
%token GREATER ">"
%token LESS_EQ "<="
%token GREATER_EQ ">="
%token EQUALS "=="
%token NOT_EQUALS "!="

%token SEMICOLON ";"
%token COMMA ","

%token NOT "!"

%token AND "&&"
%token OR "||"


%token BOOL_TYPE "bool"
%token INT_TYPE "int"
%token FLOAT_TYPE "float"
%token STRING_TYPE "string"
%token VOID"void"

%token IF "if"
%token ELSE "else"
%token WHILE "while"
%token FOR "for"
%token RETURN "return"


//%type <enum mcc_ast_data_type> type
%type <enum mcc_ast_binary_op> binary_op
%type <enum mcc_ast_unary_op> unary_op

%type <struct mcc_ast_single_expression *> single_expression
%type <struct mcc_ast_arguments *> arguments
%type <struct mcc_ast_expression *> expression call_expression
%type <struct mcc_ast_literal *> literal
//%type <struct mcc_ast_identifier *> identifier

%start toplevel

%%

toplevel : expression { *result = $1; }
         ;



single_expression  : literal                           { $$ = mcc_ast_new_single_expression_literal($1); loc($$, @1); }
                   | LPARENTH expression RPARENTH      { $$ = mcc_ast_new_single_expression_parenth($2); loc($$, @2); }
		  // | expression binary_op expression   { $$ = mcc_ast_new_single_expression_binary_op($2, $1, $3); loc($$, @2); }
		   | IDENTIFIER                        { $$ = mcc_ast_new_single_expression_identifier($1, NULL); loc($$,@1);}
 		   | unary_op expression               { $$ = mcc_ast_new_single_expression_unary_op($1, $2); loc($$, @1); }
                   | call_expression                   { $$ = mcc_ast_new_single_expression_call_expr($1, NULL);}
                   ;

expression     : single_expression binary_op expression  { $$ = mcc_ast_new_expression_binary_op($2, $1, $3); }
                | single_expression                       { $$ = mcc_ast_new_expression_single($1); }
                ;


//identifier   : identifier       { $$ = mcc_ast_new_identifier($1); }

call_expression     :  IDENTIFIER LPARENTH arguments RPARENTH  { $$ = mcc_ast_new_expression_call_expr($1, $3); }


literal : INT_LITERAL   { $$ = mcc_ast_new_literal_int($1);   loc($$, @1); }
        | FLOAT_LITERAL { $$ = mcc_ast_new_literal_float($1); loc($$, @1); }
        | STRING_LITERAL { $$ = mcc_ast_new_literal_string($1); loc($$, @1);}
        | BOOL_LITERAL   { $$ = mcc_ast_new_literal_bool($1); loc($$,@1); }
        ;



binary_op :  PLUS        { $$ = MCC_AST_BINARY_OP_ADD; }
          |  MINUS       { $$ = MCC_AST_BINARY_OP_SUB; }
          |  ASTER       { $$ = MCC_AST_BINARY_OP_MUL; }
          |  SLASH       { $$ = MCC_AST_BINARY_OP_DIV; }
          |  LESS        { $$ = MCC_AST_BINARY_OP_LESS; }
          |  GREATER     { $$ = MCC_AST_BINARY_OP_GREATER; }
          |  LESS_EQ     { $$ = MCC_AST_BINARY_OP_LESS_EQ; }
          |  GREATER_EQ  { $$ = MCC_AST_BINARY_OP_GREATER_EQ; }
          |  AND         { $$ = MCC_AST_BINARY_OP_AND; }
          |  OR          { $$ = MCC_AST_BINARY_OP_OR; }
          |  EQUALS      { $$ = MCC_AST_BINARY_OP_EQUALS; }
          |  NOT_EQUALS  { $$ = MCC_AST_BINARY_OP_NOT_EQUALS; }
          ;

unary_op        : MINUS             { $$ = MCC_AST_UNARY_OP_MINUS; }
                | NOT               { $$ = MCC_AST_UNARY_OP_NOT; }
                ;


arguments        : expression 					{ $$ = mcc_ast_new_arguments($1); loc($$,@1);}
		 | arguments COMMA expression 	{$$ = mcc_ast_new_arguments_expr($1, $3); loc($$, @1);}
           	 ;






/*type                    :  INT_TYPE { $$ = MCC_AST_DATA_TYPE_INT; }
                        | FLOAT_TYPE { $$ = MCC_AST_DATA_TYPE_FLOAT; }
                        | STRING_TYPE { $$ = MCC_AST_DATA_TYPE_STRING; }
                	| BOOL_TYPE { $$ = MCC_AST_DATA_TYPE_BOOL; }
                       // | VOID_TYPE { $$ = MCC_AST_DATA_TYPE_VOID;}
                    ;

*/
%%

#include <assert.h>

#include "scanner.h"
#include "utils/unused.h"

void mcc_parser_error(struct MCC_PARSER_LTYPE *yylloc, yyscan_t *scanner, const char *msg)
{
	// TODO
	UNUSED(yylloc);
	UNUSED(scanner);
	UNUSED(msg);
}

struct mcc_parser_result mcc_parse_string(const char *input)
{
	assert(input);

	FILE *in = fmemopen((void *)input, strlen(input), "r");
	if (!in) {
		return (struct mcc_parser_result){
		    .status = MCC_PARSER_STATUS_UNABLE_TO_OPEN_STREAM,
		};
	}

	struct mcc_parser_result result = mcc_parse_file(in);

	fclose(in);

	return result;
}

struct mcc_parser_result mcc_parse_file(FILE *input)
{
	assert(input);

	yyscan_t scanner;
	mcc_parser_lex_init(&scanner);
	mcc_parser_set_in(input, scanner);

	struct mcc_parser_result result = {
	    .status = MCC_PARSER_STATUS_OK,
	};

	if (yyparse(scanner, &result.expression) != 0) {
		result.status = MCC_PARSER_STATUS_UNKNOWN_ERROR;
	}

	mcc_parser_lex_destroy(scanner);

	return result;
}
