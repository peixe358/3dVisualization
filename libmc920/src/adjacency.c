#include "adjacency.h"

AdjRel *CreateAdjRel(int n)
{
   AdjRel *A=NULL;

   A      = (AdjRel *)calloc(1,sizeof(AdjRel ));
   A->adj = (AdjSpel *) calloc(n,sizeof(AdjSpel));
   A->n   = n;

  return(A);
} 

AdjRel *Circular(float r)
{
  AdjRel *A=NULL;
  int     dx,dy,n,i,R=(int)r,i0=0;
  float   R2=r*r;

  n=0;
  for (dy=-R; dy <= R; dy++)
    for (dx=-R; dx <= R; dx++){
      if ((dx*dx + dy*dy)<=R2)
	n++;
    }
  n++;
  A = CreateAdjRel(n);
  i=0;
  for (dy=-R; dy <= R; dy++)
    for (dx=-R; dx <= R; dx++)
      if ((dx*dx + dy*dy)<=R2){
	if ((dx==0)&&(dy==0)) i0 = i;
	A->adj[i].dx = dx;
	A->adj[i].dy = dy;
	A->adj[i].dz = 0;
	i++;
      }

  /* shift to right and place central voxel at first */

  for (i=i0; i > 0; i--) {
    dx = A->adj[i].dx;
    dy = A->adj[i].dy;
    A->adj[i].dx = A->adj[i-1].dx;
    A->adj[i].dy = A->adj[i-1].dy;
    A->adj[i-1].dx = dx;
    A->adj[i-1].dy = dy;
  }

  return(A);
}

AdjRel *Spheric(float r)
{
  AdjRel *A=NULL;
  int     dx,dy,dz,n,i,R=(int)r,i0=0; 
  float   R2=r*r;
  
  n=0;
  for (dz=-R; dz <= R; dz++)
    for (dy=-R; dy <= R; dy++)
      for (dx=-R; dx <= R; dx++)
	if ((dx*dx + dy*dy + dz*dz)<=R2)
	  n++;
  n++;
  A = CreateAdjRel(n);
  i=0;
  for (dz=-R; dz <= R; dz++)
    for (dy=-R; dy <= R; dy++)
      for (dx=-R; dx <= R; dx++)	
	if ((dx*dx + dy*dy + dz*dz)<=R2){
	  if ((dx==0)&&(dy==0)&&(dz==0)) i0 = i;
	  A->adj[i].dx = dx;
	  A->adj[i].dy = dy;
	  A->adj[i].dz = dz;
	  i++;
	}

  /* shift to right and place central voxel at first */

  for (i=i0; i > 0; i--) {
    dx = A->adj[i].dx;
    dy = A->adj[i].dy;
    dz = A->adj[i].dz;
    A->adj[i].dx = A->adj[i-1].dx;
    A->adj[i].dy = A->adj[i-1].dy;
    A->adj[i].dz = A->adj[i-1].dz;
    A->adj[i-1].dx = dx;
    A->adj[i-1].dy = dy;
    A->adj[i-1].dz = dz;
  }
  
  return(A);
}

void DestroyAdjRel(AdjRel **A)
{
  AdjRel *aux=*A;

 if (aux!=NULL){
    free(aux->adj);
    free(aux);
    *A = NULL;
  }
}

