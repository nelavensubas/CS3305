/*
Assignment 1
Author: Nelaven Subaskaran
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "checkPassword.h"

int main(int argc, char *argv[]) {
	int fork_request = 0;
	int hackme_request = 0;

	if(argc > 3) {
		printf("Too many arguments supplied.\n");
		exit(1);
	}

	int a;
	for(a = 0; a < argc; a++) {
		if(strcmp(argv[a], "-f") == 0) {
			fork_request = 1;
		} else if(strcmp(argv[a], "-p") == 0) {
			hackme_request = 1;
		}
	}

	if(fork_request == 0) {
		int i, j, k;
		int comb1[3], comb2[3], comb3[3], comb4[3];

		// Check three characters at a time without forking
		for(i = 33; i < 127; i++) {
			for(j = 33; j < 127; j++) {
				for(k = 33; k < 127; k++) {
					char combination[3] = {i, j, k};
					if(checkPassword(combination, 0) == 0) {
						comb1[0] = i;
						comb1[1] = j;
						comb1[2] = k;
					} else if(checkPassword(combination, 3) == 0) {
						comb2[0] = i;
						comb2[1] = j;
						comb2[2] = k;
					} else if(checkPassword(combination, 6) == 0) {
						comb3[0] = i;
						comb3[1] = j;
						comb3[2] = k;
					} else if(checkPassword(combination, 9) == 0) {
						comb4[0] = i;
						comb4[1] = j;
						comb4[2] = k;
					}
				}
			}
		}

		printf("%c%c%c\n", comb1[0], comb1[1], comb1[2]);
		printf("%c%c%c\n", comb2[0], comb2[1], comb2[2]);
		printf("%c%c%c\n", comb3[0], comb3[1], comb3[2]);
		printf("%c%c%c\n", comb4[0], comb4[1], comb4[2]);
	} else {
		// Check three characters at a time, but with forking
		int pid1, pid2, pid3;

		pid1 = fork();
		if(pid1 < 0) {
			printf("Fork was unsuccessful\n");
			exit(1);
		} else if(pid1 == 0) {
			printf("PID 1.1 IS %d. PPID 1.0 IS %d\n", getpid(), getppid());
			fflush(stdout);

			pid2 = fork();
			if(pid2 < 0) {
				printf("Fork was unsuccessful\n");
				exit(1);
			} else if(pid2 == 0) {
				printf("PID 1.2 IS %d. PPID 1.0 IS %d\n", getpid(), getppid());
				fflush(stdout);

				int i, j, k;
				for(i = 33; i < 127; i++) {
					for(j = 33; j < 127; j++) {
						for(k = 33; k < 127; k++) {
							char combination[3] = {i, j, k};
							if(checkPassword(combination, 9) == 0) {
								printf("%c%c%c\n", i, j, k);
								fflush(stdout);
							}
						}
					}
				}

				return 0;
			} else {
				printf("PID 1.1.1 IS %d. PPID 1.1 IS %d\n", getpid(), getppid());
				fflush(stdout);

				int i, j, k;
				for(i = 33; i < 127; i++) {
					for(j = 33; j < 127; j++) {
						for(k = 33; k < 127; k++) {
							char combination[3] = {i, j, k};
							if(checkPassword(combination, 6) == 0) {
								printf("%c%c%c\n", i, j, k);
								fflush(stdout);
							}
						}
					}
				}

				wait(NULL);
			}

			return 0;
		} else {
			printf("PID 1.0 IS %d. CHILD 1.1 IS %d\n", getpid(), getppid());
			fflush(stdout);

			pid3 = fork();
			if(pid3 < 0) {
				printf("Fork was unsuccessful\n");
				exit(1);
			} else if(pid3 == 0) {
				printf("PID 1.1 IS %d. CHILD 1.1.1 IS %d\n", getpid(), getppid());
				fflush(stdout);

				int i, j, k;
				for(i = 33; i < 127; i++) {
					for(j = 33; j < 127; j++) {
						for(k = 33; k < 127; k++) {
							char combination[3] = {i, j, k};
							if(checkPassword(combination, 3) == 0) {
								printf("%c%c%c\n", i, j, k);
								fflush(stdout);
							}
						}
					}
				}

				return 0;
			} else {
				printf("PID 1.0 IS %d. CHILD 1.2 IS %d\n", getpid(), getppid());
				fflush(stdout);

				int i, j, k;
				for(i = 33; i < 127; i++) {
					for(j = 33; j < 127; j++) {
						for(k = 33; k < 127; k++) {
							char combination[3] = {i, j, k};
							if(checkPassword(combination, 0) == 0) {
								printf("%c%c%c\n", i, j, k);
								fflush(stdout);
							}
						}
					}
				}

				wait(NULL);
			}

			wait(NULL);
		}
	}

	// Automatically run hackme file if it was requested
	if(hackme_request == 1) {
		execl("hackme", "", NULL);
	} else {
		return 0;
	}
}
