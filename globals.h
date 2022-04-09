/*
 * globals.h
 *
 *  Created on: Dec 26, 2015
 *      Author: dchiu
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

int num_threads;	// number of customer threads
int now_serving;	// customer's ID who is being served

// semaphores
sem_t *bar_empty;
sem_t *customer_entered;
sem_t *order_placed;
sem_t *drink_made;
sem_t *customer_paid;
sem_t *payment_received;

#endif /* GLOBALS_H_ */
