#ifndef MEMORY_H
#define MEMORY_H

#ifndef STACK_POSITION
#define STACK_POSITION 0x10000000
#endif

#ifndef STACK_SIZE
#define STACK_SIZE 262144  /* 1GiB divided into 4KiB pages*/
#endif

#ifndef PAGE_SIZE
#define PAGE_SIZE 0x2000
#endif

typedef enum {PUSH_PAGE = 1, POP_PAGE} Action;

void initializePageStack();
void pageManager(Action action, void **page);



#endif /* MEMORY_H */