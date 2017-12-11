// why is ftruncate nonstandard?
#define _XOPEN_SOURCE 500

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include "gen.h"
#include "lib.h"


int main() {
    int shm_fd;
    int nsize;
    struct buffer *mem;
    uint32_t seed = 0;


    shm_fd = shm_open("cyclic_buffer", O_CREAT | O_RDWR, S_IRWXU);
	if (shm_fd == -1) {
		perror("Can't open file");
		return 1;
	}

    // should shared memory objects be of pagesize?
	nsize = ftruncate(
        shm_fd,
        sizeof(struct buffer)
        //  + LEN * sizeof(((struct buffer*)NULL)->data[0])
    );
	if (nsize == -1) {
		perror("Can't truncate file");
		return nsize;
	}

	mem = mmap(
        NULL, sizeof(struct buffer),
        // i will probably need to read it
        PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0
    );
	if (mem == MAP_FAILED) {
		perror("Can't mmap");
		return -1;
	}

    mem->index = 0;

    while (1) {
        // this ensures that data is not read before it's written
        generate((void *)(mem->data + mem->index % LEN), seed);
        // in theory nothing willbe written before the index is calculated
        ++mem->index; // = (mem->index + 1) ;
        ++seed;
        printf("%d\n", seed);
        // sleep(1);
    }
}
