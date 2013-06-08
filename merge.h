void merge(char const *sours1, char const *sours2,char const *res){
    MagickWand *mw_1,*mw_2,*mw_res;
    PixelIterator *imw_1,*imw_2,*imw_res;
    PixelWand **pmw_1,**pmw_2,**pmw_res;
    Quantum qr_1,qg_1,qb_1;

    unsigned long y;
    register long x;
    unsigned int width,height;

    MagickWandGenesis();

    mw_1 = NewMagickWand();
    mw_2 = NewMagickWand();
    MagickReadImage(mw_1, sours1);
    MagickReadImage(mw_2, sours2);
    width = MagickGetImageWidth(mw_1);
    height = MagickGetImageHeight(mw_1);
    if (MagickGetImageWidth(mw_2)!=width){
        printf("%d %d\n",MagickGetImageWidth(mw_2),width );
        perror("Width doesnt match\n");
        return;
    }
    if (MagickGetImageHeight(mw_2)!=height){
        perror("Height doesnt match\n");
        return;
    }
    printf("Merge img width %d\nheight %d\n",width,height);
    mw_res = NewMagickWand();
    MagickSetSize(mw_res,width,height);
    MagickReadImage(mw_res,"xc:none");

    imw_1 = NewPixelIterator(mw_1);
    imw_2 = NewPixelIterator(mw_2);
    imw_res = NewPixelIterator(mw_res);

    int rnd;

    for(y = 0;y < height; ++y){
        pmw_1 = PixelGetNextIteratorRow(imw_1, &width);
        pmw_2 = PixelGetNextIteratorRow(imw_2, &width);
        pmw_res = PixelGetNextIteratorRow(imw_res, &width);
        for (x = 0;x < (long)width; ++x){
            rnd = rand()%2;
            qr_1 = rnd ? PixelGetRedQuantum(pmw_1[x]) 	: PixelGetRedQuantum(pmw_2[x]);
            qg_1 = rnd ? PixelGetGreenQuantum(pmw_1[x]) : PixelGetGreenQuantum(pmw_2[x]);
            qb_1 = rnd ? PixelGetBlueQuantum(pmw_1[x])  : PixelGetBlueQuantum(pmw_2[x]);

            PixelSetRedQuantum(pmw_res[x], qr_1 );
            PixelSetGreenQuantum(pmw_res[x], qg_1 );
            PixelSetBlueQuantum(pmw_res[x], qb_1 );
        }
        PixelSyncIterator(imw_res);
    }

    MagickWriteImage(mw_res,res);

    imw_1 = DestroyPixelIterator(imw_1);
    imw_2 = DestroyPixelIterator(imw_2);
    mw_1 = DestroyMagickWand(mw_1);
    mw_2 = DestroyMagickWand(mw_2);
    imw_res = DestroyPixelIterator(imw_res);
    mw_res = DestroyMagickWand(mw_res);

    MagickWandTerminus();
} 
