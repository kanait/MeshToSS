//
// lodprique.h
//
// Copyright (c) 1997-2000 Takashi Kanai; All rights reserved. 
//

#ifndef _LODPRIQUE_H
#define _LODPRIQUE_H

#ifdef __cplusplus
extern "C" {
#endif

extern LODPQHeap *create_lodpqheap(int);
extern void swap_lodpqcont(LODPQCont *, LODPQCont *);
extern LODPQCont *insert_lodpqcont(double, LODPQHeap *, Sped *);
extern void lodpqcont_to_root(int, LODPQHeap *);
extern int lodpqcont_adjust_to_parent(int, LODPQHeap *);
extern int lodpqcont_adjust_to_child(int, LODPQHeap *);
extern int adjust_lodpqcont(Id, LODPQHeap *);
extern void deletemin_lodpqcont(LODPQHeap *);
extern void delete_lodpqcont(Id, LODPQHeap *);
extern void free_lodpqheap(LODPQHeap *);
extern int parent_num(int);
  
#ifdef __cplusplus
}
#endif

#endif // _LODPRIQUE_H
