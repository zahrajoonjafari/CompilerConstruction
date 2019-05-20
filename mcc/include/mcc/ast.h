// Abstract Syntax Tree (AST)
//
// Here we define the AST data structure of the compiler. It mainly consists of
// *tagged unions* for node types and enums for operators.
//
// In addition to the node type specific members, each node features a common
// member `mmc_ast_node` which serves as a *base-class*. It holds data
// independent from the actual node type, like the source location.
//
// Also note that this makes excessive use of C11's *anonymous structs and
// unions* feature.

#ifndef MCC_AST_H
#define MCC_AST_H

#include "string.h"
#include <stdbool.h>

#include <zconf.h>


// Forward Declarations
struct mcc_ast_program;
struct mcc_ast_function;
struct mcc_ast_expression;
struct mcc_ast_literal;
struct mcc_ast_identifier;
struct mcc_ast_statement;


// ------------------------------------------------------------------- AST Node

struct mcc_ast_source_location {
    int start_line;
    int start_col;
    int end_line;
    int end_col;
};

struct mcc_ast_node {
    struct mcc_ast_source_location sloc;
};

// -------------------------------------------------------------------- Types



enum mcc_ast_data_type {
    MCC_AST_DATA_TYPE_INT,
    MCC_AST_DATA_TYPE_STRING,
    MCC_AST_DATA_TYPE_BOOL,
    MCC_AST_DATA_TYPE_FLOAT,
    MCC_AST_DATA_TYPE_VOID,
    // MCC_AST_DATA_TYPE_RETURN,

};
// ------------------------------------------------------------------ Operators

enum mcc_ast_binary_op {
    MCC_AST_BINARY_OP_ADD,
    MCC_AST_BINARY_OP_SUB,
    MCC_AST_BINARY_OP_MUL,
    MCC_AST_BINARY_OP_DIV,
    MCC_AST_BINARY_OP_AND,
    MCC_AST_BINARY_OP_OR,
    MCC_AST_BINARY_OP_EQUALS,
    MCC_AST_BINARY_OP_NOT_EQUALS,
    MCC_AST_BINARY_OP_LESS,
    MCC_AST_BINARY_OP_GREATER,
    MCC_AST_BINARY_OP_LESS_EQ,
    MCC_AST_BINARY_OP_GREATER_EQ,
};


enum mcc_ast_unary_op {
    MCC_AST_UNARY_OP_NOT,
    MCC_AST_UNARY_OP_MINUS
};
// ------------------------------------------------------------------- Literals

enum mcc_ast_literal_type {
    MCC_AST_LITERAL_TYPE_INT,
    MCC_AST_LITERAL_TYPE_FLOAT,
    MCC_AST_LITERAL_TYPE_STRING,
    MCC_AST_LITERAL_TYPE_BOOL
};

struct mcc_ast_literal {
    struct mcc_ast_node node;

    enum mcc_ast_literal_type type;
    union {
        // MCC_AST_LITERAL_TYPE_INT
        long i_value;

        // MCC_AST_LITERAL_TYPE_FLOAT
        double f_value;

        // MCC_AST_LITERAL_TYPE_STRING
        const char* s_value;

        // MCC_AST_LITERAL_TYPE_BOOL
        char b_value;
    };
};

struct mcc_ast_literal *mcc_ast_new_literal_int(long value);

struct mcc_ast_literal *mcc_ast_new_literal_float(double value);

struct mcc_ast_literal *mcc_ast_new_literal_string(const char* value);

struct mcc_ast_literal *mcc_ast_new_literal_bool(bool value);

void mcc_ast_delete_literal(struct mcc_ast_literal *literal);

//void mcc_ast_empty_node();


// ---------------------------------------------------------------- Expressions


enum mcc_ast_expression_type {
    //MCC_AST_STATEMENT_TYPE_EXPR,
    MCC_AST_EXPRESSION_TYPE_BINARY_OP,
    MCC_AST_EXPRESSION_TYPE_LITERAL,
    MCC_AST_EXPRESSION_TYPE_UNARY_OP,
    MCC_AST_EXPRESSION_TYPE_PARENTH,
    MCC_AST_EXPRESSION_TYPE_IDENTIFIER,
    MCC_AST_EXPRESSION_TYPE_CALL_EXPR,


};


struct mcc_ast_expression {

    struct mcc_ast_node node;
    enum mcc_ast_data_type data_type;

    enum mcc_ast_expression_type type;

    union {

        /* MCC_AST_EXPRESSION_TYPE_BINARY_OP */
        struct {
            enum mcc_ast_binary_op op;
            struct mcc_ast_expression *lhs;
            struct mcc_ast_expression *rhs;
        };
        // MCC_AST_EXPRESSION_TYPE_LITERAL
        struct mcc_ast_literal *literal;

        // MCC_AST_EXPRESSION_TYPE_UNARY_OP
        struct {

            enum mcc_ast_unary_op unary_op;
            struct mcc_ast_expression *unary_expression;
        };

        // MCC_AST_EXPRESSION_CALL

        struct {

            struct mcc_ast_identifier *identifier_call;
            struct mcc_ast_arguments *arguments;
            struct mcc_ast_symbol *call_symbol;

        };


        // MCC_AST_EXPRESSION_TYPE_PARENTH

        struct mcc_ast_expression *expression;

        //MCC_AST_EXPRESSION_IDENTIFIER
        struct{

            struct mcc_ast_identifier *identifier;
            struct mcc_ast_expression *id_expr;
            struct mcc_ast_symbol *ident_symbol;

        };
    };
};


struct mcc_ast_expression *mcc_ast_new_expression_call_expr(struct mcc_ast_identifier *identifier_call,
                                                            struct mcc_ast_arguments *arguments );

struct mcc_ast_expression *mcc_ast_new_expression_literal(struct mcc_ast_literal *literal);

struct mcc_ast_expression *mcc_ast_new_expression_identifier(struct mcc_ast_identifier *identifier,
                                                             struct mcc_ast_expression *id_expr);


struct mcc_ast_expression *mcc_ast_new_expression_unary_op( enum mcc_ast_unary_op unary_op,
                                                            struct mcc_ast_expression *expression);

struct mcc_ast_expression *mcc_ast_new_expression_parenth(struct mcc_ast_expression *expression);


struct mcc_ast_expression *mcc_ast_new_expression_binary_op(enum mcc_ast_binary_op op, struct mcc_ast_expression *rhs,
                                                            struct mcc_ast_expression *lhs);






void mcc_ast_delete_expression(struct mcc_ast_expression *expression);


// ---------------------------------------------------------------- Arguments



struct mcc_ast_arguments {

    struct mcc_ast_node node;

    int argument_size;
    int argument_count;
    struct mcc_ast_expression **arg_expr;

};

//struct mcc_ast_arguments *mcc_ast_new_arguments(struct mcc_ast_expression *arg_expr);

struct mcc_ast_arguments *mcc_ast_new_arguments_expr(struct mcc_ast_arguments *arguments,
                                                     struct mcc_ast_expression *arg_expr);

void mcc_ast_delete_arguments(struct mcc_ast_arguments *arguments);


//---------------------------------------------------------------- Identifier

struct mcc_ast_identifier {

    struct mcc_ast_node node;
    enum mcc_ast_data_type data_type;
     char *value;
     //har *new_value;


};

struct mcc_ast_identifier *mcc_ast_new_identifier(char *val_identifier, enum mcc_ast_data_type data_type);

void mcc_ast_delete_identifier(struct mcc_ast_identifier *identifier);
// ------------------------------------------------------------------- Declaration

struct mcc_ast_declaration {

    struct mcc_ast_node node;

    //enum mcc_ast_data_type data_type;

    struct mcc_ast_identifier *ident;

    struct mcc_ast_symbol *decl_symbol;

    struct mcc_ast_literal *decl_lit;
    //
    int decl_arr_size;  //TODO

};

struct mcc_ast_declaration *mcc_ast_new_declaration
        ( struct mcc_ast_literal *decl_lit, struct mcc_ast_identifier *ident, int decl_arr_size);

void mcc_ast_delete_declaration(struct mcc_ast_declaration *declaration);


// ------------------------------------------------------------------- Statements

enum mcc_ast_statement_type {
    MCC_AST_STATEMENT_TYPE_EXPRESSION,
    MCC_AST_STATEMENT_TYPE_IF,
    MCC_AST_STATEMENT_TYPE_WHILE,
    MCC_AST_STATEMENT_TYPE_DECL,
    MCC_AST_STATEMENT_TYPE_ASSIGN,
    MCC_AST_STATEMENT_TYPE_COMPOUND_STMT,
    MCC_AST_STATEMENT_TYPE_RET,

};



struct mcc_ast_statement {
    struct mcc_ast_node node;


    enum mcc_ast_statement_type type;
    bool returns;

    union {


        // MCC_AST_STATEMENT_RET
        struct{
        struct mcc_ast_expression  *ret_expr;
        struct mcc_ast_symbol *ret_symbol;

        };
        /*struct {
            enum mcc_ast_data_type data_type;
            struct mcc_ast_identifier *id_decl;
        };*/

        //MCC_AST_STATEMENT_IF
        struct {
            struct mcc_ast_expression *if_cond;
            struct mcc_ast_statement *if_stmt;
            struct mcc_ast_statement *else_stmt;
        };

        //MCC_AST_STATEMENT_WHILE
        struct {
            struct mcc_ast_expression *while_cond;
            struct mcc_ast_statement *while_stmt;
        };

        //MCC_AST_STATEMENT_ASSIGN
        struct mcc_ast_assignment *assignment;


        //MCC_AST_STATEMENT_DECL
        struct mcc_ast_declaration *declaration;


        //MCC_AST_STATEMENT_EXPR
        struct mcc_ast_expression *expression;


        //MCC_AST_STATEMENT_COMPOUND
        struct
        {

            int num_stmt;
            int max_stmt;
            struct mcc_ast_statement **stmt;
        };

    };
};

struct mcc_ast_statement *mcc_ast_new_statement_expression (struct mcc_ast_expression *expression);

struct mcc_ast_statement *mcc_ast_new_statement_if(struct mcc_ast_expression *if_cond,
                                                   struct mcc_ast_statement *if_stmt,
                                                   struct mcc_ast_statement *else_stmt);

struct mcc_ast_statement *mcc_ast_new_statement_while(struct mcc_ast_expression *while_cond,
                                                      struct mcc_ast_statement *while_stmt);

struct mcc_ast_statement *mcc_ast_new_statement_assignment(struct mcc_ast_assignment *assignment);


struct mcc_ast_statement *mcc_ast_new_statement_declaration(struct mcc_ast_declaration *declaration);

struct mcc_ast_statement *mcc_ast_new_statement_compound(struct mcc_ast_statement *compound_stmt,
                                                         struct mcc_ast_statement *statement);
struct mcc_ast_statement *mcc_ast_new_statement_single(struct mcc_ast_statement *statement);


struct mcc_ast_statement *mcc_ast_new_statement_ret( struct mcc_ast_expression* ret_expr);

struct mcc_ast_statement *mcc_ast_new_statement_empty();


void mcc_ast_delete_statement(struct mcc_ast_statement *statement);

// ------------------------------------------------------------------- Assignments

enum mcc_ast_assignment_type {
    MCC_AST_ASSIGNMENT_TYPE_SINGLE,
    MCC_AST_ASSIGNMENT_TYPE_ARRAY,
};

struct mcc_ast_assignment {
    struct mcc_ast_node node;
    enum mcc_ast_assignment_type type;
    struct mcc_ast_identifier *identifier;
    struct mcc_ast_symbol *assign_symbol;
    union {
        struct {
            struct mcc_ast_expression *rhs;
        }single_assign;

        struct {
            struct mcc_ast_expression *assign_expr;
            struct mcc_ast_expression *rhs;
        }array_assign;
    };
};

struct mcc_ast_assignment *
mcc_ast_new_assignment(struct mcc_ast_identifier *identifier,
                       struct mcc_ast_expression *rhs);

struct mcc_ast_assignment *
mcc_ast_new_array_assignment(struct mcc_ast_identifier *identifier,
                             struct mcc_ast_expression *assign_expr,
                             struct mcc_ast_expression *rhs);

void mcc_ast_delete_assignment(struct mcc_ast_assignment *assignment);


// -------------------------------------------------------------------- Parameter

struct mcc_ast_parameter {
    struct mcc_ast_node node;
    int param_len;
    int param_size;
    struct mcc_ast_statement **decl_parameter;
};

struct mcc_ast_parameter *mcc_ast_new_parameter(struct mcc_ast_statement *decl_parameter, struct mcc_ast_parameter *parameter);

void mcc_ast_delete_parameter(struct mcc_ast_parameter *parameter);


// -------------------------------------------------------------------- Function

struct mcc_ast_function {
    struct mcc_ast_node node;
    struct mcc_ast_function_def *function_def;

    //body of function
    //enum mcc_ast_data_type data_type;
    struct mcc_ast_statement *statement;
};

//function definition

struct mcc_ast_function_def {
    enum mcc_ast_data_type data_type;
    struct mcc_ast_identifier *identifier;

    struct mcc_ast_parameter *parameter;
};

struct mcc_ast_function *mcc_ast_new_function(enum mcc_ast_data_type data_type,
                                              struct mcc_ast_identifier *identifier,
                                              struct mcc_ast_parameter *parameter,
                                              struct mcc_ast_statement *compound_stmt);


void mcc_ast_delete_function(struct mcc_ast_function *function);

// -------------------------------------------------------------------- Program

struct mcc_ast_program {
    struct mcc_ast_node node;
    int max_function;
    int size_function;
    struct mcc_ast_function **function;
};


struct mcc_ast_program *mcc_ast_new_program(struct mcc_ast_program *program,
                                            struct mcc_ast_function *function);
struct mcc_ast_program *mcc_ast_new_program_empty();


void mcc_ast_delete_program(struct mcc_ast_program *program);


// -------------------------------------------------------------------- Utility


// clang-format off

#define mcc_ast_delete(x) _Generic((x), \
		struct mcc_ast_expression *:  mcc_ast_delete_expression, \
		struct mcc_ast_literal *:    mcc_ast_delete_literal, \
		struct mcc_ast_statement *:  mcc_ast_delete_statement, \
        struct mcc_ast_program *:    mcc_ast_delete_program \
	)(x)

// clang-format on

#endif // MCC_AST_H