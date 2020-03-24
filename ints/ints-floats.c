/***************************
 * M. Toups 8/31/2017      *
 * CSCI 2467 in-class demo *
 ***************************/

#include <stdio.h>

int main()
{
  int i1, iresult;
  float f1, f2;

/* do some integer arithmetic using value i1 */
  i1 = 50000;
/* iresult = i1 squared -- not using pow() function */
  iresult = i1 * i1; 
/* now print the operation and its result */
  printf("\n  %d * %d == %d \n\n" , i1, i1, iresult);

/* now do some arithemetic on floats and print results */

 f1 = 1e20 - (1e20 + 3.2);
 f2 = (1e20 - 1e20) + 3.2;

 printf("\n  1e20 - (1e20 + 3.2) == %.16f (f1)\n" , f1);
 printf(  " (1e20 - 1e20) + 3.2  == %.16f (f2)\n" , f2);

 /* also try: 44143793053630464.0 + 1 */

 printf("\n 44143793053630464.0 + 1 ==  %f \n" , 44143793053630464.0 + 1);

 return 0;
}
