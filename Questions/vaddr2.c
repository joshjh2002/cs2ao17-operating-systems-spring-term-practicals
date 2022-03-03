#include <stdlib.h>
#include <stdio.h>
int main(int argc, char *argv[])
{
    unsigned long page, offset, address;
    if (argc != 2)
        exit(1);
    address = atoll(argv[1]);

    page = address >> 20;       /*calculating pages number: 2^20 = 1 million*/
    offset = address & 0xfffff; /*calculating remaining offset: 0xfffff is a large buffer.
                                    It can hold a lot of different values*/

    printf("The address %lu contains: \n", address);
    printf("page number = %lu\n", page);
    printf("offset = %lu\n", offset);
    return 0;
}
