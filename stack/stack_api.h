#ifndef STACK_API_DEFINE
#define STACK_API_DEFINE 1

#include "stack_string.h"
#include "stack_int.h"


#define Stack_v(a, b) a = _Generic((a),\
        char *: Stack_String_value,\
        int: Stack_Int_value,\
        default: Stack_value\
        )(b)


#define Stack_create(a) _Generic((a),\
        char *: Stack_String_create,\
        int: Stack_Int_create\
        )(a)


#define Stack_create_move(a) _Generic((a),\
        char *: Stack_String_create_move,\
        int: Stack_Int_create_move\
        )(a)


#define Stack_push(a, b) _Generic((a),\
        char *: Stack_String_push,\
        int: Stack_Int_push\
        )(a, b)


#define Stack_push_move(a, b) _Generic((a),\
        char *: Stack_String_push_move,\
        int: Stack_Int_push_move\
        )(a, b)

#endif
