#include "mcc/ast_print.h"

#include <assert.h>
#include <stdio.h>
#include "mcc/ast_visit.h"

#include <mcc/ast.h>

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


        default: assert(!"unknown op");

    }

}


const char *mcc_ast_print_unary_op(enum mcc_ast_unary_op op)

{




    switch (op) {

        case MCC_AST_UNARY_OP_MINUS: return "-";
        case MCC_AST_UNARY_OP_NOT: return "!";
        default: assert(!"unknown op");
    }


}


const char *mcc_ast_print_data_type(enum mcc_ast_data_type data_type)
{
    switch (data_type) {

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

        default: assert(!"unknown data_type");
    }


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

    print_dot_node(out, expression , "expr: lit");
    print_dot_edge(out, expression , expression->literal, "literal");
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
             mcc_ast_print_unary_op(expression->unary_op));

    FILE *out = data;
    print_dot_node(out, expression, label);
    print_dot_edge(out, expression, expression->unary_expression, "expr");
}


static void print_dot_expression_identifier(struct mcc_ast_expression *expression,
                                            void *data)
{
    assert(expression);
    assert(data);

    FILE *out = data;
    print_dot_node(out, expression, "expr: id");
    print_dot_edge(out, expression, expression->identifier, "identifier");
}


static void print_dot_expression_parenth(struct mcc_ast_expression *expression, void *data)
{
    assert(expression);
    assert(data);

    FILE *out = data;
    print_dot_node(out, expression, "( )");
    print_dot_edge(out, expression, expression, "expression");
}

static void print_dot_expression_call(struct mcc_ast_expression  *expression, void *data)
{
    assert(expression);
    assert(data);

    char label[LABEL_SIZE] = { 0 };
    //snprintf(label, sizeof(label), "%s%s",expression-> single_expr->arguments, "()");

    FILE *out = data;
    print_dot_node(out, expression, label);
    for (int i = 0; i < expression->arguments->argument_count; i++) {
        print_dot_edge(out, expression, expression->arguments->arg_expr[i], "expr");
    }
}



//--------------------------------------------------------Identifier

static void print_dot_identifier(struct mcc_ast_identifier *identifier, void *data ){

    assert(identifier);
    assert(data);

    char label[LABEL_SIZE] = { 0 };
    snprintf(label, sizeof(label), "%s", identifier-> value);

    FILE *out = data;
    print_dot_node(out, identifier, label);

}
//-----------------------------------------------------------------Statements


static void print_dot_statement(struct mcc_ast_statement * statement, void *data){


    assert(statement);
    assert(data);


    FILE *out = data;
    print_dot_node(out , statement, "stmt:expr");
    print_dot_edge(out, statement, statement->expression, "expr" );

}

static void print_dot_statement_if(struct mcc_ast_statement *statement, void *data)
{
    assert(statement);
    assert(data);

    FILE *out = data;
    print_dot_edge(out, statement, statement -> if_cond, "if_condition");
    print_dot_edge(out, statement, statement -> if_stmt, "if_statement");

}
static void print_dot_statement_assignment( struct mcc_ast_statement *statement, void *data)
{
    assert(statement);
    assert(data);


    FILE *out = data;
    print_dot_edge(out, statement, statement->assignment->identifier,
                   "identifier");
    if (statement->assignment->type == MCC_AST_ASSIGNMENT_TYPE_SINGLE) {


        print_dot_edge(out, statement, statement->assignment->single_assign.rhs,
                       "=");
    } else {


        print_dot_edge(out, statement, statement->assignment->array_assign.assign_expr,
                       "[]");
        print_dot_edge(out, statement, statement->assignment->array_assign.rhs,
                       "=");
    }
}

static void print_dot_statement_while(struct mcc_ast_statement *statement, void *data)
{
    assert(statement);
    assert(data);

    FILE *out = data;
    print_dot_node(out, statement, "stmt: while");
    print_dot_edge(out, statement, statement->while_cond, "expression");
    print_dot_edge(out, statement, statement->while_stmt, "statement");
}

static void print_dot_statement_declaration(struct mcc_ast_statement *statement,void *data)
{
    assert(statement);
    assert(data);

    FILE *out = data;
    print_dot_node(out, statement, "stmt: decl");
    print_dot_edge(out, statement, statement->declaration, "declaration");
}


static void print_dot_statement_compound( struct mcc_ast_statement *statement, void *data)
{
    assert(statement);
    assert(data);

    assert(statement->type == MCC_AST_STATEMENT_TYPE_COMPOUND_STMT);

    char label[LABEL_SIZE] = "{ 0 }";

    FILE *out = data;

    print_dot_node(out, statement, label);
    for(int i = 0 ; i < statement->num_stmt; i++)
    {
        // char label[LABEL_SIZE] = { 0 };
        snprintf(label, sizeof(label), "%d", i);
        print_dot_edge(out,statement,  statement->stmt[i],label );


    }
}


static void print_dot_statement_ret(struct mcc_ast_statement *statement , void *data){

    assert(statement);
    assert(data);

    char label[LABEL_SIZE] = {"return"};

    FILE *out = data;
    print_dot_node(out, statement, label);
    if (statement->ret_expr == NULL )

        print_dot_edge(out, statement, statement->ret_expr, "expr");

}



/*// ------------------------------------------------------------------- Declaration

static void print_dot_declaration(struct mcc_ast_declaration *declaration,
                                  void *data)
{
    assert(declaration);
    assert(data);

    char label[LABEL_SIZE] = { 0 };
    snprintf(label, sizeof(label), "decl: %s");  //TODO

    FILE *out = data;
    print_dot_node(out, declaration, label);
    print_dot_edge(out, declaration, declaration->ident, "identifier");
   if(declaration->decl_lit){

       print_dot_edge(out, declaration,declaration->decl_lit, "decl_literal");
   }
}
*/
// ------------------------------------------------------------------- Parameter

static void print_dot_parameter(struct mcc_ast_parameter *parameter, void *data)
{
    assert(parameter);
    assert(data);


    FILE *out = data;
    print_dot_node(out, parameter, "param: decl");
    for(int i = 0; i< parameter->param_size; i++) {
        print_dot_edge(out, parameter, parameter->decl_parameter[i], "declaration");

    }


}

// ------------------------------------------------------------------- Function

static void print_dot_function(struct mcc_ast_function *function, void *data)
{
    assert(function);
    assert(data);

    char label[LABEL_SIZE] = { 0 };
    snprintf(label, sizeof(label), "func: %s",mcc_ast_print_data_type(function->function_def-> data_type));

    FILE *out = data;
    print_dot_node(out, function, label);
    print_dot_edge(out, function, function->function_def->identifier, "identifier");
    if (function->function_def->parameter) {
        print_dot_edge(out, function, function->function_def->parameter, "parameter");
    }
    if (function->statement) {
        print_dot_edge(out, function, function->statement, "body");
    }
}

// ------------------------------------------------------------------- Program

static void print_dot_program(struct mcc_ast_program *program, void *data)
{
    assert(program);
    assert(data);

    FILE *out = data;
    print_dot_node(out, program, "program");
    for (int i = 0; i < program->size_function; i++) {
        print_dot_edge(out, program, program->function[i],
                       "function_definition");
    }
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
            .expression_binary_op = print_dot_expression_binary_op,
            .expression_unary_op = print_dot_expression_unary_op,
            .expression_identifier = print_dot_expression_identifier,
            .expression_call = print_dot_expression_call,
            .expression_parenth = print_dot_expression_parenth,


            .statement = print_dot_statement,
            .statement_if = print_dot_statement_if,
            .statement_assignment = print_dot_statement_assignment,
            .statement_while = print_dot_statement_while,
            .statement_declaration = print_dot_statement_declaration,
            .statement_compound = print_dot_statement_compound,
            .statement_ret = print_dot_statement_ret,


            .literal_int = print_dot_literal_int,
            .literal_float = print_dot_literal_float,
            .literal_string = print_dot_literal_string,
            .literal_bool = print_dot_literal_bool,


            .identifier = print_dot_identifier,
            .program = print_dot_program,
            .parameter = print_dot_parameter,
            //.declaration = print_dot_declaration,
            .function_def = print_dot_function

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
    mcc_ast_visit_identifier(identifier, &visitor);

    print_dot_end(out);
}


void mcc_ast_print_dot_program(FILE* out, struct mcc_ast_program *program){

    assert(out);
    assert(program);


    print_dot_begin(out);

    struct mcc_ast_visitor visitor = print_dot_visitor(out);

    mcc_ast_visit(program, &visitor);

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

void mcc_ast_print_dot_function(FILE *out, struct mcc_ast_function *function)
{
    assert(out);
    assert(function);

    print_dot_begin(out);

    struct mcc_ast_visitor visitor = print_dot_visitor(out);
    mcc_ast_visit(function, &visitor);

    print_dot_end(out);
}