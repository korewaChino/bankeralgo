// Banker's algorithm, to avoid deadlocks

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

void print_array(int array[], int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%d ", array[i]);
    }
    printf("\n");
}

void print_matrix(int matrix[][NUMBER_OF_RESOURCES], int rows, int columns)
{
    for (int i = 0; i < rows; i++)
    {
        print_array(matrix[i], columns);
    }
}

#define eprintf(e) fprintf(stderr, e)

// Errors
// -1: Request cannot be granted
// -2: Invalid request
// Print to stderr because it's UNIX
void print_error(int status)
{
    // switch case
    switch (status)
    {
    case -1:
        eprintf("Request cannot be granted\n");
        break;
    case -2:
        eprintf("Invalid request\n");
        break;
    default:
        eprintf("Unknown error\n");
        break;
    }
}

struct request
{
    int customer_num;
    int request[NUMBER_OF_RESOURCES];
};

// the impl

bool check_request_validity(struct request *req)
{
    // assert if request is valid

    for (int resource = 0; resource < NUMBER_OF_RESOURCES; resource++)
    {
        if (req->request[resource] > need[req->customer_num][resource])
        {
            eprintf("Request is greater than need\n");
            return false;
        }

        if (req->request[resource] > available[resource])
        {
            eprintf("Request is greater than available\n");
            return false;
        }
    }
}

int check_availbility(int customer, int work[])
{
    for (int resource = 0; resource < NUMBER_OF_RESOURCES; resource++)
    {
        if (need[customer][resource] > work[resource])
        {
            return false;
        }
    }
    return true;
}

int find_finishable_customers(int work[], int finish[])
{
    int customers = 0;

    for (; customers < NUMBER_OF_CUSTOMERS; customers++)
    {
        if (finish[customers] == 0 && check_availbility(customers, work))
        {
            return customers;
        }
    }
}

int request_resource(struct request *req)
{
    // check if request is valid
    if (!check_request_validity(req))
    {
        return -2;
    }

    // try to alloc

    for (int resource = 0; resource < NUMBER_OF_RESOURCES; resource++)
    {
        available[resource] -= req->request[resource];
        allocation[req->customer_num][resource] += req->request[resource];
        need[req->customer_num][resource] -= req->request[resource];
    }

    // check safety

    int work[NUMBER_OF_RESOURCES];
    int finish[NUMBER_OF_CUSTOMERS];

    // init work
    for (int resource = 0; resource < NUMBER_OF_RESOURCES; resource++)
    {
        work[resource] = available[resource];
    }

    memset(finish, 0, sizeof(finish));

    // find index of customer that can be finished

    int customer = find_finishable_customers(work, finish);

    while (customer != -1)
    {
        // finish customer
        finish[customer] = 1;

        // add to work
        for (int resource = 0; resource < NUMBER_OF_RESOURCES; resource++)
        {
            work[resource] += allocation[customer][resource];
        }

        // find next customer
        customer = find_finishable_customers(work, finish);
    }

    return 0;
}

bool check_release_validity(int customer_num, int release[])
{
    for (int resource = 0; resource < NUMBER_OF_RESOURCES; resource++)
    {
        if (release[resource] > allocation[customer_num][resource])
        {
            eprintf("Release is greater than allocation\n");
            return false;
        }
    }
    return true;
}

// Releasing resources

void release_resources(int customer_num, int release[])
{
    for (int resource = 0; resource < NUMBER_OF_RESOURCES; resource++)
    {
        if (!check_release_validity)
        {
            return;
        }
    }

    // finally release
    for (int resource = 0; resource < NUMBER_OF_RESOURCES; resource++)
    {
        available[resource] += release[resource];
        allocation[customer_num][resource] -= release[resource];
        need[customer_num][resource] += release[resource];
    }

    printf("Resources released\n");

    return;
}

int main()
{

    char max_request_file_path[100];
    // printf("Specify input file path: ");
    scanf("%s", max_request_file_path);

    FILE *max_request_file = fopen(max_request_file_path, "r");

    // get max demand
    for (int customer = 0; customer < NUMBER_OF_CUSTOMERS; customer++)
    {
        for (int resource = 0; resource < NUMBER_OF_RESOURCES; resource++)
        {
            fscanf(max_request_file, "%d", &maximum[customer][resource]);
        }
    }

    fclose(max_request_file);

    // get resources capacity

    for (int resource = 0; resource < NUMBER_OF_RESOURCES; resource++)
    {
        scanf("%d", &available[resource]);
    }

    // calc need
    // = max - allocation

    for (int customer = 0; customer < NUMBER_OF_CUSTOMERS; customer++)
    {
        for (int resource = 0; resource < NUMBER_OF_RESOURCES; resource++)
        {
            need[customer][resource] = maximum[customer][resource] - allocation[customer][resource];
        }
    }

    // get requests

    char command[2];

    int customer_num;
    int request[NUMBER_OF_RESOURCES];
    int release[NUMBER_OF_RESOURCES];

    

    while (true)
    {
        // prompt
        // printf("> ");
        scanf("%s", command);

        if (strcmp(command, "*") == 0)
        {
            print_matrix(need, NUMBER_OF_CUSTOMERS, NUMBER_OF_RESOURCES);
            print_matrix(allocation, NUMBER_OF_CUSTOMERS, NUMBER_OF_RESOURCES);
            print_array(available, NUMBER_OF_RESOURCES);
        }
        else if (strcmp(command, "RQ") == 0)
        {
            scanf("%d", &customer_num);

            for (int resource = 0; resource < NUMBER_OF_RESOURCES; resource++)
            {
                scanf("%d", &request[resource]);
            }

            struct request req; /* = {
                .customer_num = customer_num,
                .request = &request,
            };*/ // initialization of ‘int’ from ‘int (*)[4]’ makes integer from pointer without a cast

            req.customer_num = customer_num;
            memcpy(req.request, request, sizeof(request));

            int status = request_resource(&req);

            if (status == -1)
            {
                print_error(status);
            }
        }
        else if (strcmp(command, "RL") == 0)
        {
            scanf("%d", &customer_num);

            for (int resource = 0; resource < NUMBER_OF_RESOURCES; resource++)
            {
                scanf("%d", &release[resource]);
            }

            printf("Releasing resources...\n");

            release_resources(customer_num, release);
        }
        else
        {
            printf("Invalid command\n");
        }
    }
}