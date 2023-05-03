/*
 * Assignment 2
 * Author: Nelaven Subaskaran
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
 * Function: power
 * ---------------
 *   Returns the power of a number
 *
 *   base: the base value
 *   exponent: the power value
 *
 *   returns: the base raised to the exponent
*/
double power(double base, int exponent) {
	double result = 1;
	for (int i = 0; i < exponent; ++i) {
		result *= base;
	}
	return result;
}

int main(int argc, char *argv[]) {
	if(argc != 3 || strlen(argv[1]) != 4 || strlen(argv[2]) != 4) {
		printf("Invalid arguments. Please provide only two integers between 1000-9999.\n");
		exit(1);
	}

	const int N = 4; // Length of the numbers provided

	int i = atoi(argv[1]);
	int j = atoi(argv[2]);
	printf("Your integers are %d %d\n", i, j);
	
	int fdP[2]; // Pipe for the child to communicate with parent
	int fdC[2]; // Pipe for the parent to communicate with child

	if(pipe(fdP) < 0) {
		printf("Pipe 1 failed.\n");
		exit(1);
	}

	if(pipe(fdC) < 0) {
		printf("Pipe 2 failed.\n");
		exit(1);
	}

	pid_t pid = fork();
	if(pid < 0) {
		printf("Fork was unsuccessful.\n");
		exit(1);
	}

	// Child process
	if(pid == 0) {
		close(fdP[0]);
		close(fdC[1]);

		int a1, a2, b1, b2, A, B, C, D;

		read(fdC[0], &a1, sizeof(a1));
		printf("	Child (PID %d): Received %d from parent\n", getpid(), a1);
		read(fdC[0], &b1, sizeof(b1));
		printf("	Child (PID %d): Received %d from parent\n", getpid(), b1);

		A = a1 * b1;
		write(fdP[1], &A, sizeof(A));
		printf("	Child (PID %d): Sending %d to parent\n", getpid(), A);

		read(fdC[0], &a1, sizeof(a1));
		printf("	Child (PID %d): Received %d from parent\n", getpid(), a1);
		read(fdC[0], &b2, sizeof(b2));
		printf("	Child (PID %d): Received %d from parent\n", getpid(), b2);

		B = a1 * b2;
		write(fdP[1], &B, sizeof(B));
		printf("	Child (PID %d): Sending %d to parent\n", getpid(), B);

		read(fdC[0], &a2, sizeof(a2));
		printf("	Child (PID %d): Received %d from parent\n", getpid(), a2);
		read(fdC[0], &b1, sizeof(b1));
		printf("	Child (PID %d): Received %d from parent\n", getpid(), b1);

		C = a2 * b1;
		write(fdP[1], &C, sizeof(C));
		printf("	Child (PID %d): Sending %d to parent\n", getpid(), C);

		read(fdC[0], &a2, sizeof(a2));
		printf("	Child (PID %d): Received %d from parent\n", getpid(), a2);
		read(fdC[0], &b2, sizeof(b2));
		printf("	Child (PID %d): Received %d from parent\n", getpid(), b2);

		D = a2 * b2;
		write(fdP[1], &D, sizeof(D));
		printf("	Child (PID %d): Sending %d to parent\n", getpid(), D);

		close(fdP[1]);
		close(fdC[0]);
	}

	// Parent porcess
	if(pid > 0){
		close(fdP[1]);
		close(fdC[0]);

		int a1 = i / 100;
		int a2 = i % 100;
		int b1 = j / 100;
		int b2 = j % 100;

		int A, B, C, D, X, Y, Z;

		printf("Parent (PID %d): created child (PID %d)\n", getpid(), pid);

		printf("\n###\n");
		printf("# Calculating X\n");
		printf("###\n");

		write(fdC[1], &a1, sizeof(a1));
		printf("Parent (PID %d): Sending %d to child\n", getpid(), a1);
		write(fdC[1], &b1, sizeof(b1));
		printf("Parent (PID %d): Sending %d to child\n", getpid(), b1);

		read(fdP[0], &A, sizeof(A));
		printf("Parent (PID %d): Received %d from child\n", getpid(), A);
		X = A * power(10, N);

		printf("\n###\n");
		printf("# Calculating Y\n");
		printf("###\n");

		write(fdC[1], &a1, sizeof(a1));
		printf("Parent (PID %d): Sending %d to child\n", getpid(), a1);
		write(fdC[1], &b2, sizeof(b2));
		printf("Parent (PID %d): Sending %d to child\n", getpid(), b2);

		read(fdP[0], &B, sizeof(B));
		printf("Parent (PID %d): Received %d from child\n", getpid(), B);

		write(fdC[1], &a2, sizeof(a2));
		printf("Parent (PID %d): Sending %d to child\n", getpid(), a2);
		write(fdC[1], &b1, sizeof(b1));
		printf("Parent (PID %d): Sending %d to child\n", getpid(), b1);

		read(fdP[0], &C, sizeof(C));
		printf("Parent (PID %d): Received %d from child\n", getpid(), C);
		Y = (B + C) * power(10, N/2);

		printf("\n###\n");
		printf("# Calculating Z\n");
		printf("###\n");

		write(fdC[1], &a2, sizeof(a2));
		printf("Parent (PID %d): Sending %d to child\n", getpid(), a2);
		write(fdC[1], &b2, sizeof(b2));
		printf("Parent (PID %d): Sending %d to child\n", getpid(), b2);

		read(fdP[0], &D, sizeof(D));
		printf("Parent (PID %d): Received %d from child\n", getpid(), D);
		Z = D * power(10, 0);

		close(fdP[0]);
		close(fdC[1]);

		wait(NULL);
		
		printf("\n%d*%d == %d + %d + %d == %d\n", i, j, X, Y, Z, (X + Y + Z));
	}

	return 0;
}
