/* part1.c -- This is part 1 of lab 0, CSCI2467, Fall 2016 */

#include <stdio.h>
#include "student.h"

int main () /* in this case we don't care about command line arguments */
{
	/* create a struct of type student_info called student */
	/* then call the make_student() function from student.c
	 * to fill it with information */
  struct student_info student = make_student (); 

  /* Now start printing out what's stored in student.
   * \n represents the newline character
   * %d is for substituting integer values */
  printf ("ID number: %d\n\n", student.id);

  /* %s is for substituting a character array */
  printf ("Name: %s\n\n", student.name);
  
  printf("Nickname: %s\n\n", student.name_pref);

  printf("Email: %s\nPersonal Email: %s\n\n", student.email, student.personal_email);

  printf("Where I Spawned: %s\n\n", student.city);
  
  /* we reference individual array elements with [0]-[6] etc */
  printf ("CS Courses taken: \nCSCI%d\nCSCI%d\nCSCI%d\nCSCI%d\nCSCI%d\nCSCI%d\nCSCI%d\n\n",
	  student.csci_classes[0], student.csci_classes[1], student.csci_classes[2], 
	  student.csci_classes[3], student.csci_classes[4], student.csci_classes[5], 
	  student.csci_classes[6]);

  printf("What I want out of this class:\n%s\n\n", student.want_from_2467);
  printf("Reason for studying Computer Science:\n%s\n\n", student.reason);
  printf("Most Complicated Programs:\n%s\n\n", student.difficult_program);
  printf("Something in CS Learned (Other than programming):\n%s\n\n", student.learned_from_CS);
  printf("Interests:\n%s\n\n", student.interest);

  return 0; /* 0 is typically returned when no errors have occurred */
};
