/* student.h */
/* define a struct for storing student information */

struct student_info {
   int id;
   char name[80]; /* 80 characters should be more than enough to store a name, right? */

   char name_pref[10]; /* Short nickname, not necessarily something I'd prefer but I wouldn't mind it. */
   char email[30]; /* email address entry for me */
   char personal_email[40]; /*just doing this to see if it works as intended */   
   char city[30];
   int num_of_semesters;

   
	   /* need more stuff here */
   int csci_classes[20]; /* has anyone taken more than 20 CS classes? */

   char reason[400]; /* This means my reason for studying CS must be
                        less than 400 characters, or I will have to 
                        increase the array size. */
   char want_from_2467[500];
   char reason_for_CS[500];
   char difficult_program[500];
   char learned_from_CS[500];
   char interest[500];

};

/* this is a function prototype, the function itself is in student.c */
struct student_info make_student(void);

