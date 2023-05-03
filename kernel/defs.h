#include "types.h"

#define NCPU 4

struct spinlock {
    uint locked;
};

// uart.c
void uartputc_sync(int);

// spinlock.c
void initlock(struct spinlock*);
void acquire(struct spinlock*);
void release(struct spinlock*);
