struct pixel{
    unsigned red;
    unsigned green;
    unsigned blue;
};

static int cmp_quant(const void *p1, const void *p2){
    return *(Quantum *)p1 >= *(Quantum *)p2;
}
static int reverse_cmp_quant(const void *p1, const void *p2){
    return *(Quantum *)p1 < *(Quantum *)p2;
}
static int cmp_color(const void *p1, const void *p2){
    return *(size_t *)p1 < *(size_t *)p2;
}
static int cmp_pixel(const void *p1, const void *p2){
    const struct pixel *px1 = p1;
    const struct pixel *px2 = p2;
   // printf("%d + %d + %d = %d\n",px1->red,px1->green,px1->blue,px1->red + px1->green + px1->blue );
    return
            (px1->red + px1->green + px1->blue)
            >=
            (px2->red + px2->green + px2->blue);
}

void full_sort(char const *sours, char const *res){
    MagickWand *mw_1,*mw_res;
    PixelIterator *imw_1,*imw_res;
    PixelWand **pmw_1,**pmw_res;
    Quantum *qr_1,*qg_1,*qb_1;

    unsigned long y;
    register long x;
    unsigned int width,height;

    MagickWandGenesis();

    mw_1 = NewMagickWand();
    MagickReadImage(mw_1, sours);
    width = MagickGetImageWidth(mw_1);
    height = MagickGetImageHeight(mw_1);
    printf("FULL Sort img\nwidth %d\nheight %d\n",width,height);
    mw_res = NewMagickWand();
    MagickSetSize(mw_res,width,height);
    MagickReadImage(mw_res,"xc:none");

    imw_1  = NewPixelIterator(mw_1);
    imw_res = NewPixelIterator(mw_res);

    long int hw = height*width;
    qr_1 = (Quantum*)malloc(hw*sizeof(Quantum));
    qb_1 = (Quantum*)malloc(hw*sizeof(Quantum));
    qg_1 = (Quantum*)malloc(hw*sizeof(Quantum));

    for(y = 0;y < height; ++y){
        pmw_1  = PixelGetNextIteratorRow(imw_1, &width);
        for (x = 0;x < (long)width; ++x){
            qr_1[x+y*width] = PixelGetRedQuantum(pmw_1[x]);
            qg_1[x+y*width] = PixelGetGreenQuantum(pmw_1[x]);
            qb_1[x+y*width] = PixelGetBlueQuantum(pmw_1[x]);
        }
    }
    qsort(qr_1,width*height,sizeof(Quantum),cmp_quant);
    qsort(qb_1,width*height,sizeof(Quantum),reverse_cmp_quant);
  //  qsort(qg_1,width*height,sizeof(Quantum),cmp_quant);
    for (y = 0;y < height; ++y){
        pmw_res = PixelGetNextIteratorRow(imw_res, &width);
        for (x = 0;x < (long)width; ++x){
            PixelSetRedQuantum(pmw_res[x], qr_1[x+y*width] );
            PixelSetGreenQuantum(pmw_res[x], qg_1[x+y*width] );
            PixelSetBlueQuantum(pmw_res[x], qb_1[x+y*width] );
        }
        PixelSyncIterator(imw_res);
    }

    MagickWriteImage(mw_res,res);

    imw_1 = DestroyPixelIterator(imw_1);
    mw_1 = DestroyMagickWand(mw_1);
    imw_res = DestroyPixelIterator(imw_res);
    mw_res = DestroyMagickWand(mw_res);


    free(qr_1);
    free(qb_1);
    free(qg_1);
    MagickWandTerminus();
}


void sort(char const *sours, char const *res){
    MagickWand *mw_1,*mw_res;
    PixelIterator *imw_1,*imw_res;
    PixelWand **pmw_1,**pmw_res;
    Quantum *qr_1,*qg_1,*qb_1;

    unsigned long y;
    register long x;
    unsigned int width,height;

    MagickWandGenesis();

    mw_1 = NewMagickWand();
    MagickReadImage(mw_1, sours);
    width = MagickGetImageWidth(mw_1);
    height = MagickGetImageHeight(mw_1);
    printf("Sort rows img\nwidth %d\nheight %d\n",width,height);
    mw_res = NewMagickWand();
    MagickSetSize(mw_res,width,height);
    MagickReadImage(mw_res,"xc:none");

    imw_1  = NewPixelIterator(mw_1);
    imw_res = NewPixelIterator(mw_res);

    qr_1 = (Quantum*)malloc(width*sizeof(Quantum));
    qb_1 = (Quantum*)malloc(width*sizeof(Quantum));
    qg_1 = (Quantum*)malloc(width*sizeof(Quantum));

    for(y = 0;y < height; ++y){
        pmw_1  = PixelGetNextIteratorRow(imw_1, &width);
        pmw_res = PixelGetNextIteratorRow(imw_res, &width);
        for (x = 0;x < (long)width; ++x){
            qr_1[x] = PixelGetRedQuantum(pmw_1[x]);
            qg_1[x] = PixelGetGreenQuantum(pmw_1[x]);
            qb_1[x] = PixelGetBlueQuantum(pmw_1[x]);
        }
        qsort(qr_1,width,sizeof(Quantum),cmp_quant);
        qsort(qb_1,width,sizeof(Quantum),reverse_cmp_quant);
 //       qsort(qg_1,width,sizeof(Quantum),cmp_quant);
        for (x = 0;x < (long)width; ++x){
            PixelSetRedQuantum(pmw_res[x], qr_1[x] );
            PixelSetGreenQuantum(pmw_res[x], qg_1[x] );
            PixelSetBlueQuantum(pmw_res[x], qb_1[x] );
        }
        PixelSyncIterator(imw_res);
    }

    MagickWriteImage(mw_res,res);

    imw_1 = DestroyPixelIterator(imw_1);
    mw_1 = DestroyMagickWand(mw_1);
    imw_res = DestroyPixelIterator(imw_res);
    mw_res = DestroyMagickWand(mw_res);

    free(qr_1);
    free(qb_1);
    free(qg_1);
    MagickWandTerminus();
}


void total_sort(char const *sours, char const *res){
    MagickWand *mw_1,*mw_res;
    PixelIterator *imw_1,*imw_res;
    PixelWand **pmw_1,**pmw_res;
    struct pixel *pix;
    double *colors;
    Quantum *qr_1,*qg_1,*qb_1;

    unsigned long y;
    register long x;
    unsigned int width,height;

    MagickWandGenesis();

    mw_1 = NewMagickWand();
    MagickReadImage(mw_1, sours);
    width = MagickGetImageWidth(mw_1);
    height = MagickGetImageHeight(mw_1);
    printf("Total Sort rows img\nwidth %d\nheight %d\n",width,height);
    mw_res = NewMagickWand();
    MagickSetSize(mw_res,width,height);
    MagickReadImage(mw_res,"xc:none");

    imw_1  = NewPixelIterator(mw_1);
    imw_res = NewPixelIterator(mw_res);

    long int hw = height*width;
    //pix = (struct pixel*)malloc(hw*sizeof(struct pixel));
    colors = (double*)malloc(hw*sizeof(double));

    for(y = 0;y < height; ++y){
        pmw_1  = PixelGetNextIteratorRow(imw_1, &width);
        for (x = 0;x < width; ++x){
        //    pix[x+y*width].red   = PixelGetRedQuantum(pmw_1[x]);
        //    pix[x+y*width].blue  = PixelGetBlueQuantum(pmw_1[x]);
        //    pix[x+y*width].green = PixelGetGreenQuantum(pmw_1[x]);
            colors[x+y*width] = PixelGetFuzz(pmw_1[x]);
        }
    }

    //qsort(pix,hw,sizeof(struct pixel),cmp_pixel);
    qsort(colors,hw,sizeof(double),cmp_color);

    for (y = 0;y < height; ++y){
        pmw_res = PixelGetNextIteratorRow(imw_res, &width);
        for (x = 0;x < (long)width; ++x){
        //    PixelSetRedQuantum(pmw_res[x], pix[x+y*width].red );
        //    PixelSetGreenQuantum(pmw_res[x], pix[x+y*width].green );
        //    PixelSetBlueQuantum(pmw_res[x], pix[x+y*width].blue );
            PixelSetFuzz(pmw_res[x], colors[x+y*width]);
        }
        PixelSyncIterator(imw_res);
    }

    MagickWriteImage(mw_res,res);

    imw_1 = DestroyPixelIterator(imw_1);
    mw_1 = DestroyMagickWand(mw_1);
    imw_res = DestroyPixelIterator(imw_res);
    mw_res = DestroyMagickWand(mw_res);

    //free(pix);
    free(colors);
    MagickWandTerminus();
}
