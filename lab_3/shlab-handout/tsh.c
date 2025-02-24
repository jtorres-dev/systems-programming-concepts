/* 
 * tsh - A tiny shell program with job control
 * 
 * Jonathan Torres, jtorres1@uno.edu
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>

/* Misc manifest constants */
#define MAXLINE    1024   /* max line size */
#define MAXARGS     128   /* max args on a command line */
#define MAXJOBS      16   /* max jobs at any point in time */
#define MAXJID    1<<16   /* max job ID */

/* Job states */
#define UNDEF 0 /* undefined */
#define FG 1    /* running in foreground */
#define BG 2    /* running in background */
#define ST 3    /* stopped */

/* 
 * Jobs states: FG (foreground), BG (background), ST (stopped)
 * Job state transitions and enabling actions:
 *     FG -> ST  : ctrl-z
 *     ST -> FG  : fg command
 *     ST -> BG  : bg command
 *     BG -> FG  : fg command
 * At most 1 job can be in the FG state.
 */

/* Global variables */
extern char **environ;      /* defined in libc */
char prompt[] = "tsh> ";    /* command line prompt (DO NOT CHANGE) */
int verbose = 0;            /* if true, print additional output */
int nextjid = 1;            /* next job ID to allocate */
char sbuf[MAXLINE];         /* for composing sprintf messages */

struct job_t {              /* The job struct */
    pid_t pid;              /* job PID */
    int jid;                /* job ID [1, 2, ...] */
    int state;              /* UNDEF, BG, FG, or ST */
    char cmdline[MAXLINE];  /* command line */
};
struct job_t jobs[MAXJOBS]; /* The job list */
/* End global variables */


/* Function prototypes */

/* Here are the functions that you will implement */
void eval(char *cmdline);
int builtin_cmd(char **argv);
void do_bgfg(char **argv);
void do_redirect(char **argv);
void waitfg(pid_t pid);

void sigchld_handler(int sig);
void sigtstp_handler(int sig);
void sigint_handler(int sig);

/* Here are helper routines that we've provided for you */
int parseline(const char *cmdline, char **argv); 
void sigquit_handler(int sig);

void clearjob(struct job_t *job);
void initjobs(struct job_t *jobs);
int maxjid(struct job_t *jobs); 
int addjob(struct job_t *jobs, pid_t pid, int state, char *cmdline);
int deletejob(struct job_t *jobs, pid_t pid); 
pid_t fgpid(struct job_t *jobs);
struct job_t *getjobpid(struct job_t *jobs, pid_t pid);
struct job_t *getjobjid(struct job_t *jobs, int jid); 
int pid2jid(pid_t pid); 
void listjobs(struct job_t *jobs);

void usage(void);
void unix_error(char *msg);
void app_error(char *msg);
typedef void handler_t(int);
handler_t *Signal(int signum, handler_t *handler);

/*
 * main - The shell's main routine 
 */
int main(int argc, char **argv) {
    char c;
    char cmdline[MAXLINE];
    int emit_prompt = 1; /* emit prompt (default) */

    /* Redirect stderr to stdout (so that driver will get all output
     * on the pipe connected to stdout) */
    dup2(1, 2);

    /* Parse the command line */
    while ((c = getopt(argc, argv, "hvp")) != EOF) {
        switch (c) {
        case 'h':             /* print help message */
            usage();
	    break;
        case 'v':             /* emit additional diagnostic info */
            verbose = 1;
	    break;
        case 'p':             /* don't print a prompt */
            emit_prompt = 0;  /* handy for automatic testing */
	    break;
	default:
            usage();
	}
    }

    /* Install the signal handlers */

    /* These are the ones you will need to implement */
    Signal(SIGINT,  sigint_handler);   /* ctrl-c */
    Signal(SIGTSTP, sigtstp_handler);  /* ctrl-z */
    Signal(SIGCHLD, sigchld_handler);  /* Terminated or stopped child */

    /* Ignoring these signals simplifies reading from stdin/stdout */
    Signal(SIGTTIN, SIG_IGN);          /* ignore SIGTTIN */
    Signal(SIGTTOU, SIG_IGN);          /* ignore SIGTTOU */


    /* This one provides a clean way to kill the shell */
    Signal(SIGQUIT, sigquit_handler); 

    /* Initialize the job list */
    initjobs(jobs);

    /* Execute the shell's read/eval loop */
    while (1) {

	/* Read command line */
	if (emit_prompt) {
	    printf("%s", prompt);
	    fflush(stdout);
	}
	if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin))
	    app_error("fgets error");
	if (feof(stdin)) { /* End of file (ctrl-d) */
	    fflush(stdout);
	    exit(0);
	}

	/* Evaluate the command line */
	eval(cmdline);
	fflush(stdout);
	fflush(stdout);
    } 

    exit(0); /* control never reaches here */
}

/*
 * eval - Evaluate the command line that the user has just typed in
 * 
 * If the user has requested a built-in command (quit, jobs, bg or fg)
 * then execute it immediately. Otherwise, fork a child process and
 * run the job in the context of the child. If the job is running in
 * the foreground, wait for it to terminate and then return.  Note:
 * each child process must have a unique process group ID so that our
 * background children don't receive SIGINT (SIGTSTP) from the kernel
 * when we type ctrl-c (ctrl-z) at the keyboard.  
*/
void eval(char *cmdline) {
    int bg; // bg checker
    char *argv[MAXARGS]; // arguments that get checked 
    pid_t pid; // used to store pid during fork
    sigset_t mask; // gathered from Textbook pg. 765
    bg = parseline(cmdline, argv); // referencing Textbook source code
    struct job_t *job; 

    if(argv[0] == NULL) // if no arguments, return
	return;
    
    if(!builtin_cmd(argv)) {
	sigemptyset(&mask); // initializing the signal set to the empty set

        /* SIGCHLD ==>signal event that a child process has stopped or terminated
         * this is added to the empty set */
        sigaddset(&mask, SIGCHLD);

	/* blocks the signal set mask */
        sigprocmask(SIG_BLOCK, &mask, NULL);

        pid = fork(); // forking since it isn't a built in cmd. originally attempted to put fork on the outside but builtin cmds printed twice
    	
        if(pid < 0) // fork returns a negative value if fork encountered an exception
	    unix_error("Fork error");
		
	else if(pid == 0) { // if process id == 0, then it is a child process
    	    
	    /* sets all children to the same group process,
	     * when a signal is sent, all children in the same group receive it. */
	    setpgid(0,0);    

	    /* we unblock signal for child */
	    sigprocmask(SIG_UNBLOCK, &mask, NULL);
	    do_redirect(argv); // allows redirect to be a command

	    /* if not a builtin_cmd, this will execute
	     *
	     * execve returns a negative value if command not found
	     * otherwise execve executes the command via condition */
	    if(execve(argv[0], argv, environ) < 0) { 	    
	        printf("%s: Command not found\n", argv[0]);
	        exit(0);
	    }
        }
	// parent process (calling process)
        else {

            /* if background process from parseline arg */
            if(bg) {  
	        addjob(jobs, pid, BG, cmdline); // add job as a background process
    	        sigprocmask(SIG_UNBLOCK, &mask, NULL); // unblocking after adding job
	        job = getjobpid(jobs, pid); // used to get job info
	        printf("[%d] (%d) %s", job -> jid, job -> pid, job -> cmdline);
	    }	  
            /* else if user indicated foreground process */
            else {
	        addjob(jobs, pid, FG, cmdline); // add job as a foreground process
                sigprocmask(SIG_UNBLOCK, &mask, NULL); // unblocking after adding job
                waitfg(pid); // waits for foreground process to finish
	    }
        }
    } 
    return;
}

/* 
 * parseline - Parse the command line and build the argv array.
 * 
 * Characters enclosed in single quotes are treated as a single
 * argument.  Return true if the user has requested a BG job, false if
 * the user has requested a FG job.  
 */
int parseline(const char *cmdline, char **argv) {
    static char array[MAXLINE]; /* holds local copy of command line */
    char *buf = array;          /* ptr that traverses command line */
    char *delim;                /* points to first space delimiter */
    int argc;                   /* number of args */
    int bg;                     /* background job? */

    strcpy(buf, cmdline);
    buf[strlen(buf)-1] = ' ';  /* replace trailing '\n' with space */
    while (*buf && (*buf == ' ')) /* ignore leading spaces */
	buf++;

    /* Build the argv list */
    argc = 0;
    if (*buf == '\'') {
       	buf++;
	delim = strchr(buf, '\'');
    }
    else {
	delim = strchr(buf, ' ');
    }

    while (delim) {
	argv[argc++] = buf;
	*delim = '\0';
	buf = delim + 1;
	while (*buf && (*buf == ' ')) /* ignore spaces */
	       buf++;

	if (*buf == '\'') {
	    buf++;
	    delim = strchr(buf, '\'');
	}
	else {
	    delim = strchr(buf, ' ');
	}
    }
    argv[argc] = NULL;
    
    if (argc == 0)  /* ignore blank line */
	return 1;

    /* should the job run in the background? */
    if ((bg = (*argv[argc-1] == '&')) != 0) {
	argv[--argc] = NULL;
    }
    return bg;
}

/* 
 * builtin_cmd - If the user has typed a built-in command then execute
 *    it immediately.  
 */
int builtin_cmd(char **argv) {
    /* if user types jobs, list the jobs */
    if(!strcmp(argv[0], "jobs")) {
        listjobs(jobs);
	return 1;
    }
    /* if user types bg or fg, do_bgfg job */
    else if(!strcmp(argv[0], "bg") || !strcmp(argv[0], "fg")) {
        do_bgfg(argv);
	return 1;
    }
    /* if user types quit, exit shell */
    else if(!strcmp(argv[0], "quit"))
        exit(0);    

    return 0;     /* not a builtin command */
}


/* 
 * do_redirect - scans argv for any use of < or > which indicate input or output redirection
 *
 */
void do_redirect(char **argv) {
    int i;
    int fd;

    for(i=0; argv[i]; i++) {
	if (!strcmp(argv[i],"<")) {
	    /* add code for input redirection below */

	    /* fd is > 0 when no errors */
	    if((fd = open(argv[i+1], O_RDONLY)) > 0) 
	    	dup2(fd, 0); //dup2 duplicates the file descriptor to stdout
	    else
		printf("%s error: No such file or directory", argv[0]);   
	    
	    /* the line below cuts argv short -- this       
               removes the < and whatever follows from argv */
	    argv[i] = NULL;
	}
	else if (!strcmp(argv[i],">")) {
	    /* do stuff for output redirection here */
	
	    /* df is > 0 when no errors */
	    /* O_CREAT => if a parthname does not exist, it creates it as a regular file */
	    /* S_IRUSR has read permission */

            /* O_RDWR read and write == 2 */
	    if((fd = open(argv[i+1], O_RDWR | O_CREAT, S_IRUSR)) > 0)
		dup2(fd, 1); //dup2 duplicates the file descriptor to stdin

	    /* the line below cuts argv short -- this       
	       removes the > and whatever follows from argv */
	    argv[i] = NULL;
	}
    }
}

/* 
 * do_bgfg - Execute the builtin bg and fg commands
 */
void do_bgfg(char **argv) {
    
    int pid;
    int jid;
    struct job_t *job;

    // if user doesn't type in any argument after calling bg or fg   
    if(argv[1] == NULL) {
	printf("%s command requires PID or %%jobid argument\n", argv[0]);
	return;
    }

    else if(argv[1][0] == '%') { // checks if user typed for jid
 	/* if arg typed is not a number */
	if(!isdigit(argv[1][1])) {
            printf("%s: argument must be a PID or %%jobid\n", argv[0]);
            return;
    	}

	jid = atoi(&argv[1][1]); // converts jid string after % to an int and stores jid
    	job = getjobjid(jobs, jid); // creates job struct from jid
	
	if(job == NULL) { // if job doesnt exist
	    printf("%s: No such job\n", argv[1]);
	    return;
	}
    }

    else { // else if process
	
	/* if arg is not a number */
	if(!isdigit(argv[1][0])) { 
	    printf("%s: argument must be a PID or %%jobid\n", argv[0]);
	    return;
	}

        pid = atoi(argv[1]); // converts pid string to int and stores pid
        job = getjobpid(jobs, pid); // creates job struct from pid

        if(job == NULL) { // if job doesnt exist
	    printf("(%s): No such process\n", argv[1]);
	    return;
        }
    }

    kill(-(job -> pid), SIGCONT); // signal children to continue

    // if user has typed in fg
    if(!strcmp(argv[0], "fg")) {
	/* if state is currently in BG or ST */
	if(job -> state == BG || job -> state == ST)
	    job -> state = FG; // changes job state to foreground
    	waitfg(job -> pid);
    }

    // else if user typed in bg and current job state is in stopped state
    // needs to be specified that ST -> BG is the only transition for BG
    else if(!strcmp(argv[0], "bg")) {
	job -> state = BG; // changes job's state to bg	
	printf("[%d] (%d) %s", job -> jid, job -> pid, job -> cmdline);
	waitfg(job -> pid);
    }   
    return;
}

/* 
 * waitfg - Block until process pid is no longer the foreground process
 */
void waitfg(pid_t pid) {
    
    struct job_t *job;
    job = getjobpid(jobs, pid); // gets job of pid
    
    if(job == NULL)
	return;

    /* while still in the fg, wait to be bg */
    while(job -> state == FG)
   	pause();
    
    return;
}

/*****************
 * Signal handlers
 *****************/

/* 
 * sigchld_handler - The kernel sends a SIGCHLD to the shell whenever
 *     a child job terminates (becomes a zombie), or stops because it
 *     received a SIGSTOP or SIGTSTP signal. The handler reaps all
 *     available zombie children, but doesn't wait for any other
 *     currently running children to terminate.  
 */
void sigchld_handler(int sig) {  
    pid_t pid; // stores pid from waitpid to determine if child process still exist
    int status; // grabbed from textbook pg. 745
    struct job_t *job; // makes a current pid stopped from a signal

    // while loop updates pid every loop and breaks when no more children
    // to reap
    while((pid = waitpid(-1, &status, WNOHANG | WUNTRACED)) > 0) {

	// normal exit deletes all jobs
	if(WIFEXITED(status))
	    deletejob(jobs, pid);

	// else if CTRL-C (terminate)
	else if(WIFSIGNALED(status)) {
	    printf("Job [%d] (%d) terminated by signal %d\n", pid2jid(pid), pid, WTERMSIG(status));
	    deletejob(jobs, pid); // deletes jobs after notifying user that signal has terminated processes
	}

	// else if CTRL-Z (stop)
	else if(WIFSTOPPED(status)) {
	    job = getjobpid(jobs, pid); // storing current job that needs to be stopped
	    job -> state = ST; // stopping job is used to print to user the update
	    printf("Job [%d] (%d) stopped by signal %d\n", pid2jid(pid), pid, WSTOPSIG(status));
	} 
    }    	    
    return;
}

/* 
 * sigint_handler - The kernel sends a SIGINT to the shell whenver the
 *    user types ctrl-c at the keyboard.  Catch it and send it along
 *    to the foreground job.  
 */
void sigint_handler(int sig) {
    int pid;
    pid = fgpid(jobs); // getting foreground job pid
     
    if(pid != 0) // fgpid returns 0 if pid doesn't exist
	kill(-pid, SIGINT);

    return;
}

/*
 * sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
 *     the user types ctrl-z at the keyboard. Catch it and suspend the
 *     foreground job by sending it a SIGTSTP.  
 */
void sigtstp_handler(int sig) {
    int pid;
    pid = fgpid(jobs); // foreground job gets stored in pid to get current job

    if(pid != 0) 
	kill(-pid, SIGTSTP); // sends signal to all children to stop 
    
    return;
}

/*********************
 * End signal handlers
 *********************/

/***********************************************
 * Helper routines that manipulate the job list
 **********************************************/

/* clearjob - Clear the entries in a job struct */
void clearjob(struct job_t *job) {
    job->pid = 0;
    job->jid = 0;
    job->state = UNDEF;
    job->cmdline[0] = '\0';
}

/* initjobs - Initialize the job list */
void initjobs(struct job_t *jobs) {
    int i;

    for (i = 0; i < MAXJOBS; i++)
	clearjob(&jobs[i]);
}

/* maxjid - Returns largest allocated job ID */
int maxjid(struct job_t *jobs) {
    int i, max=0;

    for (i = 0; i < MAXJOBS; i++)
	if (jobs[i].jid > max)
	    max = jobs[i].jid;
    return max;
}

/* addjob - Add a job to the job list */
int addjob(struct job_t *jobs, pid_t pid, int state, char *cmdline) {
    int i;
    
    if (pid < 1)
	return 0;

    for (i = 0; i < MAXJOBS; i++) {
	if (jobs[i].pid == 0) {
	    jobs[i].pid = pid;
	    jobs[i].state = state;
	    jobs[i].jid = nextjid++;
	    if (nextjid > MAXJOBS)
		nextjid = 1;
	    strcpy(jobs[i].cmdline, cmdline);
  	    if(verbose){
	        printf("Added job [%d] %d %s", jobs[i].jid, jobs[i].pid, jobs[i].cmdline);
            }
            return 1;
	}
    }
    printf("Tried to create too many jobs\n");
    return 0;
}

/* deletejob - Delete a job whose PID=pid from the job list */
int deletejob(struct job_t *jobs, pid_t pid) {
    int i;

    if (pid < 1)
	return 0;

    for (i = 0; i < MAXJOBS; i++) {
	if (jobs[i].pid == pid) {
	    clearjob(&jobs[i]);
	    nextjid = maxjid(jobs)+1;
	    return 1;
	}
    }
    return 0;
}

/* fgpid - Return PID of current foreground job, 0 if no such job */
pid_t fgpid(struct job_t *jobs) {
    int i;

    for (i = 0; i < MAXJOBS; i++)
	if (jobs[i].state == FG)
	    return jobs[i].pid;
    return 0;
}

/* getjobpid  - Find a job (by PID) on the job list */
struct job_t *getjobpid(struct job_t *jobs, pid_t pid) {
    int i;

    if (pid < 1)
	return NULL;
    for (i = 0; i < MAXJOBS; i++)
	if (jobs[i].pid == pid)
	    return &jobs[i];
    return NULL;
}

/* getjobjid  - Find a job (by JID) on the job list */
struct job_t *getjobjid(struct job_t *jobs, int jid) {
    int i;

    if (jid < 1)
	return NULL;
    for (i = 0; i < MAXJOBS; i++)
	if (jobs[i].jid == jid)
	    return &jobs[i];
    return NULL;
}

/* pid2jid - Map process ID to job ID */
int pid2jid(pid_t pid) {
    int i;

    if (pid < 1)
	return 0;
    for (i = 0; i < MAXJOBS; i++)
	if (jobs[i].pid == pid) {
            return jobs[i].jid;
        }
    return 0;
}

/* listjobs - Print the job list */
void listjobs(struct job_t *jobs) {
    int i;
    
    for (i = 0; i < MAXJOBS; i++) {
	if (jobs[i].pid != 0) {
	    printf("[%d] (%d) ", jobs[i].jid, jobs[i].pid);
	    switch (jobs[i].state) {
		case BG: 
		    printf("Running ");
		    break;
		case FG: 
		    printf("Foreground ");
		    break;
		case ST: 
		    printf("Stopped ");
		    break;
	    default:
		    printf("listjobs: Internal error: job[%d].state=%d ", 
			   i, jobs[i].state);
	    }
	    printf("%s", jobs[i].cmdline);
	}
    }
}
/******************************
 * end job list helper routines
 ******************************/


/***********************
 * Other helper routines
 ***********************/

/*
 * usage - print a help message
 */
void usage(void) {
    printf("Usage: shell [-hvp]\n");
    printf("   -h   print this message\n");
    printf("   -v   print additional diagnostic information\n");
    printf("   -p   do not emit a command prompt\n");
    exit(1);
}

/*
 * unix_error - unix-style error routine
 */
void unix_error(char *msg) {
    fprintf(stdout, "%s: %s\n", msg, strerror(errno));
    exit(1);
}

/*
 * app_error - application-style error routine
 */
void app_error(char *msg) {
    fprintf(stdout, "%s\n", msg);
    exit(1);
}

/*
 * Signal - wrapper for the sigaction function
 */
handler_t *Signal(int signum, handler_t *handler) {
    struct sigaction action, old_action;

    action.sa_handler = handler;  
    sigemptyset(&action.sa_mask); /* block sigs of type being handled */
    action.sa_flags = SA_RESTART; /* restart syscalls if possible */

    if (sigaction(signum, &action, &old_action) < 0)
	unix_error("Signal error");
    return (old_action.sa_handler);
}

/*
 * sigquit_handler - The driver program can gracefully terminate the
 *    child shell by sending it a SIGQUIT signal.
 */
void sigquit_handler(int sig) {
    printf("Terminating after receipt of SIGQUIT signal\n");
    exit(1);
}
