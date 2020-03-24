/***************************
 * M. Toups 1/28/2019      *
 * CSCI 2467 in-class demo *
 ***************************/

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
 int nsquared, n; /* two ints, each are 32 bits wide on modern systems */

 if (argc != 2) {
	 puts("Incorrect number of arguments. Provide a single int please.\n");
	 return 1;
 }

 n = strtol(argv[1],NULL,10); /* read command line arg */

/* perform some integer arithmetic and print results */
 nsquared = n * n;
 printf("  %5d * %5d == %10d (n) \n" , n, n, nsquared);

 return 0;
}
