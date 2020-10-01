/*** include ***/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <termios.h>
#include <inttypes.h>
#include <math.h>
#include <sys/ioctl.h>
#include"cfun.h"
/*** data ***/
struct editorConfig {
  struct termios orig_termios;
};

int timerOn;

struct timespec start, end;

struct editorConfig E;

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
	pc_magenta("yuh\n");
}

int editorReadKey() {
  int nread;
  char c;
  nread = read(STDIN_FILENO, &c, 1);
  return c;
}

/*** setup ***/

void setToCenter() {
	struct winsize ws;
	ioctl(0, TIOCGWINSZ, &ws);
	for (int i = 0; i < ws.ws_col/2; i++) {
		printf("%s"," ");
	}

}
/*** timer logic ***/

void startTimer() {
	timerOn = 1;
	clock_gettime(CLOCK_MONOTONIC_RAW, &start);
}

void endTimer() {
	clock_gettime(CLOCK_MONOTONIC_RAW, &end);
}

/*** i/o ***/

void readTimesFile() {
	ptr = fopen("/Users/louismeunier/dev/c:c++/timer/test/.txt","r+");
	fclose(fptr);
	
}

void eraseLine() {
	printf("\r         ");
	fflush(stdout);
}

/*** main ***/

int main() {

	printf("\e[?25l");
	setTerminalConfig();
	resetScreen();
	//printf("%s\n\r", "place" );
	//printf("\n");
	timerOn = 0;
	while (editorReadKey()!='q') {

		if (editorReadKey()==' ') {
			//system("clear");
			
			if (timerOn == 0) {
				startTimer();
				
				//printf("\r%c[2K", 27);
				eraseLine();
				setToCenter();
				printf("\r%s", "solve");
				fflush(stdout);

			} else {
				endTimer();
				uint64_t delta_us = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;
				
				float trunc_t = trunc((float)delta_us/10000);
				float final_t = trunc_t/100;
				//printf("\r%c[2K", 27);
				eraseLine();
				printf("\r%.6g", final_t);
				fflush(stdout);
				timerOn = 0;
			}			

		} else if (editorReadKey()=='r') {
			resetScreen();
		}
	};
	printf("\e[?25h");
	exit(0);
	return 0;
}