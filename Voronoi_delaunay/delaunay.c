/* Fares Belhadj 20/05/2005 */
/* amsi@ai.univ-paris8.fr   */
#include "main.h"
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <time.h>
static Point2D *      generatePoints(int);
static void           initTriangleList(TriangleList **);
static void           cercleCirconscrit(Triangle *);
static void           delaunay(TriangleList ** ptl, Point2D * pl, int nbPoints);
static void           addPointToTriangulation(TriangleList ** ptl, Point2D * p);
static SegmentList ** addTriangleSegments(SegmentList ** tpsl, Triangle * T);
static void           deleteDoubleSegment(SegmentList **);
static void           addInTriangleList(TriangleList **, Point2D *, Point2D *, Point2D *);
static void           deleteCornerTriangles(TriangleList ** ptl);
static SegmentList *  makeFinalSegmentsList(TriangleList * tl);
static void           makeUniqueSegment(SegmentList ** psl);
static void           freeTriangles(TriangleList * tl);
extern SegmentList * generateDelaunay(int nbPoints) {
  TriangleList * tl;
  SegmentList * sl;
  Point2D * pl;
  if(nbPoints < 1) return NULL;
  pl = generatePoints(nbPoints);
  initTriangleList(&tl);
  delaunay(&tl, pl, nbPoints);
  free(pl);
  deleteCornerTriangles(&tl);
  sl = makeFinalSegmentsList(tl);
  freeTriangles(tl);
  return sl;
}
extern void freeSegments(SegmentList * sl) {
  SegmentList * tmp;
  while(sl) {
    tmp = sl;
    sl = sl->next;
    free(tmp);
  }
}
/* Generer un nuage de points */
static Point2D * generatePoints(int nbPoints) {
  int i;
  Point2D * p;
  p = malloc(nbPoints * sizeof p[0]);
  assert(p);
  srand(time(NULL));
  for(i = 0; i < nbPoints; i++) {
    p[i].x = (int) (((double)WIDTH  * rand())/(RAND_MAX + 1.0));
    p[i].y = (int) (((double)HEIGHT * rand())/(RAND_MAX + 1.0));
  }
  return p;
}
/* Initialiser la liste des triangles avec 2 triangles :
 * T1 : (GAUCHE, HAUT), (DROITE, HAUT), (GAUCHE, BAS)
 * T2 : (GAUCHE, BAS), (DROITE, HAUT), (DROITE, BAS) */
static void initTriangleList(TriangleList ** ptl) {
  *ptl = malloc(sizeof (*ptl)[0]); assert(*ptl);
  (*ptl)->T.p1.x = GAUCHE; (*ptl)->T.p1.y = HAUT;
  (*ptl)->T.p2.x = DROITE; (*ptl)->T.p2.y = HAUT;
  (*ptl)->T.p3.x = GAUCHE; (*ptl)->T.p3.y =  BAS;
  cercleCirconscrit(&((*ptl)->T));
  ptl = &((*ptl)->next);
  *ptl = malloc(sizeof (*ptl)[0]); assert(*ptl);
  (*ptl)->T.p1.x = GAUCHE; (*ptl)->T.p1.y =  BAS;
  (*ptl)->T.p2.x = DROITE; (*ptl)->T.p2.y = HAUT;
  (*ptl)->T.p3.x = DROITE; (*ptl)->T.p3.y =  BAS;
  cercleCirconscrit(&((*ptl)->T));
  (*ptl)->next = NULL;
}
/* Calculer le cercle circonscrit du triangle T */
static void cercleCirconscrit(Triangle * T) {
  double d1, d2, d3, c1, c2, c3, c, c23, c31, c12, dc;
  Point2D ptmp1, ptmp2;
  ptmp1.x = T->p3.x - T->p1.x; ptmp1.y = T->p3.y - T->p1.y;
  ptmp2.x = T->p2.x - T->p1.x; ptmp2.y = T->p2.y - T->p1.y;
  d1 = Scalar(ptmp1, ptmp2);
  ptmp1.x = T->p3.x - T->p2.x; ptmp1.y = T->p3.y - T->p2.y;
  ptmp2.x = T->p1.x - T->p2.x; ptmp2.y = T->p1.y - T->p2.y;
  d2 = Scalar(ptmp1, ptmp2);
  ptmp1.x = T->p1.x - T->p3.x; ptmp1.y = T->p1.y - T->p3.y;
  ptmp2.x = T->p2.x - T->p3.x; ptmp2.y = T->p2.y - T->p3.y;
  d3 = Scalar(ptmp1, ptmp2);
  c1 = d2 * d3;
  c2 = d3 * d1;
  c3 = d1 * d2;
  c = c1 + c2 + c3;
  T->C = malloc(sizeof T->C[0]); assert(T->C);
  T->C->r = 0.5 * sqrt(((d1 + d2) * (d2 + d3) * (d3 + d1)) / c);
  dc = 2. * c; c23 = (c2 + c3) / dc; c31 = (c3 + c1) / dc; c12 = (c1 + c2) / dc;
  T->C->c.x = (c23 * T->p1.x) + (c31 * T->p2.x) + (c12 * T->p3.x);
  T->C->c.y = (c23 * T->p1.y) + (c31 * T->p2.y) + (c12 * T->p3.y);
}
/* Construire par recurence le delaunay :
 * A chaque iteration, ajouter un point 
 * au delaunay existant */
static void delaunay(TriangleList ** ptl, Point2D * pl, int nbPoints) {
  int i;
  for(i = 0; i < nbPoints; i++)
    addPointToTriangulation(ptl, &pl[i]);
}
/* Ajouter un point a la triangulation existante
 * et creer une nouvelle triangulation */
static void addPointToTriangulation(TriangleList ** ptl, Point2D * p) {
  Cercle c;
  TriangleList * tl = *ptl;
  SegmentList * psl = NULL, * tmp, ** tpsl;
  tpsl = &psl;
  /* DETECTER LES TRIANGLES DONT LE CERCLE CIRCONSCRIT INCLU LE POINT */
  /* CREER UNE LISTE DE SEGMENTS A PARTIR DE CES TRIANGLES            */
  /* SUPPRIMER CES TRIANGLES                                          */
  while(*ptl) {
    c = (*((*ptl)->T.C));
    if(IsInTheCircle((*p), c)) {
      tpsl = addTriangleSegments(tpsl, &((*ptl)->T));
      tl = *ptl;
      *ptl = (*ptl)->next;
      free(tl);
    }
    else
      ptl = &((*ptl)->next);
  }
  /* SUPPRIMER LES SEGMENTS EN DOUBLES */
  deleteDoubleSegment(&psl);
  /* AJOUTER LES NOUVEAUX TRIANGLES    */
  tpsl = &psl;
  while(*tpsl) {
    addInTriangleList(ptl, p, &((*tpsl)->S.p1), &((*tpsl)->S.p2));
    tmp = *tpsl;
    *tpsl = (*tpsl)->next;
    free(tmp);
    ptl = &((*ptl)->next);
  }
}
/* Ajouter les segments du nouveau Triangle
 * a la liste des segments */
static SegmentList ** addTriangleSegments(SegmentList ** tpsl, Triangle * T) {
  *tpsl = malloc(sizeof (*tpsl)[0]); assert(*tpsl);
  (*tpsl)->S.p1 = T->p1;
  (*tpsl)->S.p2 = T->p2;
  (*tpsl)->next = NULL;
  tpsl = &((*tpsl)->next);
  *tpsl = malloc(sizeof (*tpsl)[0]); assert(*tpsl);
  (*tpsl)->S.p1 = T->p2;
  (*tpsl)->S.p2 = T->p3;
  (*tpsl)->next = NULL;
  tpsl = &((*tpsl)->next);
  *tpsl = malloc(sizeof (*tpsl)[0]); assert(*tpsl);
  (*tpsl)->S.p1 = T->p3;
  (*tpsl)->S.p2 = T->p1;
  (*tpsl)->next = NULL;
  tpsl = &((*tpsl)->next);
  return tpsl;
}
/* Supprimer toute occurence d'un segment
 * se trouvant en double dans la liste des segments */
static void deleteDoubleSegment(SegmentList ** psl) {
  SegmentList ** pt1 = psl, ** pt2, * tmp;
  int flag;
  while(*pt1) {
    flag = 1;
    for(pt2 = &((*pt1)->next); *pt2; ) {
      if(IsSameSegment(((*pt1)->S), ((*pt2)->S))) {
	tmp = *pt2;
	*pt2 = (*pt2)->next;
	free(tmp);
	flag = 0;
      }
      else
	pt2 = &((*pt2)->next);
    }
    if(flag)
      pt1 = &((*pt1)->next);
    else {
      tmp = *pt1;
      *pt1 = (*pt1)->next;
      free(tmp);
    }
  }
}
/* Ajouter le nouveau triangle a la liste des triangles */
static void 
addInTriangleList(TriangleList ** ptl, Point2D * p1, Point2D * p2, Point2D * p3) {
  *ptl = malloc(sizeof (*ptl)[0]); assert(*ptl);
  (*ptl)->T.p1.x = p1->x; (*ptl)->T.p1.y = p1->y;
  (*ptl)->T.p2.x = p2->x; (*ptl)->T.p2.y = p2->y;
  (*ptl)->T.p3.x = p3->x; (*ptl)->T.p3.y = p3->y;
  (*ptl)->next   = NULL;
  cercleCirconscrit(&((*ptl)->T));
}
/* Supprimer les triangles initiaux (initTriangleList)
 * de la liste des triangles */
static void deleteCornerTriangles(TriangleList ** ptl) {
  int i, d;
  Point2D * p[3];
  TriangleList * tl;
  while(*ptl) {
    d = 0;
    p[0] = &((*ptl)->T.p1); p[1] = &((*ptl)->T.p2); p[2] = &((*ptl)->T.p3);
    for(i = 0; i < 3; i++)
      if(IsOnCorner(*(p[i]))) {
	d = 1;
	tl = *ptl;
	*ptl = (*ptl)->next;
	free(tl);
	break;
      }
    if(!d)
      ptl = &((*ptl)->next);
  }
}
/* Fabriquer la liste definitive de segments */
static SegmentList * makeFinalSegmentsList(TriangleList * tl) {
  SegmentList * sl, ** psl;
  psl = &sl;
  while(tl) {
    psl = addTriangleSegments(psl, &(tl->T));
    tl = tl->next;
  }
  makeUniqueSegment(&sl);
  return sl;
}
/* Supprimer les occurences en double des segments
 * de la liste des segments */
static void makeUniqueSegment(SegmentList ** psl) {
  SegmentList ** pt1 = psl, ** pt2, * tmp;
  while(*pt1) {
    for(pt2 = &((*pt1)->next); *pt2; ) {
      if(IsSameSegment(((*pt1)->S), ((*pt2)->S))) {
	tmp = *pt2;
	*pt2 = (*pt2)->next;
	free(tmp);
      }
      else
	pt2 = &((*pt2)->next);
    }
    pt1 = &((*pt1)->next);
  }
}
/* Liberer la liste des triangles */
static void freeTriangles(TriangleList * tl) {
  TriangleList * tmp;
  while(tl) {
    tmp = tl;
    tl = tl->next;
    free(tmp);
  }
}
