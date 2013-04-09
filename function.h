#include "math.h"
float dev(unsigned int x, unsigned int y){
    float res=0;
    res = x;
    res/= (y+1);
    return res; 
}
float sinxy(unsigned int x, unsigned int y){
    float res=0;
    res = sin(x*y);
    return res;
}
float loga(unsigned int x, unsigned int y){
    float res=0;
    res = y;
    res /= (x+1);
    res = log(res);
    return res;
}
float logax(unsigned int x, unsigned int y){
    return log(y);
}

void function(char const *sours, char const *res, float (*fun)(unsigned int x,unsigned int y)){
    MagickWand *mw_1,*mw_res;
    PixelIterator *imw_1,*imw_res;
    PixelWand **pmw_1,**pmw_res;
    Quantum qr_1,qg_1,qb_1;

    unsigned long y;
    register long x;
    unsigned int width,height;
    float xy;

    MagickWandGenesis();

    mw_1 = NewMagickWand();
    MagickReadImage(mw_1, sours);
    width = MagickGetImageWidth(mw_1);
    height = MagickGetImageHeight(mw_1);
    printf("Function img width %d\nheight %d\n",width,height);
    mw_res = NewMagickWand();
    MagickSetSize(mw_res,width,height);
    MagickReadImage(mw_res,"xc:none");

    imw_1  = NewPixelIterator(mw_1);
    imw_res = NewPixelIterator(mw_res);

    for(y = 0;y < height; ++y){
        pmw_1  = PixelGetNextIteratorRow(imw_1, &width);
        pmw_res = PixelGetNextIteratorRow(imw_res, &width);
        for (x = 0;x < (long)width; ++x){
            xy = fun(x,y);
            qr_1 = PixelGetRedQuantum(pmw_1[x])*xy;
            qg_1 = PixelGetGreenQuantum(pmw_1[x])*xy;
            qb_1 = PixelGetBlueQuantum(pmw_1[x])*xy;


            PixelSetRedQuantum(pmw_res[x], qr_1 );
            PixelSetGreenQuantum(pmw_res[x], qg_1 );
            PixelSetBlueQuantum(pmw_res[x], qb_1 );

        }
        PixelSyncIterator(imw_res);
    }

    MagickWriteImage(mw_res,res);

    imw_1 = DestroyPixelIterator(imw_1);
    mw_1 = DestroyMagickWand(mw_1);
    imw_res = DestroyPixelIterator(imw_res);
    mw_res = DestroyMagickWand(mw_res);

    MagickWandTerminus();
} 


void function_add(char const *sours, char const *res,unsigned k, float (*fun)(unsigned int x,unsigned int y)){
    MagickWand *mw_1,*mw_res;
    PixelIterator *imw_1,*imw_res;
    PixelWand **pmw_1,**pmw_res;
    Quantum qr_1,qg_1,qb_1;

    unsigned long y;
    register long x;
    unsigned int width,height;
    float xy;

    MagickWandGenesis();

    mw_1 = NewMagickWand();
    MagickReadImage(mw_1, sours);
    width = MagickGetImageWidth(mw_1);
    height = MagickGetImageHeight(mw_1);
    printf("Function img width %d\nheight %d\n",width,height);
    mw_res = NewMagickWand();
    MagickSetSize(mw_res,width,height);
    MagickReadImage(mw_res,"xc:none");

    imw_1  = NewPixelIterator(mw_1);
    imw_res = NewPixelIterator(mw_res);

    for(y = 0;y < height; ++y){
        pmw_1  = PixelGetNextIteratorRow(imw_1, &width);
        pmw_res = PixelGetNextIteratorRow(imw_res, &width);
        for (x = 0;x < (long)width; ++x){
            xy = fun(x,y)*k;
            qr_1 = PixelGetRedQuantum(pmw_1[x])+xy;
            qg_1 = PixelGetGreenQuantum(pmw_1[x])+xy;
            qb_1 = PixelGetBlueQuantum(pmw_1[x])+xy;


            PixelSetRedQuantum(pmw_res[x], qr_1 );
            PixelSetGreenQuantum(pmw_res[x], qg_1 );
            PixelSetBlueQuantum(pmw_res[x], qb_1 );

        }
        PixelSyncIterator(imw_res);
    }

    MagickWriteImage(mw_res,res);

    imw_1 = DestroyPixelIterator(imw_1);
    mw_1 = DestroyMagickWand(mw_1);
    imw_res = DestroyPixelIterator(imw_res);
    mw_res = DestroyMagickWand(mw_res);

    MagickWandTerminus();
} 