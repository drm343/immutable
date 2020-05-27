#ifndef IMMUTABLE_FSM_DEFINE
#define IMMUTABLE_FSM_DEFINE 1

#include <stdio.h>
#include <stdlib.h>


typedef struct _Dictionary Dictionary;


typedef int (*f_callback)(int action);


Dictionary *FSM_create(int key, f_callback f, Dictionary *d);
Dictionary *FSM_status(int status, Dictionary *d);
int FSM_run(int status, int event, Dictionary *d);
Dictionary *FSM_free(Dictionary **d);
void FSM_release(Dictionary **d);
#endif
