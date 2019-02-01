//
// lodprique.cxx
// priority queue functions for LOD (Level of Details)
// 
// Copyright (c) 2000 IPA and Keio University SFC Research Institution
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include "smd.h"

#include "veclib.h"
#include "matlib.h"
#include "file.h"
#include "time.h"

#include "lodprique.h"

LODPQHeap *create_lodpqheap( int size )
{
  LODPQHeap *pqh = (LODPQHeap *) malloc( sizeof(LODPQHeap) );
  pqh->pqcont = (LODPQCont *) malloc( size * sizeof(LODPQCont) );
  
  for ( int i = 0; i < size; ++i ) pqh->pqcont[i].id = i;
  pqh->size = size;
  pqh->last = 0;

  return pqh;
}

void swap_lodpqcont( LODPQCont *a, LODPQCont *b )
{
  double temp_length = a->length;
  a->length   = b->length;
  b->length   = temp_length;

  Sped *temp_ed = a->ed;
  a->ed   = b->ed;
  b->ed   = temp_ed;
  
  a->ed->pqc = a;
  b->ed->pqc = b;
}  

LODPQCont *insert_lodpqcont( double length, LODPQHeap *pqh, Sped *ed )
{
  if ( pqh->last >= pqh->size ) return NULL;

  pqh->pqcont[pqh->last].length = length;
  pqh->pqcont[pqh->last].ed     = ed;
  
  ed->pqc = &(pqh->pqcont[pqh->last]);
  ed->pq_type = EXIST;
  
  (void) lodpqcont_adjust_to_parent( pqh->last, pqh );

  ++(pqh->last);

  return ed->pqc;
}

// parent direction process
void lodpqcont_to_root( int id, LODPQHeap *pqh )
{
  int c = id;
  while ( TRUE ) {
    if ( !c ) break;
    int p = parent_num( c );
    swap_lodpqcont( &(pqh->pqcont[c]), &(pqh->pqcont[p]) );
    c = p;
  }
}

int lodpqcont_adjust_to_parent( int id, LODPQHeap *pqh )
{
  int c = id;
  while ( c > 0 ) {
    int p = parent_num( c );
    if ( pqh->pqcont[p].length > pqh->pqcont[c].length ) {
      swap_lodpqcont( &(pqh->pqcont[c]), &(pqh->pqcont[p]) );
      c = p;
    } else {
      return c;
    }
  }
  return c;
}

// child direction process
int lodpqcont_adjust_to_child( int id, LODPQHeap *pqh )
{
  int p = id;
  int c, i2l, i2r;
  while ( p <= parent_num(pqh->last-1) ) {

    // child: i2l, i2r
    i2l = 2*p+1; i2r = i2l+1;
    if ( i2r == pqh->last ) { // pqh->last does not exist.
      c = i2l;
    } else if ( pqh->pqcont[i2l].length < pqh->pqcont[i2r].length ) {
      c = i2l;
    } else {
      c = i2r;
    }
    
    if ( pqh->pqcont[p].length > pqh->pqcont[c].length ) {
      swap_lodpqcont( &(pqh->pqcont[p]), &(pqh->pqcont[c]) );
    } else {
      return p;
    }
    p = c;
  }
  return p;
}

int adjust_lodpqcont( Id id, LODPQHeap *pqh )
{
  // update values
  Sped *ed = pqh->pqcont[id].ed;
  pqh->pqcont[id].length = ed->error;
  
  // update heap trees
  int pid = lodpqcont_adjust_to_parent( id, pqh );
  int cid = lodpqcont_adjust_to_child( pid, pqh );

  return cid;
}

void deletemin_lodpqcont( LODPQHeap *pqh )
{
  // empty
  if ( pqh->last == 0 ) return;
  
  // delete mininum (root) 
  pqh->pqcont[0].ed->pq_type = DELETED;
  pqh->pqcont[0].ed->pqc     = NULL;

  pqh->pqcont[0].length = pqh->pqcont[pqh->last-1].length;
  pqh->pqcont[0].ed     = pqh->pqcont[pqh->last-1].ed;

  pqh->pqcont[0].ed->pqc = &(pqh->pqcont[0]);

  --(pqh->last);

  int i = 0;
  (void) lodpqcont_adjust_to_child( i, pqh );
}

void delete_lodpqcont( Id id, LODPQHeap *pqh )
{
  lodpqcont_to_root( id, pqh );
  deletemin_lodpqcont( pqh );
}

void free_lodpqheap( LODPQHeap *pqh )
{
  free( pqh->pqcont );
  free( pqh );
}

int parent_num( int child )
{
  return (int) (child - 1) / 2;
}
