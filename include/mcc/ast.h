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

#include <zconf.h>

// Forward Declarations
struct mcc_ast_expression;
struct mcc_ast_literal;
struct mcc_ast_identifier;
struct mcc_ast_single_expression;
struct mcc_ast_arguments;

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
    MCC_AST_DATA_TYPE_VOID
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
// ---------------------------------------------------------------- Single/Expressions

enum mcc_ast_expression_type {
    MCC_AST_EXPRESSION_TYPE_SINGLE,
    MCC_AST_EXPRESSION_TYPE_BINARY_OP
};

struct mcc_ast_expression {
   struct  mcc_ast_node node;

    enum mcc_ast_data_type data_type;

    enum mcc_ast_expression_type type;

    union {

        /* MCC_AST_EXPRESSION_TYPE_LITERAL */
        struct mcc_ast_single_expression *single_expr;

        /* MCC_AST_EXPRESSION_TYPE_BINARY_OP */ // optional
        struct {
            enum mcc_ast_binary_op op;
            struct mcc_ast_single_expression *lhs;
            struct mcc_ast_expression *rhs;
        };
    };


};

//------------------------Single

enum mcc_ast_single_expression_type {
   // MCC_AST_STATEMENT_TYPE_EXPR,
    MCC_AST_SINGLE_EXPRESSION_TYPE_LITERAL,
    MCC_AST_SINGLE_EXPRESSION_TYPE_UNARY_OP,
    MCC_AST_SINGLE_EXPRESSION_TYPE_PARENTH,
    MCC_AST_SINGLE_EXPRESSION_TYPE_IDENTIFIER,
    MCC_AST_SINGLE_EXPRESSION_TYPE_CALL_EXPR,


};


struct mcc_ast_single_expression {

    struct mcc_ast_node node;

    enum mcc_ast_data_type data_type;

    enum mcc_ast_single_expression_type type;

    union {
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

       };


        // MCC_AST_EXPRESSION_TYPE_PARENTH
        struct mcc_ast_expression *expression;

        //MCC_AST_EXPRESSION_IDENTIFIER
        struct{

            struct mcc_ast_identifier *identifier;
            struct mcc_ast_expression *id_expr;

        };
    };
};


struct mcc_ast_singel_expression *mcc_ast_new_single_expression_call_expr(struct mcc_ast_identifier *identifier_call,
                                                            struct mcc_ast_arguments *arguments );

struct mcc_ast_single_expression *mcc_ast_new_single_expression_literal(struct mcc_ast_literal *literal);

struct mcc_ast_single_expression *mcc_ast_new_single_expression_identifier(struct mcc_ast_identifier *identifier,
                                                             struct mcc_ast_expression *id_expr);


struct mcc_ast_single_expression *mcc_ast_new_single_expression_unary_op( enum mcc_ast_unary_op op,
                                                            struct mcc_ast_expression *expression);

struct mcc_ast_single_expression *mcc_ast_new_single_expression_parenth(struct mcc_ast_expression *expression);





struct mcc_ast_expression *mcc_ast_new_expression_single(struct mcc_ast_single_expression *single_expr);

struct mcc_ast_expression *mcc_ast_new_expression_binary_op(enum mcc_ast_binary_op op,
                                          struct  mcc_ast_single_expression *lhs, struct mcc_ast_expression *rhs);




//void mcc_ast_delete_expression(struct mcc_ast_new_expression_single *single_expression,
  //                             struct mcc_ast_new_expression_binary_op *binaryOp);




// ---------------------------------------------------------------- Arguments



struct mcc_ast_arguments {

    struct mcc_ast_node node;

    int argument_size;
    int argument_count;
    struct mcc_ast_expression **arg_expr;

};

struct mcc_ast_arguments *mcc_ast_new_arguments(struct mcc_ast_expression *arg_expr);

struct mcc_ast_arguments *mcc_ast_new_arguments_expr(struct mcc_ast_arguments *arguments,
                                              struct mcc_ast_expression *arg_expr);

void mcc_ast_delete_arguments(struct mcc_ast_arguments *arguments);


//---------------------------------------------------------------- Identifier

struct mcc_ast_identifier {
    struct mcc_ast_node node;


    enum mcc_ast_data_type *data_type;
    char *name;


};

struct mcc_ast_identifier *mcc_ast_new_identifier(char *name);

void mcc_ast_delete_identifier(struct mcc_ast_identifier *identifier);

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

struct mcc_ast_literal *mcc_ast_new_literal_string(char* value);

struct mcc_ast_literal *mcc_ast_new_literal_bool(char value);

void mcc_ast_delete_literal(struct mcc_ast_literal *literal);

//void mcc_ast_empty_node();


// -------------------------------------------------------------------- Utility

// clang-format off

#define mcc_ast_delete(x) _Generic((x), \
		struct mcc_ast_expression *: mcc_ast_delete_expression, \
		struct mcc_ast_literal *:    mcc_ast_delete_literal,\
		struct mcc_ast_arguments *: mcc_ast_delete_arguments, \
		struct mcc_ast_identifier *: mcc_ast_delete_identifier,\
	)(x)


// clang-format on

#endif // MCC_AST_H