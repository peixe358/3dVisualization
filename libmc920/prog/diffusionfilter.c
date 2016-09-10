#include "mc920.h"

GrayImage *LinearFilter1(GrayImage *I, Kernel *K)
{
  int xp,yp,xq,yq,i;
  float val,wsum;
  GrayImage *J=CreateGrayImage(I->nx,I->ny);


  wsum=0.0;
  for (i=0; i < K->A->n; i++) 
    wsum += fabs(K->w[i]);

  for (yp=0; yp < J->ny; yp++) {
    for (xp=0; xp < J->nx; xp++) {
      val = 0.0;
      for (i=0; i < K->A->n; i++) {
	xq = xp + K->A->adj[i].dx;
	yq = yp + K->A->adj[i].dy;
	if ((xq >= 0)&&(xq < J->nx)&&(yq >= 0)&&(yq < J->ny)) 
	  val += I->val[yq][xq]*K->w[i];
      }
      J->val[yp][xp] = ROUND(val/wsum);
    }
  }

  return(J);
}

GrayImage *DiffusionFilter(GrayImage *I, Kernel *K, float perc)
{
  int xp,yp,xq,yq,i,j,*cost=AllocIntArray(K->A->n),wsize=(int)(perc*K->A->n);
  GrayImage *J=CreateGrayImage(I->nx,I->ny);
  GQueue *Q=CreateGQueue(256,K->A->n,cost);
  float  *w=AllocFloatArray(wsize),wsum,val;
 
  wsum=0.0;
  for (i=0; i < wsize; i++) {
    w[i]=exp(-((float)(i*i)/(wsize*wsize)));
    wsum += w[i];
  }

  for (yp=0; yp < J->ny; yp++) {
    for (xp=0; xp < J->nx; xp++) {
      for (i=0; i < K->A->n; i++) {
	xq = xp + K->A->adj[i].dx;
	yq = yp + K->A->adj[i].dy;	
	if ((xq >= 0)&&(xq < J->nx)&&(yq >= 0)&&(yq < J->ny)) {
	  cost[i]=abs(I->val[yq][xq]-I->val[yp][xp]);
	  InsertGQueue(&Q,i);
	}	
      }
      val = 0.0; j = 0;
      while (!EmptyGQueue(Q)){
	i = RemoveGQueue(Q);
	if (j < wsize){
	  xq = xp + K->A->adj[i].dx;
	  yq = yp + K->A->adj[i].dy;	
	  val += I->val[yq][xq]*w[j];
	  j++;
	}
      }
      J->val[yp][xp] = (int)(val/wsum+0.5);
    }
  }
  DestroyGQueue(&Q);
  free(w); free(cost);
  return(J);
}

int main(int argc, char **argv) {
  GrayImage *I,*J;
  Kernel   *K;

  if (argc!=2) {
    fprintf(stderr,"usage: difusionfilter <image> \n");
    fprintf(stderr,"image: arquivo de imagem pgm\n");    
    exit (-1);
  }

  I = ReadGrayImage(argv[1]);
  //K = CircularGaussian(15.0);
  //J = LinearFilter1(I,K);
  K = CircularGaussian(15.0); 
  J = DiffusionFilter(I,K,1.0);
  WriteGrayImage(J,"filtered.pgm");

  DestroyGrayImage(&I);
  DestroyGrayImage(&J);
  DestroyKernel(&K);


  return 0;
}

