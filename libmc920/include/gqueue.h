#ifndef _GQUEUE_H_
#define _GQUEUE_H_

#include "common.h"

#define FIFOBREAK 0
#define LIFOBREAK 1
#define QSIZE     32768

#define SetTieBreak(a,b) a->C.tiebreak=b 

typedef struct _gnode { 
  int  next;  /* next node */
  int  prev;  /* prev node */
  char color; /* WHITE=0, GRAY=1, BLACK=2 */ 
} GNode;

typedef struct _gdoublylinkedlists {
  GNode *elem;  /* all possible doubly-linked lists of the circular queue */
  int nelems;  /* total number of elements */
  int *cost;   /* the cost of the nodes in the graph */
} GDoublyLinkedLists; 

typedef struct _gcircularqueue { 
  int  *first;   /* list of the first elements of each doubly-linked list */
  int  *last;    /* list of the last  elements of each doubly-linked list  */
  int  nbuckets; /* number of buckets in the circular queue */
  int  mincost;  /* minimum cost of a node in queue */
  int  maxcost;  /* maximum cost of a node in queue */
  char tiebreak; /* 1 is LIFO, 0 is FIFO (default) */
} GCircularQueue;

typedef struct _gqueue { /* Priority queue by Dial implemented as
                           proposed by A. Falcao */
  GCircularQueue C;
  GDoublyLinkedLists L;
} GQueue;

GQueue *CreateGQueue(int nbuckets, int nelems, int *cost);
void   DestroyGQueue(GQueue **Q);
void   ResetGQueue(GQueue *Q);
int    EmptyGQueue(GQueue *Q);
void   InsertGQueue(GQueue **Q, int elem);
int    RemoveGQueue(GQueue *Q);
void   RemoveGQueueElem(GQueue *Q, int elem);
void   UpdateGQueue(GQueue *Q, int elem, int newcost);
GQueue *GrowGQueue(GQueue **Q, int nbuckets);

#endif







