#ifndef _KERNEL_H_
#define _KERNEL_H_

#include "common.h"
#include "adjacency.h"

typedef struct _kernel { 
  AdjRel *A; /* relação de adjacência  */
  float  *w; /* pesos dos adjacentes   */
} Kernel;


Kernel   *CreateKernel(AdjRel *A); /* Aloca memória para armazenar os
				      pesos e a relação de adjacência */
 
Kernel   *ReadKernel2D(char *filename); /* Ler adjacentes e pesos de um
		  			   arquivo de entrada */

Kernel   *ReadKernel3D(char *filename); /* Ler adjacentes e pesos de um
		  			   arquivo de entrada */

void      DestroyKernel(Kernel **K); /* Destrói kernel liberando a
					memória */

Kernel   *CircularGaussian(float r); /* Cria kernel Gaussiano 2D com raio r */

int       MaximumDisplacement(Kernel *K); /* Retorna o maior
					     descolamento para um spel
					     adjacente no
					     kernel. Usada para saber
					     de quanto aumenta a
					     imagem após
					     correlação/convolução */

Kernel   *ReflectKernel(Kernel *K); /* Reflete o kernel em torno da
				       sua origem, para cálculo de
				       convolução */

#endif
