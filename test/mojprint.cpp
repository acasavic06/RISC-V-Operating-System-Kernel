#include "../lib/hw.h"
#include "../lib/console.h"
#include "mojprint.hpp"

void printInteger(uint64 integer)
{
    static char digits[] = "0123456789";
    char buf[16];
    int i, neg;
    uint x;

    neg = 0;
    if (integer < 0)
    {
        neg = 1;
        x = -integer;
    } else
    {
        x = integer;
    }

    i = 0;
    do
    {
        buf[i++] = digits[x % 10];
    } while ((x /= 10) != 0);
    if (neg)
        buf[i++] = '-';

    while (--i >= 0) { __putc(buf[i]); }

}
void printHexa(uint64 xx,uint padding){
    char buffer[16];
    for (int i = 7; i >= 0; i--)
    {
        uint8 byte = (xx >> (8 * i)) & 0xFF; // i-ti bajt
        buffer[15 - (i*2 + 1)] = ("0123456789ABCDEF"[byte >> 4]);
        buffer[15 - (i*2)] = ("0123456789ABCDEF"[byte & 0x0F]);
    }

    __putc('0'); __putc('x');

    int i = 0;
    while (i < 15 && buffer[i] == '0') i++;

    int len = 16 - i;
    int diff = padding - len;

    for (int j = 0; j < diff; j++) __putc('0'); // padding
    while (i < 16) __putc(buffer[i++]);
}
void printStr(const char *string) {
    while(*string!='\0'){
        __putc(*string);
        string++;
    }
}


void printUint64(unsigned long value) {
    if (value == 0) {
        __putc('0');
        __putc('x');
        __putc('0');
        return;
    }

    // Print "0x" prefix for hexadecimal
    __putc('0');
    __putc('x');

    char buffer[16]; // Enough to hold the largest unsigned long value in hexadecimal
    int i = 0;

    // Convert the number to a hex string in reverse order
    while (value > 0) {
        unsigned long digit = value & 0xF;
        if (digit < 10) {
            buffer[i++] = '0' + digit;
        } else {
            buffer[i++] = 'A' + (digit - 10);
        }
        value >>= 4;
    }

    // Print the string in the correct order
    while (i > 0) {
        __putc(buffer[--i]);
    }
}