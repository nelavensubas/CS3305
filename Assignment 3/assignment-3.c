/*
 * Assignment 3
 * Author: Nelaven Subaskaran
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

long totalSum = 0;
int totalCount = 0, highestInt = 0, threadDiv = 0;

/*
 * Function: primeCheck
 * ---------------
 *   Check to see if a number is a prime or not
 *
 *   num: the number to check
 *
 *   returns: 0 if the number is a prime, otherwise 1 if
 *   it's not a prime
 */
int primeCheck(int num)
{
    int flag = 0;

    if (num == 0 || num == 1)
    {
        flag = 1;
    }

    for (int i = 2; i <= num / 2; ++i)
    {
        if (num % i == 0)
        {
            flag = 1;
            break;
        }
    }

    return flag;
}

/*
 * Function: findAllPrimes
 * ---------------
 *   Find all the primes from a certain range, sum up
 *   the primes, add the sum to the total sum, and add
 *   the number of primes found to the total count.
 *
 *   num: the id to indicate which thread is being used
 *
 *   returns: NULL
 */
void *findAllPrimes(void *i)
{
    long id = (long)i;

    int low = (id == 0) ? 0 : threadDiv * (id);
    int high = (id == 0) ? threadDiv : threadDiv * (id + 1);
    high = high > highestInt ? highestInt : high;

    printf("Thread # %ld is finding primes from low = %d to high = %d\n", id, low, high);

    long sum = 0;
    int count = 0;

    while (low < high)
    {
        if (primeCheck(low) == 0)
        {
            sum += low;
            count += 1;
        }
        low += 1;
    }

    totalSum += sum;
    totalCount += count;

    printf("Sum of thread %ld is %ld, Count is %d\n", id, sum, count);

    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Proper usage is ./assignment-3 <threadCount> <highestInt>\n");
        exit(1);
    }

    int numThreads = atoi(argv[1]);
    highestInt = atoi(argv[2]);
    threadDiv = (highestInt + (numThreads - 1)) / numThreads;

    pthread_t threads[numThreads];

    for (long i = 0; i < numThreads; i++)
    {
        pthread_create(&threads[i], NULL, &findAllPrimes, (long *)i);
    }

    for (long i = 0; i < numThreads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("\n       GRAND SUM IS %ld, COUNT IS %d\n", totalSum, totalCount);

    return 0;
}
