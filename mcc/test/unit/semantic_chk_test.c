#include <CuTest.h>
#include <mcc/ast.h>
#include <stdlib.h>

#include "mcc/parser.h"
#include <symtab.h>
#include <error.h>


//------------------------------------------------




struct  mcc_ast_symbol_table *GetSymTab() {


    const char input[200];

    struct mcc_parser_result result = mcc_parse_string(input);

    if (MCC_PARSER_STATUS_OK != result.status) {

        if (result.program != NULL) {

            mcc_ast_delete_program(result.program);
        }

        perror("fail");
        exit(EXIT_FAILURE);
       }

        struct mcc_ast_symbol_table *resultTable = create_symtab(result.program);
        return resultTable;
    }


 void CheckBinaryOpType(CuTest *tc){

     const char input[] = "void main() {int a; a = 1 + 2.0;}";

     struct mcc_parser_result result = mcc_parse_string(input);
     CuAssertTrue(tc, MCC_PARSER_STATUS_UNKNOWN_ERROR == result.status);

     char error_msg[1024] = {0};
     printf(error_msg, sizeof(error_msg), ERROR_BINARY_OP_TYPE,  "int", "+", "float");

    CuAssertStrEquals(tc, error_msg, GetSymTab(result)->error->err_msg);
    mcc_ast_delete(result.program);
}

void CheckAssignmentType(CuTest *tc){


    const char input[] = "void main() { int a; int [3]b; a = b;}";

    struct mcc_parser_result result = mcc_parse_string(input);

    CuAssertTrue(tc, MCC_PARSER_STATUS_UNKNOWN_ERROR == result.status);

    char error_msg[1024] = {0};

    printf(error_msg, sizeof(error_msg), ERROR_ASSIGN_ARRAY, "int", "int array");

CuAssertStrEquals(tc, error_msg, GetSymTab(result)->error->err_msg);
    mcc_ast_delete(result.program);


}

void CheckAssignmentType_2(CuTest *tc){


    const char input[] = "void main(){int[4] a; int[3] b; b = a;}";

    struct mcc_parser_result result = mcc_parse_string(input);

    CuAssertTrue(tc, MCC_PARSER_STATUS_UNKNOWN_ERROR == result.status);

    char error_msg[1024] = {0};

    printf(error_msg, sizeof(error_msg), ERROR_ASSIGN_ARRAY, "int", "int array");

    CuAssertStrEquals(tc, error_msg, GetSymTab(result)->error->err_msg);
    mcc_ast_delete(result.program);


}


void CheckAssignmentType_1(CuTest *tc){


    const char input[] = "void main(){int a; bool b; b = a;}";

    struct mcc_parser_result result = mcc_parse_string(input);

    CuAssertTrue(tc, MCC_PARSER_STATUS_UNKNOWN_ERROR == result.status);

    char error_msg[1024] = {0};

    printf(error_msg, sizeof(error_msg), ERROR_ASSIGN_TYPE_MIS, "int", "int array");

    CuAssertStrEquals(tc, error_msg, GetSymTab(result)->error->err_msg);
    mcc_ast_delete(result.program);


}


void CheckArgumentType(CuTest *tc){


    const char input[] = "void main(){int a; bool b; add(a,b);}  int add(int d, int c){return d+c;}";

    struct mcc_parser_result result = mcc_parse_string(input);

    CuAssertTrue(tc, MCC_PARSER_STATUS_UNKNOWN_ERROR == result.status);

    char error_msg[1024] = {0};

    printf(error_msg, sizeof(error_msg), ERROR_ARG_TYPE);

    CuAssertStrEquals(tc, error_msg, GetSymTab(result)->error->err_msg);
    mcc_ast_delete(result.program);


}

void CheckIfStatement(CuTest *tc)
{

const char input[] = "void main() { int a; if(a+1) {} }";

    struct mcc_parser_result result = mcc_parse_string(input);

    CuAssertTrue(tc, MCC_PARSER_STATUS_UNKNOWN_ERROR == result.status);

    char error_msg[1024] = {0};

    printf(error_msg, sizeof(error_msg), ERROR_IF_DEF_MIS, "int");

    CuAssertStrEquals(tc, error_msg, GetSymTab(result)->error->err_msg);
    mcc_ast_delete(result.program);

}

void CheckWhileStatement(CuTest *tc)
{

const char input[] = "void main() { int a; while((a + 1) * 5) {} }";

    struct mcc_parser_result result = mcc_parse_string(input);

    CuAssertTrue(tc, MCC_PARSER_STATUS_UNKNOWN_ERROR == result.status);

    char error_msg[1024] = {0};

    printf(error_msg, sizeof(error_msg), ERROR_WHILE_DEF_MIS, "int");

    CuAssertStrEquals(tc, error_msg, GetSymTab(result)->error->err_msg);
    mcc_ast_delete(result.program);
}
void CheckReturnType(CuTest *tc){


    const char input[] = "int func1(){int a; return;} void main() {}";

    struct mcc_parser_result result = mcc_parse_string(input);

    CuAssertTrue(tc, MCC_PARSER_STATUS_UNKNOWN_ERROR == result.status);

    char error_msg[1024] = {0};

    printf(error_msg, sizeof(error_msg), ERROR_RETURN_TYPE, "void");

    CuAssertStrEquals(tc, error_msg, GetSymTab(result)->error->err_msg);
    mcc_ast_delete(result.program);

}

void CheckReturnType_1(CuTest *tc){


    const char input[] = "float test(){return (42 + 192);}";

    struct mcc_parser_result result = mcc_parse_string(input);

    CuAssertTrue(tc, MCC_PARSER_STATUS_UNKNOWN_ERROR == result.status);

    char error_msg[1024] = {0};

    printf(error_msg, sizeof(error_msg), ERROR_RETURN_TYPE, "void");

    CuAssertStrEquals(tc, error_msg, GetSymTab(result)->error->err_msg);
    mcc_ast_delete(result.program);

}


#define TESTS \
	TEST(BinaryOp_1) \

#include "main_stub.inc"
#undef TESTS