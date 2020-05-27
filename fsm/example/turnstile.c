#include <stdio.h>
#include <stdlib.h>


#include "../fsm.h"


#define COIN 0
#define PUSH 1


#define LOCKED 0
#define UNLOCKED 1
#define MAX_STATUS_VALUE (UNLOCKED + 1)
#define MAX_STATUS(a, b) (MAX_STATUS_VALUE * a + b)


typedef struct {
    int coin;
    int status;
    Dictionary *top;
} Door;


// UNLOCKED(PUSH) :- LOCKED.
// UNLOCKED(_) :- UNLOCKED.
int handle_unlocked(int action) {
    if (action == PUSH) {
        printf("UNLOCKED: push\n");
        return LOCKED;
    }
    else {
        printf("UNLOCKED: can not coin\n");
        return UNLOCKED;
    }
}


// LOCKED(COIN) :- UNLOCKED.
// LOCKED(_) :- LOCKED.
int handle_locked(int action) {
    if (action == COIN) {
        printf("LOCKED: insert coin\n");
        return UNLOCKED;
    }
    else {
        printf("LOCKED: can not push\n");
        return LOCKED;
    }
}


Door start(void) {
    Dictionary *d_UNLOCKED = FSM_create(UNLOCKED, handle_unlocked, NULL);
    Dictionary *d_LOCKED = FSM_create(LOCKED, handle_locked, d_UNLOCKED);

    int status = LOCKED;
    Door init = {0, status, d_LOCKED};
    return init;
}


// coin(LOCKED, UNLOCKED) :- coin++.
Door coin(Door door) {
    int new_status = FSM_run(door.status, COIN, door.top);
    int door_coin = door.coin;
    int do_status = MAX_STATUS(door.status, new_status);

    switch (do_status) {
        case MAX_STATUS(LOCKED, UNLOCKED):
            door_coin++;
            break;
    }
    Door new_door = {door_coin, new_status, door.top};
    return new_door;
}


// push(UNLOCKED, LOCKED).
Door push(Door door) {
    int new_status = FSM_run(door.status, PUSH, door.top);
    int do_status = MAX_STATUS(door.status, new_status);

    Door new_door = {door.coin, new_status, door.top};
    return new_door;
}


// take(_, LOCKED) :- coin = 0.
Door take(Door door) {
    printf("take:%d coin\n", door.coin);

    Door new_door = {0, LOCKED, door.top};
    return new_door;
}


// TAKE(_) :- LOCKED.
int handle_take(int action) {
    return LOCKED;
}


// take2(_, LOCKED) :- coin = 0.
Door take2(Door door) {
    const int any = MAX_STATUS_VALUE + 1;
    Dictionary *d_TAKE = FSM_create(any, handle_take, door.top);

    int new_status = FSM_run(door.status, any, d_TAKE);

    printf("take:%d coin\n", door.coin);

    Door new_door = {0, new_status, door.top};
    FSM_free(&d_TAKE);
    return new_door;
}


int main(void) {
    Door a = start();
    Door b = push(a);
    b = coin(b);

    Door c = coin(b);
    c = coin(c);
    c = push(c);
    c = coin(c);

    c = take(c);

    Door d = coin(c);
    d = take2(d);

    printf("\ncoin\n");
    printf("a:%d\n", a.coin);
    printf("b:%d\n", b.coin);
    printf("c:%d\n", c.coin);
    printf("d:%d\n", d.coin);
    FSM_release(&(c.top));
}
