#define _XOPEN_SOURCE 500
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>

#include "gen.h"
#include "lib.h"


int main() {
    int shm_fd;
    uint64_t index;
    struct buffer *mem;
    uint32_t seed;
    uint32_t new_seed;
    int64_t tmp = -1;

    shm_fd = shm_open("cyclic_buffer", O_RDONLY, S_IRWXU);
    if (shm_fd == -1) {
        perror("Can't open file");
        return 1;
    }

    mem = mmap(
        NULL, sizeof(struct buffer), PROT_READ, MAP_SHARED, shm_fd, 0
    );

    // ensure it doesn't fail
    index = mem->index;
    while (tmp == -1)
        tmp = verify((void *)(mem->data + index % LEN));
    seed = tmp;

    while (1) {
        if (index / LEN < mem->index % LEN) {
            printf("overtaken\n");
        }
        if (index >= mem->index) {
            usleep(1);
            continue;
        }

        // discards volatile
        new_seed = verify((void *)(mem->data + index % LEN));

        if (seed != new_seed) {
            printf("Wrong seed\n");
        } else {
            printf("%d\n", seed);
        }

        seed += 1;
        index += 1;
        // index = (index + 1) % LEN;
    }

}
