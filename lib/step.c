#include<stdint.h>
#include<stdlib.h>

// funckja sprawdzająca czy liczba jest względnie pierwsza
uint16_t gcd(uint16_t a, uint16_t b){

    uint16_t temp;

    while (b != 0){
        temp = a % b;

        a = b;
        b = temp;

    }

    return a;

}

// funkcja znajdująca losową liczbę względnie pierwszą do parametru size
uint16_t find_step(uint16_t size,  unsigned int* seed){

    uint16_t r;

    do{
        r = rand_r(seed) % (size - 1) + 1;
    } while(gcd(r, size) != 1);
        
    return r;

}
