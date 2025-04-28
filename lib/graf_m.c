#include<stdlib.h>
#include<stdio.h>
#include<stdint.h>

uint16_t** create_graf_mother(int size, int conf){

    uint16_t** m = (uint16_t**)malloc(sizeof(uint16_t*) * size);
    if(m == NULL)
        return NULL;

    for(int i = 0; i < size; i++){
        m[i] = (uint16_t*)malloc(sizeof(uint16_t) * size);
        if(m[i] == NULL)
        return NULL;
    }

    unsigned int seed;
    FILE *f = fopen("/dev/urandom", "rb");
    if(f != NULL){
        if (fread(&seed, sizeof(seed), 1, f) != 1) {
            // Read failed: fallback to less secure method
            seed = (unsigned int)time(NULL) + omp_get_thread_num();
        }
        fclose(f);
    } else {
        // File open failed: use time + thread ID
        seed = (unsigned int)time(NULL) + omp_get_thread_num();
    }

    rand_r(&seed);

    return m;

}
