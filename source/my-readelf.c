#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>

#include "elf.h"


int main (int argc, char **argv)
{
    readelf(argv[1]);
    return EXIT_SUCCESS;
}