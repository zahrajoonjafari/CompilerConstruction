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




void ChkNoMain(CuTest *tc) {

    const char input[] = "void func1() {}";

   struct mcc_parser_result result = mcc_parse_string(input);
    //CuAssertTrue(tc, MCC_PARSER_STATUS_UNKNOWN_ERROR == result.status);

    char error_msg[1024] = {0};
    printf(error_msg, sizeof(error_msg), ERROR_NO_MAIN);

    CuAssertStrEquals(tc, error_msg, GetSymTab()->error->err_msg);

    mcc_ast_delete(result.program);

}


void ChkNoMain_1(CuTest *tc) {

    const char input[] = "void main(int a){}";

    struct mcc_parser_result result = mcc_parse_string(input);
    CuAssertTrue(tc, MCC_PARSER_STATUS_UNKNOWN_ERROR == result.status);

    char error_msg[1024] = {0};
    printf(error_msg, sizeof(error_msg), ERROR_NO_MAIN);

    CuAssertStrEquals(tc, error_msg, GetSymTab(result)->error->err_msg);
    mcc_ast_delete(result.program);
}


void IdentifierUndef(CuTest *tc) {

    const char input[] = "void main(){!b;}";

    struct mcc_parser_result result = mcc_parse_string(input);
    CuAssertTrue(tc, MCC_PARSER_STATUS_UNKNOWN_ERROR == result.status);

    char error_msg[1024] = {0};
    printf(error_msg, sizeof(error_msg), ERROR_ID_UNDEF);

    CuAssertStrEquals(tc, error_msg, GetSymTab(result)->error->err_msg);
    mcc_ast_delete(result.program);
}



void ChekDublicateFunc(CuTest *tc){


    const char input[] = "void main() {} void func1(){} void func1(){}";
    struct mcc_parser_result result = mcc_parse_string(input);
    CuAssertTrue(tc, MCC_PARSER_STATUS_UNKNOWN_ERROR == result.status);

    char error_msg[1024] = {0};
    printf(error_msg, sizeof(error_msg), ERROR_FUNC_DUBL);

    CuAssertStrEquals(tc, error_msg, GetSymTab(result)->error->err_msg);
    mcc_ast_delete(result.program);

}


void ChkNoReturn(CuTest *tc){

        const char input[] = "void main() {int a; } int func1(){}";

    struct mcc_parser_result result = mcc_parse_string(input);
    CuAssertTrue(tc, MCC_PARSER_STATUS_UNKNOWN_ERROR == result.status);

    char error_msg[1024] = {0};
    printf(error_msg, sizeof(error_msg), ERROR_NO_RETURN);

    CuAssertStrEquals(tc, error_msg, GetSymTab(result)->error->err_msg);
    mcc_ast_delete(result.program);

}
void ChkNoReturn_2(CuTest *tc){

    const char input[] = "void main() {int a; } int func1(){int a; if (a == 5) { a = 6;} else if (a == 5) {return 6;}}";
    struct mcc_parser_result result = mcc_parse_string(input);
    CuAssertTrue(tc, MCC_PARSER_STATUS_UNKNOWN_ERROR == result.status);

    char error_msg[1024] = {0};
    printf(error_msg, sizeof(error_msg), ERROR_NO_RETURN);

    CuAssertStrEquals(tc, error_msg, GetSymTab(result)->error->err_msg);
    mcc_ast_delete(result.program);

}


void ChkNoReturn_3(CuTest *tc){

    const char input[] = "void main() {int a; } int func1(){int a; if (a == 5) { return 6;} else if (a == 5) {return 8;}}";
    struct mcc_parser_result result = mcc_parse_string(input);
    CuAssertTrue(tc, MCC_PARSER_STATUS_UNKNOWN_ERROR == result.status);

    char error_msg[1024] = {0};
    printf(error_msg, sizeof(error_msg), ERROR_NO_RETURN);

    CuAssertStrEquals(tc, error_msg, GetSymTab(result)->error->err_msg);
    mcc_ast_delete(result.program);

}

void ReturnType(CuTest *tc){

    const char input[] = "float test(){return (42 + 192);}";
    struct mcc_parser_result result = mcc_parse_string(input);
    CuAssertTrue(tc, MCC_PARSER_STATUS_UNKNOWN_ERROR == result.status);

    char error_msg[1024] = {0};
    printf(error_msg, sizeof(error_msg), ERROR_RETURN_TYPE);

    CuAssertStrEquals(tc, error_msg, GetSymTab(result)->error->err_msg);
    mcc_ast_delete(result.program);

}


void MissFunctionDef(CuTest *tc)
{

const char input[] = "void main() {int a; func2(); }";

    struct mcc_parser_result result = mcc_parse_string(input);
    CuAssertTrue(tc, MCC_PARSER_STATUS_UNKNOWN_ERROR == result.status);

    char error_msg[1024] = {0};
    printf(error_msg, sizeof(error_msg), ERROR_FUN_DEF_MIS);

    CuAssertStrEquals(tc, error_msg, GetSymTab(result)->error->err_msg);
    mcc_ast_delete(result.program);

}

void CheckArgumentNum(CuTest *tc){


    const char input[] = "void main(){test();} int test(int a){int b; return b;}";

    struct mcc_parser_result result = mcc_parse_string(input);

    CuAssertTrue(tc, MCC_PARSER_STATUS_UNKNOWN_ERROR == result.status);

    char error_msg[1024] = {0};

    printf(error_msg, sizeof(error_msg), ERROR_ARG_NUM);

    CuAssertStrEquals(tc, error_msg, GetSymTab(result)->error->err_msg);
    mcc_ast_delete(result.program);


}


#define TESTS \
	TEST(BinaryOp_1) \

#include "main_stub.inc"
#undef TESTS