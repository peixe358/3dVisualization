#include "mc920.h"

int main(int argc, char **argv) {
  MedicalImage *I1=NULL;
  GrayImage    *I2=NULL;
  ColorImage   *I3=NULL;
  float        *hist=NULL;
  int   i,Imax,x,y,z,nelems=0,size,nbins=0;
  char *ext;
  FILE *fp=NULL;

  if (argc!=2) {
    fprintf(stderr,"usage: histogram <image> \n");
    fprintf(stderr,"image: arquivo de imagem\n");    
    exit (-1);
  }


  /* Descubre o formato */

  ext = strrchr(argv[1],'.')+1;
  
  if (strcmp(ext,"pgm")==0){
    I2=ReadGrayImage(argv[1]);    
    //    WriteGrayImage(I2,"teste.pgm");
    /* Calcula o valor maximo */
    Imax = INT_MIN;
    for (y=0; y < I2->ny; y++)
      for (x=0; x < I2->nx; x++)
	if (I2->val[y][x]>Imax)
	  Imax = I2->val[y][x];
    nbins=Imax+1;

    /* Calcula o histograma */
    nelems=I2->nx*I2->ny;
    hist = AllocFloatArray(nbins);
    for (y=0; y < I2->ny; y++)
      for (x=0; x < I2->nx; x++){
	hist[I2->val[y][x]]++;
      }

    DestroyGrayImage(&I2);
    
  }else{
    if (strcmp(ext,"ppm")==0){
      I3=ReadColorImage(argv[1]);    
      WriteColorImage(I3,"teste.ppm");

    /* Calcula o histograma com 64 bins = 4x4x4 bins */

      nbins=64;
      size = 256/4;
      nelems=I3->nx*I3->ny;
      hist = AllocFloatArray(nbins);
      for (y=0; y < I3->ny; y++)
	for (x=0; x < I3->nx; x++){
	  i=(I3->cor[y][x].val[RED]/size)+
	    (I3->cor[y][x].val[GREEN]/size)*4 + 
	    (I3->cor[y][x].val[BLUE]/size)*16;
	  hist[i]++;
	}

      DestroyColorImage(&I3);

    }else{
      if (strcmp(ext,"scn")==0){
	I1=ReadMedicalImage(argv[1]);    
	//WriteMedicalImage(I1,"teste.scn");

	/* Calcula o valor maximo */
	Imax = INT_MIN;
	for (z=0; z < I1->nz; z++)
	  for (y=0; y < I1->ny; y++)
	    for (x=0; x < I1->nx; x++)
	      if (I1->val[z][y][x]>Imax)
		Imax = I1->val[z][y][x];
	nbins=Imax+1;
	/* Calcula o histograma */
	nelems=I1->nx*I1->ny*I1->nz;
	hist = AllocFloatArray(nbins);
	for (z=0; z < I1->nz; z++)
	  for (y=0; y < I1->ny; y++)
	    for (x=0; x < I1->nx; x++)
	      hist[I1->val[z][y][x]]++;
	
	DestroyMedicalImage(&I1);
      }else{
	fprintf(stdout,"Imagem nao processada, devido a extensao invalida");
      }
    }
  }
  fp = fopen("hist.txt","w");
  for (i=0; i < nbins; i++) 
    fprintf(fp,"%d %f\n",i,hist[i]/nelems);
  free(hist);
  fclose(fp);

  return 0;
}

