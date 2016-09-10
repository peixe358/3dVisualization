#ifndef _ADJACENCY_H_
#define _ADJACENCY_H_

/* Este módulo contém relações de adjacência invariantes à translação
   para os casos de imagens 2D e 3D */

#include "common.h"

/* Lista de spels adjacentes */

typedef struct _adjspel {
  int dx,dy,dz; /* deslocamento para chegar ao spel adjacente */   
} AdjSpel;

/* Relação de adjacência invariante à translação */

typedef struct _adjrel { 
  AdjSpel *adj; /* lista  de spels adjacentes */
  int        n; /* numero de spels adjacentes */
} AdjRel;


AdjRel *CreateAdjRel(int n); /* Aloca memória para relação de
				adjacência com n adjacentes */ 

AdjRel *Circular(float r); /* Cria relação de adjacência circular com
			      raio r */

void    DestroyAdjRel(AdjRel **A); /* Destrói relação de adjacência,
				      liberando a memória alocada */

AdjRel *Spheric(float r); /* Cria relação de adjacência esférica com
			     raio r */

#endif
