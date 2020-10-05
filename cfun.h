#include <stdio.h>

void pc_clear() {
	printf("\033[0m;");
}

void pc_red(char* arg) {
	printf("\033[0;31m");
	printf("%s",arg);
	printf("\033[0m;");
}

void pc_bred(char* arg) {
	printf("\033[1;31m");
	printf("%s",arg);
	printf("\033[0m;");
}

void pc_green(char* arg) {
	printf("\033[0;32m");
	printf("%s",arg);
	printf("\033[0m;");
}

void pc_bgreen(char* arg) {
	printf("\033[1;32m");
	printf("%s",arg);
	printf("\033[0m;");
}

void pc_yellow(char* arg) {
	printf("\033[0;33m");
	printf("%s",arg);
	printf("\033[0m;");
}

void pc_byellow(char* arg) {
	printf("\033[01;33m");
	printf("%s",arg);
	printf("\033[0m;");
}

void pc_blue(char* arg) {
	printf("\033[0;34m");
	printf("%s",arg);
	printf("\033[0m;");
}

void pc_bblue(char* arg) {
	printf("\033[1;34m");
	printf("%s",arg);
	printf("\033[0m");
} 

void pc_magenta(char* arg) {
	printf("\033[0;35m");
	printf("%s",arg);
	printf("\033[0m;");
}

void pc_bmagenta(char* arg) {
	printf("\033[1;35m");
	printf("%s",arg);
	printf("\033[0m;");
}

void pc_cyan(char* arg) {
	printf("\033[0;36m");
	printf("%s",arg);
	printf("\033[0m;");
}

void pc_bcyan(char* arg) {
	printf("\033[1;36m");
	printf("%s",arg);
	printf("\033[0m;");
}
