#ifndef ERROR_H
#define ERROR_H


#include <stdio.h>


#include "mcc/ast.h"
#include <mcc/parser.h>
#include <mcc/ast_visit.h>

#ifdef __cplusplus
extern "C" {
#endif



 enum symtab_err {

        ERROR_ID_UNDEF,
        ERROR_FUNC_DUBL,
        ERROR_FUN_DEF_MIS,
        ERROR_WRONG_PARAM_TYPE,
        ERROR_IF_DEF_MIS,
        ERROR_WHILE_DEF_MIS,
        ERROR_ARG_NUM,
        ERROR_ARG_TYPE,
        ERROR_NO_RETURN,
        ERROR_NO_MAIN,
        ERROR_BINARY_OP_TYPE,
        ERROR_ASSIGN_TYPE_MIS,
        ERROR_ASSIGN_ARRAY,
        ERROR_RETURN_TYPE,

};



void mcc_error_list(const char *err_msg, int line_err, struct mcc_parser_result *result, ...);
struct mcc_parser_result *mcc_ast_semantic_chk(struct mcc_parser_result *result);
struct mcc_parser_result *sem_result;
struct mcc_parser_result *st_result;


#ifdef __cplusplus
}
#endif

#endif //ERROR_H