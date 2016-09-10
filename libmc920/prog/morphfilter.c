#include "mc920.h"

GrayImage *Dilate(GrayImage *I, Kernel *K)
{
  int xp,yp,xq,yq,i;
  GrayImage *J=CreateGrayImage(I->nx,I->ny);

  for (yp=0; yp < J->ny; yp++) {
    for (xp=0; xp < J->nx; xp++) {
      J->val[yp][xp] = INT_MIN;
      for (i=0; i < K->A->n; i++) {
	xq = xp + K->A->adj[i].dx;
	yq = yp + K->A->adj[i].dy;
	if ((xq >= 0)&&(xq < J->nx)&&(yq >= 0)&&(yq < J->ny)) 
	  if (I->val[yq][xq] > J->val[yp][xp])
	    J->val[yp][xp] = I->val[yq][xq] + (int)K->w[i]; 
      }
    }
  }


  return(J);
}

GrayImage *Erode(GrayImage *I, Kernel *K)
{
  int xp,yp,xq,yq,i;
  GrayImage *J=CreateGrayImage(I->nx,I->ny);

  for (yp=0; yp < J->ny; yp++) {
    for (xp=0; xp < J->nx; xp++) {
      J->val[yp][xp] = INT_MAX;
      for (i=0; i < K->A->n; i++) {
	xq = xp + K->A->adj[i].dx;
	yq = yp + K->A->adj[i].dy;
	if ((xq >= 0)&&(xq < J->nx)&&(yq >= 0)&&(yq < J->ny)) 
	  if (I->val[yq][xq] < J->val[yp][xp])
	    J->val[yp][xp] = I->val[yq][xq] + (int)K->w[i]; 
      }
    }
  }

  return(J);
}

int main(int argc, char **argv) {
  GrayImage *I[3];
  Kernel   *K;

  if (argc!=3) {
    fprintf(stderr,"usage: morphfilter <image> <structuring element>\n");
    fprintf(stderr,"image:  arquivo de imagem pgm\n");    
    fprintf(stderr,"structuring element: arquivo com a lista de adjacentes e pesos\n");    
    exit (-1);
  }

  I[0] = ReadGrayImage(argv[1]);
  K    = ReadKernel2D(argv[2]);
  I[1] = Dilate(I[0],K);
  WriteGrayImage(I[1],"dilated.pgm");
  I[2] = Erode(I[1],K);
  WriteGrayImage(I[2],"closed.pgm");


  DestroyGrayImage(&I[0]);
  DestroyGrayImage(&I[1]);
  DestroyGrayImage(&I[2]);
  DestroyKernel(&K);

  return 0;
}

