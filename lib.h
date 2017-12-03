// #include <sys/types.h>
#include <stdint.h>

// too small buffers don't work for some reason
#define LEN 512

// i really want to use a flexible array
struct buffer {
    volatile int index;
    // it says unsinged in the description
    volatile uint64_t data[LEN];
};
