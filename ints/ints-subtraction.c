/***************************************
 * M. Toups Fall 2017                  *
 * CSCI 2467 in-class demo (modified)  *
 ***************************************/

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  int i1, i2, iresult;

/* i1 is a negative value close to but not equal to TMin */
  i1 = -2147483645;

 if (argc != 2) {
         printf("Incorrect number of arguments. What number would you like to subtract from %d?\n", i1);
         return 1;
 }

 i2 = strtol(argv[1],NULL,10); /* read command line arg */


/* what happens when we take a negative number and subtract a positive number? */
//  iresult = i1 - i2; /* the same as i1 + (~i2 +1) */
  iresult = i1 + (~i2 + 1); /* the same as i1 - i2 in the line above */
/* we would expect iresult to be negative, right? */

/* now print the operation and its result */
  printf("\n  %d - %d == %d \n\n" , i1, i2, iresult);

  return 0; /* finished */
}
