/* CSCI 2467 / M. Toups */
/* compile with:  gcc structdebug.c -o structdebug -ggdb */
/* then debug with: gdb structdebug */

#include <string.h>
#include <stdio.h>

struct student_info {
   int id;    /* 4-byte (32-bit) int */
   float gpa; /* or this could be:  double gpa; */
   short age; /* 2-byte (16-bit) short int */
   char name[36]; /* 36 characters */
   int csci_classes[5]; /* 5 ints x 4-bytes each = 20b */
};

/* function prototype */
struct student_info make_student(void);

/* our main function */
void main(void)
{
 struct student_info stu = make_student();

 int myclasses[5] = {2120, 2450, 0, 0, -1};
 
 printf("myclasses[4] = %d\n", myclasses[4]);

 return;
}

/* make_student returns a struct student_info */
struct student_info make_student(void){
   struct student_info me;

// first zero-out the struct
   bzero(&me, sizeof(me));

   me.id = 1688987;
   strcpy(me.name, "my name here"); /* strcpy is necessary */

   me.age=1023; /* one thousand twenty three! */

   me.gpa=9.999999; /* impossible GPA! */

   me.csci_classes[0]=2120; /* array indices always being with 0 */
   me.csci_classes[1]=2450;
   me.csci_classes[4]=-1; /* negative one will look cool in hex */

   return me;
}
