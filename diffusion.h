void diffusion(char const *sours,char const *sours2, char const *res){
    MagickWand *mw_1,*mw_res,*mw_2;
    PixelIterator *imw_1,*imw_res,*imw_2;
    PixelWand **pmw_1,**pmw_res,**pmw_2;
    Quantum qr_1,qg_1,qb_1;

    unsigned long y;
    unsigned long x1;
    unsigned long x2;
    register long x;
    unsigned int width,height;

    MagickWandGenesis();

    mw_1 = NewMagickWand();
    mw_2 = NewMagickWand();
    MagickReadImage(mw_1, sours);
    MagickReadImage(mw_2, sours2);
    width = MagickGetImageWidth(mw_1);
    height = MagickGetImageHeight(mw_1);
    if (MagickGetImageWidth(mw_2)!=width){
    	perror("Width doesnt match\n");
    	return;
    }
    if (MagickGetImageHeight(mw_2)!=height){
    	perror("Height doesnt match\n");
    	return;
    }
    printf("Diffusion img width %d\nheight %d\n",width,height);
    
    mw_res = NewMagickWand();
    MagickSetSize(mw_res,width+width,height);
    MagickReadImage(mw_res,"xc:none");

    imw_1 = NewPixelIterator(mw_1);
    imw_2 = NewPixelIterator(mw_2);
    imw_res = NewPixelIterator(mw_res);

    for(y = 0;y < height; ++y){
        pmw_1 = PixelGetNextIteratorRow(imw_1, &width);
        pmw_2 = PixelGetNextIteratorRow(imw_2, &width);
        pmw_res = PixelGetNextIteratorRow(imw_res, &width);
        x1=x2=0;
        for (x = 0;x < (long)width; ++x){
        	if ((x+y)%2){
        		//printf("[x1 %d] ",x1 );
            	qr_1 = PixelGetRedQuantum(pmw_1[x1]);
            	qg_1 = PixelGetGreenQuantum(pmw_1[x1]);
            	qb_1 = PixelGetBlueQuantum(pmw_1[x1]);
            	x1++;
            }else {
            	//printf("[x2 %d] ",x2 );
            	qr_1 = PixelGetRedQuantum(pmw_2[x2]);
            	qg_1 = PixelGetGreenQuantum(pmw_2[x2]);
            	qb_1 = PixelGetBlueQuantum(pmw_2[x2]);
            	x2++;
            }

            PixelSetRedQuantum(pmw_res[x], qr_1 );
            PixelSetGreenQuantum(pmw_res[x], qg_1 );
            PixelSetBlueQuantum(pmw_res[x], qb_1 );

        }
        PixelSyncIterator(imw_res);
    }

    MagickWriteImage(mw_res,res);

    imw_1 = DestroyPixelIterator(imw_1);
    mw_1 = DestroyMagickWand(mw_1);
    imw_2 = DestroyPixelIterator(imw_2);
    mw_2 = DestroyMagickWand(mw_2);
    imw_res = DestroyPixelIterator(imw_res);
    mw_res = DestroyMagickWand(mw_res);

    MagickWandTerminus();
} 
