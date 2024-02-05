#include "string.hpp"

void* memcpy(void* dest, const void* src, size_t n) {

    for (size_t i = 0; i < n; i++) {
        ((char*)dest)[i] = ((char*)src)[i];
    }

    return dest;
}

void* memmove(void* dstptr, const void* srcptr, size_t size) {

    unsigned char* dst = (unsigned char*) dstptr;
    const unsigned char* src = (const unsigned char*) srcptr;

    if (dst < src) {

        for (size_t i = 0; i < size; i++)

            dst[i] = src[i];

    } else {

        for (size_t i = size; i != 0; i--)

            dst[i-1] = src[i-1];
    }
    return dstptr;
}

int memcmp(const void* aptr, const void* bptr, size_t size) {

    const unsigned char* a = (const unsigned char*) aptr;
    const unsigned char* b = (const unsigned char*) bptr;

    for (size_t i = 0; i < size; i++) {

        if (a[i] < b[i])

            return -1;

        else if (b[i] < a[i])

            return 1;
    }

    return 0;
}

void* memset(char* dest, int z, size_t n) {
    
    for (size_t i = 0; i < n; i++) {
        dest[i] = z;
    }

    return dest;
}

size_t strlen(const char* string) {
    size_t Length = 0;

    while (string[Length]) {
        Length++;
    }

    return Length;
}

int strcmp(const char* firstString, const char* secondString) {

    while (( *firstString ) && ( *firstString == *secondString ))
    {

        ++firstString;
        ++secondString;
    }

    return ( *firstString - *secondString );
}
