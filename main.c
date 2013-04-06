#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wand/MagickWand.h>
#include <sys/types.h>
#include "sort.h"
#include "cartoon.h"
#include "shuffling.h"

//gcc `MagickWand-config --cflags --cppflags` -o main main.c `MagickWand-config --ldflags --libs`

int main(int argc, char const *argv[])
{
    srand(time(NULL));

    total_sort(argv[1],argv[2]);
    sort(argv[1],argv[3]);
    cartoon(argv[1],argv[4]);
    shuffling(argv[1],argv[5]);
    return 0;
}