#include "kernel.h"

Kernel *CreateKernel(AdjRel *A)
{
  Kernel *K=NULL;
  int     i;

  /* Cria kernel */

  K = (Kernel *)calloc(1,sizeof(Kernel ));
  K->A = CreateAdjRel(A->n);
  K->w = AllocFloatArray(A->n);

  /* Copia a relação de adjacência */

  for (i=0; i < A->n; i++) 
    K->A->adj[i] = A->adj[i];

  return(K);
} 

Kernel *ReadKernel2D(char *filename)
{
  FILE   *fp=fopen(filename,"r");
  Kernel *K=NULL;
  AdjRel *A=NULL;
  int     i,n;

  fscanf(fp,"%d",&n); 
  A = CreateAdjRel(n);
  K = CreateKernel(A);
   
  /* Ler os adjacentes */

  for (i=0; i < A->n; i++) {
    fscanf(fp,"%d %d %f",&K->A->adj[i].dx,&K->A->adj[i].dy,&K->w[i]); 
   }
   fclose(fp);

   DestroyAdjRel(&A);

   return(K);
}

Kernel *ReadKernel3D(char *filename)
{
  FILE   *fp=fopen(filename,"r");
  Kernel *K=NULL;
  AdjRel *A=NULL;
  int     i,n;

  fscanf(fp,"%d",&n); 
  A = CreateAdjRel(n);
  K = CreateKernel(A);
   
  /* Ler os adjacentes */

  for (i=0; i < A->n; i++) {
    fscanf(fp,"%d %d %d %f",&K->A->adj[i].dx,&K->A->adj[i].dy,&K->A->adj[i].dz,&K->w[i]); 
   }
   fclose(fp);

   DestroyAdjRel(&A);

   return(K);
}

void DestroyKernel(Kernel **K)
{
  Kernel *aux=*K;

 if (aux!=NULL){
   DestroyAdjRel(&aux->A);
   free(aux->w);
   free(aux);
   *K = NULL;
 }
}

Kernel *CircularGaussian(float r)
{
  Kernel *K=NULL;
  AdjRel *A=Circular(r);
  int     i;
  float   dx,dy,sigma=r/3.0,R2=2*sigma*sigma;
  float   sum=0.0;

  K = CreateKernel(A);
  DestroyAdjRel(&A);

  for (i=0; i < K->A->n; i++) {
    dx      = K->A->adj[i].dx;
    dy      = K->A->adj[i].dy;
    K->w[i] = expf(-(dx*dx + dy*dy)/R2);
    sum    += K->w[i];
  }

  for (i=0; i < K->A->n; i++) {
    K->w[i] /= sum;
  }

  return(K);
}

int MaximumDisplacement(Kernel *K)
{
  int i,dmax=INT_MIN; /* dmax = -infinito */

  for (i=0; i < K->A->n; i++) {
    if (dmax < abs(K->A->adj[i].dx))
      dmax = abs(K->A->adj[i].dx);
    if (dmax < abs(K->A->adj[i].dy))
      dmax = abs(K->A->adj[i].dy);
    if (dmax < abs(K->A->adj[i].dz))
      dmax = abs(K->A->adj[i].dz);
  }

  return(dmax);
}

Kernel   *ReflectKernel(Kernel *K)
{
  int i;
  Kernel *K1 = CreateKernel(K->A);

  for (i=0; i < K->A->n; i++) {
    K1->A->adj[i].dx = -K->A->adj[i].dx; 
    K1->A->adj[i].dy = -K->A->adj[i].dy; 
    K1->A->adj[i].dz = -K->A->adj[i].dz; 
    K1->w[i]         =  K->w[i];
  }

  return(K1);
}
