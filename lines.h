void lines(char const *sours, char const *res){
    MagickWand *mw_1,*mw_res;
    PixelIterator *imw_1,*imw_res;
    PixelWand **pmw_1,**pmw_res;
    Quantum qr_1,qg_1,qb_1;

    unsigned long y;
    register long x;
    unsigned int width,height;

    MagickWandGenesis();

    mw_1 = NewMagickWand();
    MagickReadImage(mw_1, sours);
    width = MagickGetImageWidth(mw_1);
    height = MagickGetImageHeight(mw_1);
    printf("lining img width %d\nheight %d\n",width,height);
    mw_res = NewMagickWand();
    MagickSetSize(mw_res,width,height);
    MagickReadImage(mw_res,"xc:none");

    imw_1  = NewPixelIterator(mw_1);
    imw_res = NewPixelIterator(mw_res);

    for(y = 0;y < height; ++y){
        pmw_1  = PixelGetNextIteratorRow(imw_1, &width);
        pmw_res = PixelGetNextIteratorRow(imw_res, &width);
        for (x = 0;x < (long)width; ++x){
            qr_1 = PixelGetRedQuantum(pmw_1[x]);
            qg_1 = PixelGetGreenQuantum(pmw_1[x]);
            qb_1 = PixelGetBlueQuantum(pmw_1[x]);

            if (x%2){
            	PixelSetRedQuantum(pmw_res[width-x],  qr_1 );
            	PixelSetGreenQuantum(pmw_res[width-x], qg_1 );
            	PixelSetBlueQuantum(pmw_res[width-x], qb_1 );
            }else{
            	PixelSetRedQuantum(pmw_res[x], qr_1 );
            	PixelSetGreenQuantum(pmw_res[x], qg_1 );
            	PixelSetBlueQuantum(pmw_res[x], qb_1 );
            }
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
 


void lines_offset(char const *sours, char const *res,int round){
    MagickWand *mw_1,*mw_res;
    PixelIterator *imw_1,*imw_res;
    PixelWand **pmw_1,**pmw_res;
    Quantum qr_1,qg_1,qb_1;

    unsigned long y;
    register long x;
    unsigned int width,height;
    unsigned int offset;

    MagickWandGenesis();

    mw_1 = NewMagickWand();
    MagickReadImage(mw_1, sours);
    width = MagickGetImageWidth(mw_1);
    height = MagickGetImageHeight(mw_1);
    printf("lining img width %d\nheight %d\n",width,height);
    mw_res = NewMagickWand();
    MagickSetSize(mw_res,width,height);
    MagickReadImage(mw_res,"xc:none");

    imw_1  = NewPixelIterator(mw_1);
    imw_res = NewPixelIterator(mw_res);

    for(y = 0;y < height; ++y){
        pmw_1  = PixelGetNextIteratorRow(imw_1, &width);
        pmw_res = PixelGetNextIteratorRow(imw_res, &width);
        offset = rand() % round;
        for (x = 0;x < (long)width; ++x){
            qr_1 = PixelGetRedQuantum(pmw_1[x]);
            qg_1 = PixelGetGreenQuantum(pmw_1[x]);
            qb_1 = PixelGetBlueQuantum(pmw_1[x]);

            if (x + offset < width){
                PixelSetRedQuantum(pmw_res[x+offset],  qr_1 );
                PixelSetGreenQuantum(pmw_res[x+offset], qg_1 );
                PixelSetBlueQuantum(pmw_res[x+offset], qb_1 );
            }else{
                PixelSetRedQuantum(pmw_res[x+offset-width], qr_1 );
                PixelSetGreenQuantum(pmw_res[x+offset-width], qg_1 );
                PixelSetBlueQuantum(pmw_res[x+offset-width], qb_1 );
            }
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
