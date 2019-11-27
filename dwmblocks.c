#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include<X11/Xlib.h>
#define LENGTH(X)               (sizeof(X) / sizeof (X[0]))

typedef struct {
	char* icon;
	char* command;
	unsigned int interval;
	unsigned int signal;
} Block;
void sighandler(int num);
void replace(char *str, char old, char new);
void getcmds(int time);
void getsigcmds(int signal);
void setupsignals();
int getstatus(char *str, char *last);
void setroot();
void statusloop();
void statusinit();
void sighandler(int signum);
void termhandler(int signum);


#include "blocks.h"

static Display *dpy;
static int screen;
static Window root;
static char statusbar[LENGTH(blocks)][50] = {0};
static char statusstr[2][256];
static int statusContinue = 1;
<<<<<<< HEAD
static void (*writestatus) () = setroot;
=======
>>>>>>> 5ff59d4e8ba9c64963d36c8ea51e7a1d644aef48

void replace(char *str, char old, char new)
{
	int N = strlen(str);
	for(int i = 0; i < N; i++)
		if(str[i] == old)
			str[i] = new;
}

//opens process *cmd and stores output in *output
void getcmd(const Block *block, char *output)
{
	strcpy(output, block->icon);
	char *cmd = block->command;
	FILE *cmdf = popen(cmd,"r");
	if (!cmdf)
		return;
	char c;
	int i = strlen(block->icon);
	while((c = fgetc(cmdf)) != EOF)
		output[i++] = c;
	if (delim != '\0' && --i)
		output[i++] = delim;
	output[i++] = '\0';
	pclose(cmdf);
}

void getcmds(int time)
{
	const Block* current;
	for(int i = 0; i < LENGTH(blocks); i++)
	{	
		current = blocks + i;
		if ((current->interval != 0 && time % current->interval == 0) || time == -1)
			getcmd(current,statusbar[i]);
	}
}

void getsigcmds(int signal)
{
	const Block *current;
	for (int i = 0; i < LENGTH(blocks); i++)
	{
		current = blocks + i;
		if (current->signal == signal)
			getcmd(current,statusbar[i]);
	}
}

void setupsignals()
{
	for(int i = 0; i < LENGTH(blocks); i++)
	{	  
		if (blocks[i].signal > 0)
			signal(SIGRTMIN+blocks[i].signal, sighandler);
	}

}

int getstatus(char *str, char *last)
{
	strcpy(last, str);
<<<<<<< HEAD
	str[0] = '\0';
	for(int i = 0; i < LENGTH(blocks); i++)
=======
	int j = 0;
	for(int i = 0; i < LENGTH(blocks); j+=strlen(statusbar[i++]))
>>>>>>> 5ff59d4e8ba9c64963d36c8ea51e7a1d644aef48
	{	
		strcat(str, statusbar[i]);
	}
<<<<<<< HEAD
	str[strlen(str)-1] = '\0';
=======
	str[--j] = '\0';
>>>>>>> 5ff59d4e8ba9c64963d36c8ea51e7a1d644aef48
	return strcmp(str, last);//0 if they are the same
}

void setroot()
{
	if (!getstatus(statusstr[0], statusstr[1]))//Only set root if text has changed.
		return;
	Display *d = XOpenDisplay(NULL);
	if (d) {
		dpy = d;
	}
	screen = DefaultScreen(dpy);
	root = RootWindow(dpy, screen);
	XStoreName(dpy, root, statusstr[0]);
	XCloseDisplay(dpy);
}

void pstdout()
{
	if (!getstatus(statusstr[0], statusstr[1]))//Only write out if text has changed.
		return;
	printf("%s\n",statusstr[0]);
	fflush(stdout);
}


void statusloop()
{
	setupsignals();
	int i = 0;
	getcmds(-1);
	while(statusContinue)
	{
		getcmds(i);
		writestatus();
		sleep(1.0);
		i++;
	}
}

void statusinit()
{
	statusloop();
}


void sighandler(int signum)
{
	getsigcmds(signum-SIGRTMIN);
	writestatus();
}

void termhandler(int signum)
{
	statusContinue = 0;
	exit(0);
}

int main(int argc, char** argv)
{
	for(int i = 0; i < argc; i++)
	{	
		if (!strcmp("-d",argv[i]))
			delim = argv[++i][0];
		else if(!strcmp("-p",argv[i]))
			writestatus = pstdout;
	}
	signal(SIGTERM, termhandler);
	signal(SIGINT, termhandler);
	statusinit();
}
