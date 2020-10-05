/*** include ***/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <termios.h>
#include <inttypes.h>
#include <math.h>
#include <sys/ioctl.h>
#include <string.h>


/*** data ***/

struct editorConfig {
  struct termios orig_termios;
};

int timerOn;

int sysDia = 0;

struct timespec start, end;

struct editorConfig E;

float ao5[5];

FILE *fptr;

/*** setup ***/
//This is currently unused, and doesn't work with the current formatting
void setToCenter(int len) {
	struct winsize ws;
	printf("\r");
	ioctl(0, TIOCGWINSZ, &ws);
	for (int i = 0; i < (ws.ws_col/2)-((len-len%2)/2); i++) {
		printf("%s"," ");
	}
}


/*** config ***/

void resetTerminalCongif() {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_termios);
}

void setTerminalConfig() {
	tcgetattr(STDIN_FILENO, &E.orig_termios);
    atexit(resetTerminalCongif);
    struct termios raw = E.orig_termios;

	tcgetattr(STDIN_FILENO, &raw);
	raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  	raw.c_oflag &= ~(OPOST);
  	raw.c_cflag |= (CS8);
  	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	
	raw.c_cc[VMIN] = 0;
  	raw.c_cc[VTIME] = 1;
  	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void resetScreen() {
	system("clear");
	setToCenter(sizeof("~~~~le timer~~~~"));
	printf("\033[1;34m");
	printf("~~~~le timer~~~~\n");
	printf("\033[0m");
}

int editorReadKey() {
  int nread;
  char c;
  nread = read(STDIN_FILENO, &c, 1);
  return c;
}


/*** timer logic ***/

void startTimer() {
	timerOn = 1;
	clock_gettime(CLOCK_MONOTONIC_RAW, &start);
}

float endTimer() {
	clock_gettime(CLOCK_MONOTONIC_RAW, &end);
	uint64_t delta_us = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;
					
	float trunc_t = trunc((float)delta_us/10000);
	float final_t = trunc_t/100;

	return final_t;
}

/*** i/o ***/

void writeTime(float num) {
	fptr = fopen("/Users/louismeunier/dev/c:c++/timer/test_times.txt","a");
	
    fprintf(fptr, "\r%.6g", num);

	fclose(fptr);
}

void readTime() {
	fptr = fopen("/Users/louismeunier/dev/c:c++/timer/test_times.txt","r");
	
	char input[255];
	char *status1;

	do {
	    status1 = fgets(input, sizeof(input), fptr);
	    
	    //change this later to return times etc..
	    printf("\r%s", input);
	    fflush(stdout);
	} while (status1);

}

void eraseLine() {
	printf("\r                         ");
	fflush(stdout);
}

/*** avg logic ***/

void cmdArrow() {

	printf("\033[01;33m");
	printf("\r%s","=>");
	printf("\033[0m");
	fflush(stdout);
}

void addToAO5(float ctime) {
	float buffOne = ao5[1];
	ao5[1] = ao5[0];
	ao5[0] = ctime;

	float buffTwo = ao5[2];
	ao5[2] = buffOne;

	float buffThree = ao5[3];
	ao5[3] = buffTwo;

	float buffFour = ao5[4];
	ao5[4] = buffThree;
} 

void bestSingle() {
	int c, size = 5, location = 0;
	for (c = 1; c < size; c++)
    	if (ao5[c] < ao5[location])
      	location = c;
    eraseLine();
    cmdArrow();
    printf(" best single: ");
    printf("\033[0;32m");
    printf("%.6g", ao5[location]);
    printf("\033[0m");
    fflush(stdout);
}

void calcAO5() {
	int c, size = 5, location = 0, location1 = 0;

    for (c = 1; c < size; c++)
    	if (ao5[c] < ao5[location])
      	location = c;
    for (int i = 1; i < size; i++)
    	if (ao5[i] > ao5[location1])
		location1 = i;

	float total;
	for (int i=0;i<5;i++) 
		if (i!=location && i!=location1)
			total+=ao5[i];
	float average = total/3;
	float rAverage = round(average*100);

	cmdArrow();
	printf(" ao5: %.6g", rAverage/100);
	fflush(stdout);
}

/*** timer cmds***/
void toggleCMD() {
	eraseLine();
	printf("\r%s","cmd toggled..." );
	fflush(stdout);
	sysDia = abs(sysDia-1);
}

/*** main ***/

int main() {

	printf("\e[?25l");
	setTerminalConfig();
	resetScreen();

	timerOn = 0;

	while (editorReadKey()!='q') {
		if (editorReadKey()=='y' ) {
			toggleCMD();
		}
		if (sysDia==0) {
			if (editorReadKey()==' ') {
				if (timerOn == 0) {
					startTimer();
					
					eraseLine();

					printf("\r%s", "solve");
					fflush(stdout);
	
				} else {
					float final_t = endTimer();
					
					eraseLine();
					addToAO5(final_t);

					printf("\r%.6g", final_t);
					fflush(stdout);
	
					timerOn = 0;
				}			
	
			}
		} else {
			if (editorReadKey()=='r') {
				resetScreen();
			} else if (editorReadKey()=='t') {
				eraseLine();
				cmdArrow();
				printf(" %.6g,%.6g,%.6g,%.6g,%.6g", ao5[0],ao5[1],ao5[2],ao5[3],ao5[4]);
				fflush(stdout);
			} else if (editorReadKey()=='f') {
				eraseLine();
				calcAO5();
			} else if (editorReadKey()=='b') {
				bestSingle();
			} 
		}
		
	};

	printf("\e[?25h");
	exit(0);
	return 0;
}