
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <mcc/parser.h>


struct mcc_parser_error_errors *new_parser_error_errors(){

    struct mcc_parser_error_errors *parser_errors = malloc(sizeof(*parser_errors));

         parser_errors->error = malloc(sizeof(struct mcc_parser_error));
         parser_errors->count = 0;

    return parser_errors;

}

struct mcc_parser_error_errors *parsing_error(struct mcc_parser_error_errors *errors,
                                              struct mcc_parser_error *error)

{
    assert(error);
    assert(errors);

    if(errors->count == 0){
        memcpy(errors, error, sizeof(*error));

    } else {
        struct mcc_parser_error *new = realloc(errors, sizeof(error) *(errors->count + 1));
        if(new == NULL){
            return NULL;
        }

        errors->error = new;
        memcpy(&(errors->error[errors->count]), error, sizeof(*error));
    }

   free(error);
    errors->count++;
    return errors;
}

void mcc_delete_result(struct mcc_parser_result *result){


    free(result->errors);
    free(result->errors->error);
}