/*
 * Assignment 5
 * Author: Nelaven Subaskaran
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t lock;

#define TRAIN_0_PASSENGERS 100
#define TRAIN_1_PASSENGERS 50

int s0 = 500, s1 = -50, s2 = -100, s3 = -250, s4 = -100;

struct arg_struct {
    int thread_id;
    int station_num;
    int num_passengers;
    int moving_back;
};

/*
 * Function: passengers
 * ---------------
 *   Pickup and unloading passengers at each station for the first
 *   and second train.
 *
 *   arguments: all the information about the train
 *
 *   returns: NULL
 */
void *passengers(void *arguments) {
    struct arg_struct *args = arguments;

    while (!(s0 == 0 && s1 == 0 && s2 == 0 && s3 == 0 && s4 == 0)) {
        pthread_mutex_lock(&lock);

        int train_num = args->thread_id;

        printf("Train %d ENTERS Station %d\n", train_num, args->station_num);

        if (args->station_num == 0) {
            args->moving_back = 0;
            printf("\tStation %d has %d passengers to pick up\n", args->station_num, s0);

            printf("\tPicking up passengers...\n");
            int max = train_num == 0 ? TRAIN_0_PASSENGERS : TRAIN_1_PASSENGERS;
            while (s0 > 0 && args->num_passengers < max) {
                s0--;
                args->num_passengers++;
            }
            sleep(args->num_passengers * 0.10);

            printf("\tTrain %d is at Station %d and has %d/%d passengers\n", train_num, args->station_num, args->num_passengers, max);
            printf("\tStation %d has %d passengers to pick up\n", args->station_num, s0);
            printf("Train %d LEAVES Station %d\n", train_num, args->station_num);

            args->station_num++;
        } else if (args->station_num == 1) {
            printf("\tStation %d has %d passengers to drop off\n", args->station_num, abs(s1));

            if (s1 < 0) {
                printf("\tUnloading passengers...\n");
                while (args->num_passengers > 0 && s1 < 0) {
                    args->num_passengers--;
                    s1++;
                }
                sleep(args->num_passengers * 0.10);
            }

            int max = train_num == 0 ? TRAIN_0_PASSENGERS : TRAIN_1_PASSENGERS;
            printf("\tTrain %d is at Station %d and has %d/%d passengers\n", train_num, args->station_num, args->num_passengers, max);
            printf("\tStation %d has %d passengers to drop off\n", args->station_num, abs(s1));
            printf("Train %d LEAVES Station %d\n", train_num, args->station_num);

            if (args->num_passengers > 0 && args->moving_back != -1) {
                args->station_num++;
            } else {
                args->station_num--;
            }
        } else if (args->station_num == 2) {
            printf("\tStation %d has %d passengers to drop off\n", args->station_num, abs(s2));

            if (s2 < 0) {
                printf("\tUnloading passengers...\n");
                while (args->num_passengers > 0 && s2 < 0) {
                    args->num_passengers--;
                    s2++;
                }
                sleep(args->num_passengers * 0.10);
            }

            int max = train_num == 0 ? TRAIN_0_PASSENGERS : TRAIN_1_PASSENGERS;
            printf("\tTrain %d is at Station %d and has %d/%d passengers\n", train_num, args->station_num, args->num_passengers, max);
            printf("\tStation %d has %d passengers to drop off\n", args->station_num, abs(s2));
            printf("Train %d LEAVES Station %d\n", train_num, args->station_num);

            if (args->num_passengers > 0 && args->moving_back != -1) {
                args->station_num++;
            } else {
                args->moving_back = -1;
                args->station_num--;
            }
        } else if (args->station_num == 3) {
            printf("\tStation %d has %d passengers to drop off\n", args->station_num, abs(s3));

            if (s3 < 0) {
                printf("\tUnloading passengers...\n");
                while (args->num_passengers > 0 && s3 < 0) {
                    args->num_passengers--;
                    s3++;
                }
                sleep(args->num_passengers * 0.10);
            }

            int max = train_num == 0 ? TRAIN_0_PASSENGERS : TRAIN_1_PASSENGERS;
            printf("\tTrain %d is at Station %d and has %d/%d passengers\n", train_num, args->station_num, args->num_passengers, max);
            printf("\tStation %d has %d passengers to drop off\n", args->station_num, abs(s3));
            printf("Train %d LEAVES Station %d\n", train_num, args->station_num);

            if (args->num_passengers > 0 && args->moving_back != -1) {
                args->station_num++;
            } else {
                args->moving_back = -1;
                args->station_num--;
            }
        } else if (args->station_num == 4) {
            printf("\tStation %d has %d passengers to drop off\n", args->station_num, abs(s4));

            if (s4 < 0) {
                printf("\tUnloading passengers...\n");
                while (args->num_passengers > 0 && s4 < 0) {
                    args->num_passengers--;
                    s4++;
                }
                sleep(args->num_passengers * 0.10);
            }

            int max = train_num == 0 ? TRAIN_0_PASSENGERS : TRAIN_1_PASSENGERS;
            printf("\tTrain %d is at Station %d and has %d/%d passengers\n", train_num, args->station_num, args->num_passengers, max);
            printf("\tStation %d has %d passengers to drop off\n", args->station_num, abs(s4));
            printf("Train %d LEAVES Station %d\n", train_num, args->station_num);

            args->moving_back = -1;
            args->station_num--;
        }

        pthread_mutex_unlock(&lock);

        sleep(1);
    }

    return NULL;
}

int main() {
    struct arg_struct *args1 = (struct arg_struct *)malloc(sizeof(struct arg_struct));
    args1->thread_id = 0;
    args1->station_num = 0;
    args1->num_passengers = 0;
    args1->moving_back = -1;

    struct arg_struct *args2 = (struct arg_struct *)malloc(sizeof(struct arg_struct));
    args2->thread_id = 1;
    args2->station_num = 0;
    args2->num_passengers = 0;
    args2->moving_back = -1;

    pthread_t train_0_thread, train_1_thread;

    pthread_mutex_init(&lock, NULL);

    pthread_create(&train_0_thread, NULL, passengers, (void *)args1);
    pthread_create(&train_1_thread, NULL, passengers, (void *)args2);

    pthread_join(train_0_thread, NULL);
    pthread_join(train_1_thread, NULL);

    pthread_mutex_destroy(&lock);

    return 0;
}