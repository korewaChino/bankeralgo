#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <stdbool.h>

#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 4

/* the available amount of each resource */
int available[NUMBER_OF_RESOURCES];

/*the maximum demand of each customer */
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

/* the amount currently allocated to each customer */
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

/* the remaining need of each customer */
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

// Funtion prototypes
void print_array(int array[], int size);
void print_matrix(int matrix[][NUMBER_OF_RESOURCES], int rows, int columns);
int request_resources(int customer_num, int request[]); 
void release_resources(int customer_num, int release[]);
void printError(int status);

// Helper functions

void print_array(int array[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

void print_matrix(int matrix[][NUMBER_OF_RESOURCES], int rows, int columns) {
    for (int i = 0; i < rows; i++) {
        print_array(matrix[i], columns);
    }
}

/**
 * Print error message based on status
 * -1: Request cannot be granted
 * -2: Invalid request
*/
void printError(int status) {
    if (status == -1) {
        printf("Request cannot be granted\n");
    } else if (status == -2) {
        printf("Invalid request\n");
    } else {
        printf("Unknown error\n");
    }
}

// Implementation
int request_resources(int customer_num, int request[]) {

   // Check if request is valid
    for (int resource = 0; resource < NUMBER_OF_RESOURCES; resource++) {
        if (request[resource] > need[customer_num][resource]) {
            printf("Request is greater than need\n");
            return -2;
        }

        if (request[resource] > available[resource]) {
            printf("Request is greater than available\n");
            return -2;
        }
    }

    // Try to allocate resources
    for (int resource = 0; resource < NUMBER_OF_RESOURCES; resource++) {
        available[resource] -= request[resource];
        allocation[customer_num][resource] += request[resource];
        need[customer_num][resource] -= request[resource];
    }

    // Check if state is safe
    int work[NUMBER_OF_RESOURCES];
    int finish[NUMBER_OF_CUSTOMERS];

    // Initialize work
    for (int resource = 0; resource < NUMBER_OF_RESOURCES; resource++) {
        work[resource] = available[resource];
    }

    // Initialize finish
    memset(finish, 0, sizeof(finish));

    /**
     * Find index of customer that can be finished
     * finish[i] = 0 (false)
     * Need[i] <= Work
    */
    step2:
    int customer = 0;
    for(;customer < NUMBER_OF_CUSTOMERS; customer++) {
        bool can_finish = true;

        if (finish[customer] == 0) {
            // Check every resource not exceeding work (available)
            for (int resource = 0; resource < NUMBER_OF_RESOURCES; resource++) {
                if (need[customer][resource] > work[resource]) {
                    can_finish = false;
                    break;
                }
            }
        }

        if (can_finish && finish[customer] == 0) {
            printf("Customer %d can finish\n", customer);
            goto step3;
        }
    }

    goto step4; // No such i exists, go to step 4

    step3:
    for(int resource = 0; resource < NUMBER_OF_RESOURCES; resource++) {
       work[resource] += allocation[customer][resource];
       finish[customer] = 1;
    }

    goto step2; // Check if other customers can finish

    // Check if all customers is finished
    step4:
    bool all_finished = true;
    for (int customer = 0; customer < NUMBER_OF_CUSTOMERS; customer++) {
        if (finish[customer] == 0) {
            printf("Customer %d cannot finish\n", customer);
            all_finished = 0;
            break;
        }
    }

    // If not all customers is finished, rollback
    if (!all_finished) {
        // Restore resources
        for (int resource = 0; resource < NUMBER_OF_RESOURCES; resource++) {
            available[resource] += request[resource];
            allocation[customer_num][resource] -= request[resource];
            need[customer_num][resource] += request[resource];
        }

        return -1;
    }

    return 0;
}

void release_resources(int customer_num, int release[]) {
    // Check if release is valid
    for (int resource = 0; resource < NUMBER_OF_RESOURCES; resource++) {
        if (release[resource] > allocation[customer_num][resource]) {
            printf("Release is greater than allocation\n");
            return;
        }
    }

    // Release resources
    for (int resource = 0; resource < NUMBER_OF_RESOURCES; resource++) {
        available[resource] += release[resource];
        allocation[customer_num][resource] -= release[resource];
        need[customer_num][resource] += release[resource];
    }

    printf("Resources released\n");

    return;
}

int main() {

    char max_request_file_path[100];
    scanf("%s", max_request_file_path);

    FILE *max_request_file = fopen(max_request_file_path, "r");
    
    /**
     * Get maximum demand of each customer
     * Each line in the file represents a customer
     * Each number in a line represents the maximum demand of a resource seperated by a comma
    */
    for (int customer = 0; customer < NUMBER_OF_CUSTOMERS; customer++) {
       for (int resource = 0; resource < NUMBER_OF_RESOURCES; resource++) {
           fscanf(max_request_file, "%d,", &maximum[customer][resource]);
       }
    }

    fclose(max_request_file);

    // Get Resources capacity
    for(int resource = 0; resource < NUMBER_OF_RESOURCES; resource++) {
        scanf("%d", &available[resource]);
    }

    /**
     * Calulate need
     * Need = Maximum - Allocation
    */
    for (int customer = 0; customer < NUMBER_OF_CUSTOMERS; customer++) {
        for (int resource = 0; resource < NUMBER_OF_RESOURCES; resource++) {
            need[customer][resource] = maximum[customer][resource] - allocation[customer][resource];
        }
    }

    /**
     * Command input
     * RQ - Request resources
     * RL - Release resources
     * * - Print current state
    */

    char command[2];
    int customer_num;
    int request[NUMBER_OF_RESOURCES];
    int release[NUMBER_OF_RESOURCES];

    while(1) {
        scanf("%s", command);

        // Print current state
        if (command[0] == '*') {
            printf("Available:\n");
            print_array(available, NUMBER_OF_RESOURCES);

            printf("Maximum:\n");
            print_matrix(maximum, NUMBER_OF_CUSTOMERS, NUMBER_OF_RESOURCES);

            printf("Allocation:\n");
            print_matrix(allocation, NUMBER_OF_CUSTOMERS, NUMBER_OF_RESOURCES);

            printf("Need:\n");
            print_matrix(need, NUMBER_OF_CUSTOMERS, NUMBER_OF_RESOURCES);
        } 
        // Request resources
        else if (command[0] == 'R' && command[1] == 'Q') {
            scanf("%d", &customer_num);
            for (int resource = 0; resource < NUMBER_OF_RESOURCES; resource++) {
                scanf("%d", &request[resource]);
            }

            printf("Requesting resources...\n");
            int status = request_resources(customer_num, request);

            if (status == 0) {
                printf("Request granted\n");
            } else {
                printError(status);
            }

        } 
        // Release Resources
        else if (command[0] == 'R' && command[1] == 'L') {
            scanf("%d", &customer_num);
            for (int resource = 0; resource < NUMBER_OF_RESOURCES; resource++) {
                scanf("%d", &release[resource]);
            }

            printf("Releasing resources...\n");
            release_resources(customer_num, release);
        } else {
            printf("Invalid command\n");
        }
    }

    return 0;
}