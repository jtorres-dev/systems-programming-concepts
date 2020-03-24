/* by: Matthew Toups
   mtoups@cs.uno.edu

   Based on copyij and copyji functions described in
   Bryant & O'Hallaron's Computer Systems: A Programmer's Perspective
   text.

   August 15, 2016
*/

#include <stdio.h>
#include <stdlib.h>

void copyji (int src[2048][2048], int dst[2048][2048])
{
  int i, j;
  for (j = 0; j < 2048; j++)
    for (i = 0; i < 2048; i++)
      dst[i][j] = src[i][j];
}

void main ()
{
/* use malloc to grab appropriate size block of memory */
  int **A = malloc (2048 * 2048 * sizeof (int));
  int **B = malloc (2048 * 2048 * sizeof (int));

/* we could set values, or just leave whatever values are already in 
   memory since we don't actually care what is inside the matrix */

  printf ("Running copyji...");
  copyji ((int (*)[2048]) A, (int (*)[2048]) B);
  printf (" done!\n");

  return;
}
