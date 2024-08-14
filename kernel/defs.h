#define NCPU 5

struct spinlock;

// uart.c
void uartinit();
void uartputc_sync(int);

// spinlock.c
void initlock(struct spinlock*);
void acquire(struct spinlock*);
void release(struct spinlock*);
