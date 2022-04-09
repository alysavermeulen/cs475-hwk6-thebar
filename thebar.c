/*
 * TheBar.c
 *
 *  Created on: Dec 26, 2015
 *      Author: dchiu
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include "globals.h"
#include "customer.h"
#include "bartender.h"

void printBanner();
void init();
void cleanup();

/**
 * Main function
 */
int main(int argc, char **argv)
{
	if (argc != 2 || atoi(argv[1]) == 0){
		printf("Usage: ./thebar [num customers]\n");
		exit(0);
	}
	num_threads = atoi(argv[1]); // number of customers
	int i;
	printBanner();
	init(); // initialize semaphores

	// create bartender and customer threads
	pthread_t *b = (pthread_t *)malloc(sizeof(pthread_t));
	pthread_t *c = (pthread_t *)malloc(num_threads * sizeof(pthread_t));

	// fire off bartender
	pthread_create(b, NULL, (void *)bartender, NULL);

	// create array for customer IDs
	int *ids = (int *)malloc(sizeof(int) * num_threads);

	// fire off customers
	for (i = 0; i < num_threads; i++)
	{
		ids[i] = i;
		pthread_create(&c[i], NULL, (void *)customer, (void *)&ids[i]);
	}

	// wait for threads to finish
	pthread_join(*b, NULL);
	for (i = 0; i < num_threads; i++)
	{
		pthread_join(c[i], NULL);
	}

	cleanup(); // cleanup and destroy semaphores
	free(b); // free bartender thread
	free(c); // free customer threads
	free(ids); // free array holding customer IDs

	return 0;
}

/**
 * Prints the activity banner.
 * Do not touch.
 */
void printBanner()
{
	printf("Customer:\t\t\t\t\t\t\t\t\t\t| Employee:\n");
	printf("Traveling\tArrived\t\tOrdering\tBrowsing\tAt Register\tLeaving");
	printf("\t| Waiting\tMixing Drinks\tAt Register\tPayment Recv\n");
	printf("----------------------------------------------------------------------------------------+");
	printf("-----------------------------------------------------------\n");
}

/**
 * Create and initialize semaphores
 */
void init()
{
	// unlink semaphores
	sem_unlink("/bar_empty");
    sem_unlink("/customer_entered");
    sem_unlink("/order_placed");
    sem_unlink("/drink_made");
    sem_unlink("/customer_paid");
	sem_unlink("/payment_received");

	// create semaphores
    bar_empty = sem_open("/bar_empty", O_CREAT, 0600, 1); // initialized to 1 (bar is empty)
    customer_entered = sem_open("/customer_entered", O_CREAT, 0600, 0);
    order_placed = sem_open("/order_placed", O_CREAT, 0600, 0);
    drink_made = sem_open("/drink_made", O_CREAT, 0600, 0);
    customer_paid = sem_open("/customer_paid", O_CREAT, 0600, 0);
	payment_received = sem_open("/payment_received", O_CREAT, 0600, 0);

}

/**
 * Cleanup and destroy semaphores
 */
void cleanup()
{
	// close semaphores
	sem_close(bar_empty);
	sem_close(customer_entered);
	sem_close(order_placed);
	sem_close(drink_made);
	sem_close(customer_paid);
	sem_close(payment_received);

	// delete semaphores
	sem_unlink("/bar_empty");
	sem_unlink("/customer_entered");
	sem_unlink("/order_placed");
	sem_unlink("/drink_made");
	sem_unlink("/customer_paid");
	sem_unlink("/payment_received");
}
