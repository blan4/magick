#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wand/MagickWand.h>
#include <sys/types.h>
#include "sort.h"
#include "cartoon.h"
#include "shuffling.h"
#include "average.h"
#include "dependence.h"
#include "function.h"
#include "lines.h"
#include "brick.h"
#include "diffusion.h"
//gcc `MagickWand-config --cflags --cppflags` -o main main.c `MagickWand-config --ldflags --libs`

int main(int argc, char const *argv[])
{
    srand(time(NULL));
    diffusion(argv[1],argv[2],argv[3]);
 //   brick_mix_pics(argv[1],argv[2],argv[3]);
 //   brick_mix_chanals(argv[1],argv[2]);
 //	  brick_colorful(atoi(argv[1]),atoi(argv[2]),argv[3]);
 //   lines_offset(argv[1],argv[2],atoi(argv[3]));
 //   lines(argv[1],argv[2]);
 //   float (*functions[4]) (unsigned int,unsigned int)={dev,sinxy,loga,logax};
 //   function(argv[1],argv[2],functions[atoi(argv[3])]);
 //   function_add(argv[1],argv[2],atoi(argv[3]),logax);
 //   dependence(argv[1],argv[2],atoi(argv[3]));
 //   average(argv[1],argv[2]);
 //   total_sort(argv[1],argv[2]);
 //   super_sort(argv[1],argv[2]);
 //   cartoon(argv[1],argv[4]);
 //   shuffling(argv[1],argv[5]);
    return 0;
}
