#include "mc920.h"

GrayImage *MedianFilter(GrayImage *I, AdjRel *A)
{
  int xp,yp,xq,yq,i,j,nelems,*cost=AllocIntArray(A->n);
  GrayImage *J=CreateGrayImage(I->nx,I->ny);
  GQueue *Q=CreateGQueue(256,A->n,cost);
   
  for (yp=0; yp < J->ny; yp++) {
    for (xp=0; xp < J->nx; xp++) {
      nelems=0;
      for (i=0; i < A->n; i++) {
	xq = xp + A->adj[i].dx;
	yq = yp + A->adj[i].dy;	
	if ((xq >= 0)&&(xq < J->nx)&&(yq >= 0)&&(yq < J->ny)) {
	  cost[i]=I->val[yq][xq];
	  InsertGQueue(&Q,i);
	  nelems++;
	}	
      }
      j = 0;
      while (j <= nelems/2){
	i = RemoveGQueue(Q);
	j++;
      }
      xq = xp + A->adj[i].dx;
      yq = yp + A->adj[i].dy;
      J->val[yp][xp] = I->val[yq][xq];
      while (!EmptyGQueue(Q)){
	i = RemoveGQueue(Q);
      }      
    }
  }

  return(J);
}

int main(int argc, char **argv) {
  GrayImage *I,*J;
  AdjRel    *A;

  if (argc!=3) {
    fprintf(stderr,"usage:  linearfilter <image> <raio de adjacencia>\n");
    fprintf(stderr,"image:  arquivo de imagem pgm\n");    
    fprintf(stderr,"raio :  raio de adjacencia (e.g., 1.5)\n");    
    exit (-1);
  }

  I = ReadGrayImage(argv[1]);
  A = Circular(atof(argv[2]));
  J = MedianFilter(I,A);
  WriteGrayImage(J,"filtered.pgm");

  DestroyGrayImage(&I);
  DestroyGrayImage(&J);
  DestroyAdjRel(&A);


  return 0;
}

