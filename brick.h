void print(int** matrix,int size_x,int size_y){
	int i,j;
	for (i = 0; i < size_y; ++i){
		for (j = 0; j < size_x; ++j){
			printf("%4d ", matrix[i][j]);
		}
		printf("\n");
	}
}
void find_pos(int** matrix,int size_x,int size_y,int* x,int* y){
	int i,j;
	for (i= 0; i < size_y; i++){
		for (j= 0; j < size_x; j++){
			//printf("[%d %d]=%d\t",j,i,matrix[i][j]);
			if (matrix[i][j]==0){
				*x = j;
				*y = i;
				return;
			}
		}
	}
}
int calculate_size(int** matrix,int size_x,int size_y,int pos_x,int pos_y){
	int i,j;
	int res_x=0,res_y=0;
	for (i=pos_y;i<size_y;++i)
		if (matrix[i][pos_x]==0)
			res_x++;
		
	for (j=pos_x;j<size_x;++j)
		if (matrix[pos_y][j]==0)
			res_y++;
	
	return (res_y > res_x) ? res_x : res_y;
}
void brick(int size_x,int size_y){
	int** matrix;
	int i,j;
	int r,len;

	matrix = (int**)malloc(sizeof(int*)*size_y);
	for (i=0;i<size_y;i++){
		matrix[i] = (int*)malloc(sizeof(int)*size_x);
		for (j=0;j<size_x;j++)
			matrix[i][j] = 0;
	}
	print(matrix,size_x,size_y);
	int x=0,y=0;
	int count=1;

	while ((x+1)<size_x || (y+1)<size_y){
		//printf("\n");
		find_pos(matrix,size_x,size_y,&x,&y);
		//printf("[%d %d] ",x,y );
		len = calculate_size(matrix,size_x,size_y,x,y);
		//printf("len=%d ",len);
		r = rand() % len;
		//printf("r=%d\n",r );
		if (r==0)
			r++;
		for (i=y;i<r+y;++i)
			for (j=x;j<r+x;++j)
				matrix[i][j]=count;
		count++;
		//print(matrix,size_x,size_y);
	}
	printf("\n");
	print(matrix,size_x,size_y);
} 
int true_random(int len){
	return (len == 1) ? len : len/2 ;
}
int true_random_2(int len,int n){
	int good = n/20;
	int so_good = good/2 + rand()%good;
	if (len < so_good){
		return (len == 1) ? len : len/2 ;
	}
	return so_good;
}
void brick_colorful(int size_x,int size_y,const char* res){
	int** matrix;
	int i,j;
	int r,len;

	matrix = (int**)malloc(sizeof(int*)*size_y);
	for (i=0;i<size_y;i++){
		matrix[i] = (int*)malloc(sizeof(int)*size_x);
		for (j=0;j<size_x;j++)
			matrix[i][j] = 0;
	}
	int y;
    int x;
    int color;
	printf("START GENERATING MATRIX\n");
	while ((x+1)<size_x || (y+1)<size_y){
		find_pos(matrix,size_x,size_y,&x,&y);
		len = calculate_size(matrix,size_x,size_y,x,y);
		//r = true_random(len);
		r = rand() % len;
		if (r == 0)
			r++;
		color = (rand()%3) + 1;
		for (i=y;i<r+y;++i)
			for (j=x;j<r+x;++j)
				matrix[i][j]=color;
	}
//	print(matrix,size_x,size_y);
	printf("SAVING\n");

	//==========================
	MagickWand *mw_res;
    PixelIterator *imw_res;
    PixelWand **pmw_res;
    Quantum qr_1,qg_1,qb_1;

    unsigned int width,height;

    MagickWandGenesis();

    width = size_x;
    height = size_y;
    mw_res = NewMagickWand();
    MagickSetSize(mw_res,width,height);
    MagickReadImage(mw_res,"xc:none");

    imw_res = NewPixelIterator(mw_res);

    for(y = 0;y < height; ++y){
        pmw_res = PixelGetNextIteratorRow(imw_res, &width);
        for (x = 0;x < width; ++x){
        	(matrix[y][x] == 1) ? PixelSetRedQuantum(pmw_res[x], 65535 ) :
        	(matrix[y][x] == 2) ? PixelSetGreenQuantum(pmw_res[x], 65535 ) :
        	PixelSetBlueQuantum(pmw_res[x], 65535 );          
        }
        PixelSyncIterator(imw_res);
    }

    MagickWriteImage(mw_res,res);

    imw_res = DestroyPixelIterator(imw_res);
    mw_res = DestroyMagickWand(mw_res);

    MagickWandTerminus();
}

void brick_mix_chanals(const char* sours,const char* res){
	MagickWand *mw_1,*mw_res;
    PixelIterator *imw_1,*imw_res;
    PixelWand **pmw_1,**pmw_res;
    Quantum qr_1,qg_1,qb_1;

    unsigned int width,height;
    MagickWandGenesis();

    mw_1 = NewMagickWand();
    MagickReadImage(mw_1, sours);
    width = MagickGetImageWidth(mw_1);
    height = MagickGetImageHeight(mw_1);


	int** matrix;
	int i,j;
	int r,len;

	matrix = (int**)malloc(sizeof(int*)*height);
	for (i=0;i<height;i++){
		matrix[i] = (int*)malloc(sizeof(int)*width);
		for (j=0;j<width;j++)
			matrix[i][j] = 0;
	}
	int y;
    int x;
    int color;
	printf("START GENERATING MATRIX\n");
	while ((x+1)<width || (y+1)<height){
		find_pos(matrix,width,height,&x,&y);
		len = calculate_size(matrix,width,height,x,y);
		r = true_random_2(len,width);
		//r = rand() % len;
		if (r == 0)
			r++;
		color = (rand()%6) + 1;
		for (i=y;i<r+y;++i)
			for (j=x;j<r+x;++j)
				matrix[i][j]=color;
	}
//	print(matrix,size_x,size_y);
	printf("SAVING\n");

	//==========================

    
    printf("Shuffling img width %d\nheight %d\n",width,height);
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

            shuffle_case(&qr_1,&qg_1,&qb_1,matrix[y][x]);

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