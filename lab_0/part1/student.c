#include <string.h>
#include "student.h"

struct student_info make_student(void){
   struct student_info me;

   me.id = 2471307;
  
   strcpy(me.name, "Jonathan Torres"); /* strcpy is necessary */
   strcpy(me.name_pref, "John");
  
   strcpy(me.email, "jtorres1@uno.edu");
   strcpy(me.personal_email, "jonathanissocool@live.com");
  
   strcpy(me.city, "New Orleans");
  
   me.num_of_semesters = 7;
   
   me.csci_classes[0]=1205;
   me.csci_classes[1]=1581;
   me.csci_classes[2]=1583;
   me.csci_classes[3]=2120;
   me.csci_classes[4]=2121;
   me.csci_classes[5]=2125; /* array indices always being with 0 */
   me.csci_classes[6]=2450;


   strcpy(me.want_from_2467, "The main thing I want to get out of this class is to be more familiar with linux and commands that have great uses. I would like to learn the differences in between terminal, kernel, shell, scripts, and to correct anything I have misunderstood from it.");

   strcpy(me.reason, "I am studying CS because of the complex nature behind them. They can do so much yet not everything. I believe that CS will continue to leave me dumbfounded until I am no longer alive.");

   strcpy(me.difficult_program, "The most difficult program that I have made so far was a hash table that contained different chars to choose from for a string that needed to have chars to be swapped. I also wrote ballclock program in Data Structures which was neat.");

  strcpy(me.learned_from_CS, "I have learned from CS that you will never know everything as things always evolve in CS. Recursion on knowledge and life.");
  
  strcpy(me.interest, "I like playing competitive video games such as Rocket League, Overwatch, and Counter Strike Global Offensive. I enjoy listening to different sounds created by different sound engineers who take the time to create an interesting piece.");
  
   return me;
}
