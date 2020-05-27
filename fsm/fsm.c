#include "fsm.h"


typedef struct _Dictionary {
    int key;
    f_callback action;
    struct _Dictionary *next;
} Dictionary;


Dictionary *FSM_create(int key, f_callback f, Dictionary *d) {
    Dictionary *r;
    r = malloc(sizeof(*r));
    r->key = key;
    r->action = f;
    r->next = d;
    return r;
}


Dictionary * FSM_status(int status, Dictionary *d) {
    Dictionary *copy = d;

    while (copy != NULL) {
        if (copy->key == status) {
            return copy;
        }
        else {
            copy = copy->next;
        }
    }
    return NULL;
}


Dictionary *FSM_free(Dictionary **d) {
    Dictionary *current = *d;
    Dictionary *next = current->next;
    free(current);
    *d = NULL;
    return next;
}


void FSM_release(Dictionary **d) {
    Dictionary *current = *d;

    while (current != NULL) {
        current = FSM_free(&current);
    }
    *d = NULL;
}


int FSM_run(int status, int event, Dictionary *d) {
    Dictionary *copy = FSM_status(status, d);

    if (copy != NULL) {
        return copy->action(event);
    }
    return status;
}
