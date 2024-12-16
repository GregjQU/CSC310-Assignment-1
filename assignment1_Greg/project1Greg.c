#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
//hello

typedef struct Node {
	int prime;
	struct Node* next; // possibly try nextAfter comnd if does not work
} Node;


Node*head = NULL;
Node*tail = NULL;
int count = 0;
int top_range;
pthread_mutex_t lock;

bool is_prime( int num) {
	
	if (num <= 1) return false;
	for (int i = 2; i*i <= num; i++) {
		if (num %i == 0) return false;
	}
	return true;
}

void add_prime( int prime){
	Node * new_node = (Node*)malloc(sizeof(Node));
	new_node -> prime = prime;
	new_node -> next = NULL;
	pthread_mutex_lock(&lock);
	//Besa
	if (tail == NULL){
	head = tail = new_node;
	} else {
	tail ->next = new_node;
	tail = new_node;
	
	}
	count++;
	pthread_mutex_unlock(&lock);
	
}
void* find_primes(void*arg) {
	for (int i =1; i <= top_range; i ++) {
	if (is_prime(i)) {
		add_prime(i);
	} sleep(1);
	
	}
	return NULL;
}

// besa

void print_top_n (int n){
	pthread_mutex_lock( &lock);
	Node * current = head;
	for (int i = 0; i < n && current != NULL; i ++){
		printf("%d" , current -> prime);
		current = current -> next;
		}
		printf("\n");
		pthread_mutex_unlock(&lock);
		
	//besa // double check this 
}
void print_largest_prime(){
	pthread_mutex_lock(&lock);
	if (tail != NULL) {
		printf("Largest prime: %d\n", tail -> prime);
	} else {
		printf("no primes found.\n");
	}
	pthread_mutex_unlock( &lock);
}
void print_help() {
	 printf("Commands:\n");
	 printf("t/T: Print the top N values found. N is the smaller of 5 and the number found\n");
	 printf("l/L: Print the largest prime found (if there is one)\n");
	 printf("h/H: Print this help message\n");
	 printf("c/C: The count of how many numbers have been processed\n");
	 printf("q/Q: Quit the program early\n");
	 
}
void print_count(){
pthread_mutex_lock(&lock);
printf("Count of numbers processed: %d\n", count);
pthread_mutex_unlock(&lock);
} 

void write_primes_to_file(){
	FILE* file = fopen("primes.txt", "w"); // double check "primes.txt"
	if (file == NULL) {
		perror("Failed to open file");
		return;
	}
	pthread_mutex_lock (&lock);
	Node* current = head;
	while (current != NULL) {
		fprintf(file, "%d\n", current ->prime);
		current= current ->next;
	}
	pthread_mutex_unlock(&lock);
	fclose(file);
	
}

int main (int argc, char* argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <top_range>\n", argv[0]);
		return 1;
	}
	top_range = atoi(argv[1]);
	pthread_t prime_thread;
	pthread_mutex_init(&lock,NULL);
	
	if (pthread_create(&prime_thread, NULL, find_primes, NULL) != 0) {
		perror ("Failed to create thread");
		return 1;
		
	}
	char command;
	while (true){
		
		printf("Enter command: ");
		scanf(" %c", &command);
		switch (command) {
			case 't':
			case 'T':
			print_top_n( count < 5 ? count : 5);
			break;
			case 'l':
			case 'L':
			print_largest_prime();
			break;
			case 'h':
			case 'H':
			print_help();
			break;
			case 'c':
			case 'C':
			print_count();
			break;
			
			case 'q':
			case 'Q' :
			pthread_cancel(prime_thread);
			write_primes_to_file();
			//pthread_mutexattr_destroy(&lock);
			pthread_mutex_destroy(&lock);
			return 0;
			default:
			printf("unknown commands. Type 'h' for help.\n");
			
			
		}
		
		
	}
	
	pthread_join(prime_thread, NULL);
	write_primes_to_file();
	pthread_mutex_destroy(&lock);
	return 0;
	
	
	
	
}

