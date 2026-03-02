#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structures

// Waiting List - Queue 
typedef struct waiting
{
    int contact, busNum, seat;
    char name[50];
    struct waiting *next;
} waiting;

// Ticket History - Stack 
typedef struct passenger
{
    int contact, busNum, seat, book;
    char name[50];
    struct passenger *next;
} passenger;

// Seat - Linked List 
typedef struct seat
{
    int num, book;
    struct seat *next;
} seat;

// Manage Schedule - Buses - Binary Search Tree 
typedef struct bus
{
    int busNum, seatCount;
    char source[30], destination[30], time[10];
    struct bus *left, *right;
    seat *seats;
} bus;

// Global Variables

waiting *waitingList = NULL; // Front for Queue
passenger *stack = NULL;     // Top for Stack
seat *head = NULL;           // Head for Linked List
bus *root = NULL;            // Root for Binary Search Tree
int serial = 1;              // Serial Number for print;

// Function Prototypes

// waiting list functions 

waiting *createWaiting(int contact, int busNum, int seat, char name[50]);
void enqueueWaiting(int contact, int busNum, int seat, char name[50]);
void dequeueWaiting();
void printWaiting();

// ticket history functions - Kamy
passenger *newPassenger(int contact, int busNum, int seat, int book, char name[50]);
void pushPassenger(passenger **stack, int contact, int busNum, int seat, int book, char name[50]);
void printstack(passenger *stack);
void clearHistory(passenger **stack);
void popPassenger(passenger **stack);

// Seats functions 
seat *creatSeat(int num);
seat *insertSeat(seat *head, int num);
seat *searchSeat(seat *head, int num);

// Bus Schedule functions 
bus *createBus(int busNum, char source[30], char destination[30], char time[10], int seatCount);
bus *addBus(bus *root, int busNum, char source[30], char destination[30], char time[10], int seatCount);
bus *findMin(bus *root);
void freeSeats(seat *head);
bus *removeBus(bus *root, int busNum);
bus *searchBus(bus *root, int busNum);
void displayBus(bus *root);

// Ticket Booking functions 
void seatMap(bus *Bus);
void cancelTicket(bus *Bus);

// Others

// Clearing Terminal
void clearTerminal()
{
#ifdef _WIN32
    system("cls"); // For Windows
#else
    system("clear"); // For UNIX/Linux
#endif
}

// Save & Load
void saveBuses(bus *root, FILE *file);
void saveBusData();
void loadBusData();
void saveWaitingList();
void loadWaitingList();
void saveTicketHistory();
void loadTicketHistory();
void saveBusSeats(bus *Bus);
void saveAllBusSeats(bus *root);
void loadBusSeats(bus *Bus);
void loadAllBusSeats(bus *root);
void saveAllData();
void loadAllData();

// Memory Management
void freeBuses(bus *root);
void freeWaitingList();
void freePassengerStack();

// User Interface 
void manageTicket();
void manageSchedule();
void viewTicketHistory();
void handleWaitingList();
void showMenu();
void exitProgram();

int programExit = 0;

// Main Function :)

int main()
{
    loadAllData();
    while (!programExit)
    {
        showMenu();
        int choice;
        printf("\tEnter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            manageTicket();
            break;
        case 2:
            manageSchedule();
            break;
        case 3:
            viewTicketHistory();
            break;
        case 4:
            handleWaitingList();
            break;
        case 5:
            exitProgram();
            break;
        default:
            printf("\tInvalid Choice.\n");
        }
    }
    return 0;
}


// User Interface functions
void showMenu()
{
    clearTerminal();

    printf("\n\n");
    printf("\t=========================================\n");
    printf("\t||          BUS TICKET MANAGEMENT      ||\n");
    printf("\t=========================================\n");
    printf("\t||                                     ||\n");
    printf("\t||   1. Manage Ticket                  ||\n");
    printf("\t||   2. Manage Schedule                ||\n");
    printf("\t||   3. View Ticket History            ||\n");
    printf("\t||   4. Manage Waiting List            ||\n");
    printf("\t||   5. Exit (Save and Exit Program)   ||\n");
    printf("\t||                                     ||\n");
    printf("\t=========================================\n");
    printf("\t  Please select an option (1-5):\n");
}
void manageTicket()
{
    clearTerminal();

    printf("\n\n");
    printf("\t=========================================\n");
    printf("\t||         MANAGE TICKET MENU          ||\n");
    printf("\t=========================================\n");
    printf("\t||                                     ||\n");
    printf("\t||   1. Book a Ticket                  ||\n");
    printf("\t||   2. Cancel a Ticket                ||\n");
    printf("\t||   3. Go Back to Main Menu           ||\n");
    printf("\t||                                     ||\n");
    printf("\t=========================================\n");
    printf("\t  Please select an option (1-3):\n");

    int choice, busNum;
    bus *temp = NULL;

    printf("\tEnter your choice: ");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
        clearTerminal();
        printf("\n\n");
        printf("\t=========================================\n");
        printf("\tEnter Bus Number: ");
        scanf("%d", &busNum);
        temp = searchBus(root, busNum);
        if (temp != NULL)
        {
            seatMap(temp);
        }
        else
        {
            printf("\tBus Not Found\n");
        }
        printf("\n\tPress any key to continue...");
        getchar();
        getchar();
        break;
    case 2:
        clearTerminal();
        printf("\n\n");
        printf("\t=========================================\n");
        printf("\tEnter Bus Number: ");
        scanf("%d", &busNum);
        temp = searchBus(root, busNum);
        if (temp != NULL)
        {
            cancelTicket(temp);
        }
        else
        {
            printf("\tBus Not Found\n");
        }
        printf("\n\tPress any key to continue...");
        getchar();
        getchar();
        break;
    case 3:
        printf("\tReturning to Main Menu...\n");
        break;
    default:
        printf("\tInvalid Choice. Please try again.\n");
    }
}

void manageSchedule()
{
    clearTerminal();

    printf("\n\n");
    printf("\t=========================================\n");
    printf("\t||         MANAGE SCHEDULE MENU        ||\n");
    printf("\t=========================================\n");
    printf("\t||                                     ||\n");
    printf("\t||   1. Add a New Bus                  ||\n");
    printf("\t||   2. Remove an Existing Bus         ||\n");
    printf("\t||   3. Search for a Bus               ||\n");
    printf("\t||   4. Show All Buses                 ||\n");
    printf("\t||   5. Go Back to Main Menu           ||\n");
    printf("\t||                                     ||\n");
    printf("\t=========================================\n");
    printf("\t  Please select an option (1-5):\n");

    int choice, busNum, seatCount;
    char source[30], destination[30], time[10];
    bus *temp = NULL;

    printf("\tEnter your choice: ");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
        clearTerminal();
        printf("\n\n");
        printf("\t=========================================\n");
        printf("\tEnter Bus Number: ");
        scanf("%d", &busNum);
        printf("\tEnter Source: ");
        scanf("%s", source);
        printf("\tEnter Destination: ");
        scanf("%s", destination);
        printf("\tEnter Departure Time (HH:MM): ");
        scanf("%s", time);
        printf("\tEnter Number of Seats: ");
        scanf("%d", &seatCount);
        root = addBus(root, busNum, source, destination, time, seatCount);
        printf("\tBus Added Successfully!\n");

        printf("\n\tPress any key to continue...");
        getchar();
        getchar();
        break;
    case 2:
        clearTerminal();
        printf("\n\n");
        printf("\t=========================================\n");
        printf("\tEnter Bus Number: ");
        scanf("%d", &busNum);
        root = removeBus(root, busNum);
        printf("\tBus Removed Successfully!\n");

        printf("\n\tPress any key to continue...");
        getchar();
        getchar();
        break;
    case 3:
        clearTerminal();
        printf("\n\n");
        printf("\t=========================================\n");
        printf("\tEnter Bus Number: ");
        scanf("%d", &busNum);
        temp = searchBus(root, busNum);
        if (temp != NULL)
        {
            printf("\tBus found: Bus Num: %d, Source: %s, Destination: %s, Time: %s\n", temp->busNum, temp->source, temp->destination, temp->time);
        }
        else
        {
            printf("\tBus Not Found\n");
        }

        printf("\n\tPress any key to continue...");
        getchar();
        getchar();
        break;
    case 4:
        clearTerminal();
        printf("\t=========================================\n");
        printf("\t||        LIST OF AVAILABLE BUSES      ||\n");
        printf("\t=========================================\n");
        displayBus(root);
        printf("\n");
        printf("\t-----------------------------------------\n");
        printf("\n\tPress any key to continue...");
        getchar();
        getchar();
        break;
    case 5:
        printf("\tReturning to Main Menu...\n");
        break;
    default:
        printf("\tInvalid Choice. Please try again.\n");
    }
}

void viewTicketHistory()
{
    clearTerminal();

    printf("\n\n");
    printf("\t=========================================\n");
    printf("\t||           TICKET HISTORY            ||\n");
    printf("\t=========================================\n");

    if (stack == NULL)
    {
        printf("\tNo ticket history available.\n");
    }
    else
    {
        printstack(stack);
    }

    printf("\t=========================================\n");
    printf("\t||             OPTIONS                 ||\n");
    printf("\t=========================================\n");
    printf("\t||   1. Delete Last Entry              ||\n");
    printf("\t||   2. Clear Entire History           ||\n");
    printf("\t||   3. Go Back to Main Menu           ||\n");
    printf("\t=========================================\n");

    int choice;
    printf("\tEnter your choice (1-3): ");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
        if (stack == NULL)
        {
            printf("\tNo history to delete.\n");
        }
        else
        {
            popPassenger(&stack);
            printf("\tLast entry deleted successfully.\n");
        }
        printf("\n\tPress any key to continue...");
        getchar();
        getchar();
        break;
    case 2:
        if (stack == NULL)
        {
            printf("\tNo history to clear.\n");
        }
        else
        {
            clearHistory(&stack);
            printf("\tEntire ticket history cleared.\n");
        }
        printf("\n\tPress any key to continue...");
        getchar();
        getchar();
        break;
    case 3:
        printf("\tReturning to Main Menu...\n");
        break;
    default:
        printf("\tInvalid Choice. Please try again.\n");
    }
}

void handleWaitingList()
{
    clearTerminal();

    printf("\n\n");
    printf("\t=========================================\n");
    printf("\t||            WAITING LIST             ||\n");
    printf("\t=========================================\n");

    if (waitingList == NULL)
    {
        printf("\tNo passengers in the waiting list.\n");
    }
    else
    {
        printWaiting();
    }

    printf("\n\t=========================================\n");
    printf("\t||               OPTIONS               ||\n");
    printf("\t=========================================\n");
    printf("\t||   1. Process Next Waiting Passenger ||\n");
    printf("\t||   2. Go Back to Main Menu           ||\n");
    printf("\t=========================================\n");

    int choice, busNum;
    bus *temp = NULL;

    printf("\tEnter your choice: ");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
        printf("\tEnter Bus Number: ");
        scanf("%d", &busNum);
        temp = searchBus(root, busNum);
        if (temp != NULL)
        {
            seatMap(temp);
            dequeueWaiting();
        }
        else
        {
            printf("\tBus Not Found\n");
        }

        printf("\n\tPress any key to continue...");
        getchar();
        getchar();
        break;
    case 2:
        printf("\tReturning to Main Menu...\n");
        break;
    default:
        printf("\tInvalid Choice. Please try again.\n");
    }
}

void exitProgram()
{
    printf("\tExiting the program...\n");
    saveAllData();
    freeBuses(root);
    freeWaitingList();
    freePassengerStack();
    programExit = 1;
}

// Waiting List Functions
waiting *createWaiting(int contact, int busNum, int seat, char name[50])
{
    waiting *newWaiting = (waiting *)malloc(sizeof(waiting));
    newWaiting->contact = contact;
    newWaiting->busNum = busNum;
    newWaiting->seat = seat;
    strcpy(newWaiting->name, name);
    newWaiting->next = NULL;
    return newWaiting;
}

void enqueueWaiting(int contact, int busNum, int seat, char name[50])
{
    waiting *newWaiting = createWaiting(contact, busNum, seat, name);
    if (waitingList == NULL)
    {
        waitingList = newWaiting;
        return;
    }
    waiting *temp = waitingList;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = newWaiting;
}

void dequeueWaiting()
{
    if (waitingList == NULL)
    {
        printf("\tWaiting List is empty\n");
        return;
    }

    waiting *temp = waitingList;
    waitingList = temp->next;
    free(temp);
}

void printWaiting()
{
    waiting *temp = waitingList;
    while (temp != NULL)
    {
        printf("\t Name: %s , Contact : %d , Bus Number : %d ,Seat Number : %d .\n", temp->name, temp->contact, temp->busNum, temp->seat);
        temp = temp->next;
    }
    printf("\n\n");
}

// Passenger Functions

passenger *newPassenger(int contact, int busNum, int seat, int book, char name[50])
{
    passenger *newPass = (passenger *)malloc(sizeof(passenger));
    newPass->contact = contact;
    newPass->busNum = busNum;
    newPass->seat = seat;
    newPass->book = book;
    strcpy(newPass->name, name);
    newPass->next = NULL;
    return newPass;
}

void pushPassenger(passenger **stack, int contact, int busNum, int seat, int book, char name[50])
{
    passenger *newPass = newPassenger(contact, busNum, seat, book, name);
    newPass->next = *stack;
    *stack = newPass;

    printf("\n\tPassenger Added to History\n");
}

void printstack(passenger *stack)
{
    passenger *temp = stack;
    while (temp != NULL)
    {
        if (temp->book != 0)
        {
            printf("\t Name: %s , Contact : %d , Bus Number : %d ,Seat Number : %d , State : BOOKED\n", temp->name, temp->contact, temp->busNum, temp->seat);
        }
        else
        {
            printf("\t Name: %s , Contact : %d , Bus Number : %d ,Seat Number : %d , State : CANCELED \n", temp->name, temp->contact, temp->busNum, temp->seat);
        }
        temp = temp->next;
    }
    printf("\n\n");
}

void clearHistory(passenger **stack)
{
    passenger *temp;
    while (*stack != NULL)
    {
        temp = *stack;
        *stack = (*stack)->next;
        free(temp);
    }
    printf("\tTicket History Cleared Successfully.\n");
}

void popPassenger(passenger **stack)
{
    if (*stack == NULL)
    {
        printf("\tTicket history is empty. No entry to delete.\n");
        return;
    }

    passenger *temp = *stack;
    *stack = (*stack)->next;
    printf("\tDeleted Entry: Name: %s, Contact: %d, Bus Number: %d, Seat Number: %d, State: %s\n",
           temp->name, temp->contact, temp->busNum, temp->seat, (temp->book == 1 ? "BOOKED" : "CANCELED"));
    free(temp);
}


// Seats Functions
seat *creatSeat(int num)
{
    seat *newSeat = (seat *)malloc(sizeof(seat));
    newSeat->num = num;
    newSeat->book = 0;
    newSeat->next = NULL;
    return newSeat;
}

seat *insertSeat(seat *head, int num)
{
    seat *newSeat = creatSeat(num);
    newSeat->next = head;
    head = newSeat;
    return head;
}

seat *searchSeat(seat *head, int num)
{
    seat *current = head;
    while (current != NULL)
    {
        if (current->num == num)
            return current;
        current = current->next;
    }
    return NULL;
}


// Bus Functions

bus *createBus(int busNum, char source[30], char destination[30], char time[10], int seatCount)
{
    bus *temp = (bus *)malloc(sizeof(bus));
    temp->busNum = busNum;
    strcpy(temp->source, source);
    strcpy(temp->destination, destination);
    strcpy(temp->time, time);
    temp->left = temp->right = NULL;
    temp->seatCount = seatCount;

    for (int i = seatCount; i >= 1; i--)
    {
        temp->seats = insertSeat(temp->seats, i);
    }

    return temp;
}

bus *addBus(bus *root, int busNum, char source[30], char destination[30], char time[10], int seatCount)
{
    if (root == NULL)
        return createBus(busNum, source, destination, time, seatCount);
    if (busNum < root->busNum)
        root->left = addBus(root->left, busNum, source, destination, time, seatCount);
    else if (busNum > root->busNum)
        root->right = addBus(root->right, busNum, source, destination, time, seatCount);
    return root;
}

bus *findMin(bus *root)
{
    if (root == NULL)
        return NULL;
    else if (root->left != NULL)
        return findMin(root->left);
    return root;
}

void freeSeats(seat *head)
{
    seat *current = head;
    seat *nextSeat;
    while (current != NULL)
    {
        nextSeat = current->next;
        free(current);
        current = nextSeat;
    }
}

bus *removeBus(bus *root, int busNum)
{
    if (root == NULL)
        return NULL;
    if (busNum > root->busNum)
        root->right = removeBus(root->right, busNum);
    else if (busNum < root->busNum)
        root->left = removeBus(root->left, busNum);
    else
    {
        if (root->left == NULL && root->right == NULL)
        {
            freeSeats(root->seats);
            free(root);
            return NULL;
        }
        else if (root->left == NULL || root->right == NULL)
        {
            bus *temp;
            if (root->left == NULL)
                temp = root->right;
            else
                temp = root->left;
            freeSeats(root->seats);
            free(root);
            return temp;
        }
        else
        {
            bus *temp = findMin(root->right);
            root->busNum = temp->busNum;
            strcpy(root->source, temp->source);
            strcpy(root->destination, temp->destination);
            strcpy(root->time, temp->time);
            root->seatCount = temp->seatCount;
            freeSeats(root->seats);
            root->seats = temp->seats;
            root->right = removeBus(root->right, temp->busNum);
        }
    }
    return root;
}

bus *searchBus(bus *root, int busNum)
{
    if (root == NULL || root->busNum == busNum)
        return root;
    if (root->busNum < busNum)
        return searchBus(root->right, busNum);
    return searchBus(root->left, busNum);
}

void displayBus(bus *root)
{
    if (root != NULL)
    {
        printf("\t%d > %d - %s - %s - %s\n", serial++, root->busNum, root->source, root->destination, root->time);
        displayBus(root->left);
        displayBus(root->right);
    }
}


// Ticket Booking & Cancel Function

void seatMap(bus *Bus)
{
    int bookNum;
    int num = Bus->seatCount;
    clearTerminal();
    printf("\n");
    printf("\t");
    for (int i = 1; i <= num; i++)
    {
        seat *check = searchSeat(Bus->seats, i);

        printf("[%2d]-%d  ", i, check->book);
        if (i % 4 == 0)
            printf("\n\t");
    }
    printf("\n\t0 == Not Booked;1== Booked\n");
    printf("\t=========================================\n");
    printf("\n\tEnter Seat number : ");
    scanf("%d", &bookNum);
    seat *check = searchSeat(Bus->seats, bookNum);

    if (check->book == 0)
    {
        check->book = 1;

        int contact;
        char name[50];

        printf("\tEnter Passenger Name : ");
        scanf(" %[^\n]", name);
        printf("\tEnter Contact Number (Start With 880) : ");
        scanf("%d", &contact);

        pushPassenger(&stack, contact, Bus->busNum, bookNum, 1, name); // 1 means booked
        printf("\tTicket Booked Succesfully\n");
    }
    else
    {
        printf("\n\tSeat Already Booked. Do you want to join the waiting list? (1 for Yes, 0 for No): ");
        int choice;
        scanf("%d", &choice);

        if (choice == 1)
        {
            int contact;
            char name[50];

            printf("\tEnter Passenger Name: ");
            scanf(" %[^\n]", name);
            printf("\tEnter Contact Number (Start With 880) : ");
            scanf("%d", &contact);

            enqueueWaiting(contact, Bus->busNum, bookNum, name);
        }
        else
        {
            printf("\tPassenger declined to join the waiting list.\n");
        }
    }
}
// canceling

void cancelTicket(bus *Bus)
{
    int cancelNum;
    printf("\tEnter Seat Number : ");
    scanf("%d", &cancelNum);

    seat *check = searchSeat(Bus->seats, cancelNum);

    if (check->book == 1)
    {
        check->book = 0;

        int contact;
        char name[50];

        printf("\tEnter Passenger Name : ");
        scanf(" %[^\n]", name);
        printf("\tEnter Contact Number (Start With 880) : ");
        scanf("%d", &contact);

        pushPassenger(&stack, contact, Bus->busNum, cancelNum, 0, name);
        printf("\tTicket Canceled Succesfully.\n");
    }
    else
        printf("Ticket Not Booked Yet.\n");
}

// Save & load - Memory
void saveBuses(bus *root, FILE *file)
{
    if (root == NULL)
        return;
    fprintf(file, "%d %s %s %s %d\n", root->busNum, root->source, root->destination, root->time, root->seatCount);
    saveBuses(root->left, file);
    saveBuses(root->right, file);
}

void saveBusData()
{
    FILE *file = fopen("buses.txt", "w");
    if (file == NULL)
    {
        printf("Error saving bus data.\n");
        return;
    }
    saveBuses(root, file);
    fclose(file);
}

void loadBusData()
{
    FILE *file = fopen("buses.txt", "r");
    if (file == NULL)
    {
        printf("No saved bus data found.\n");
        return;
    }
    int busNum, seatCount;
    char source[30], destination[30], time[10];
    while (fscanf(file, "%d %s %s %s %d", &busNum, source, destination, time, &seatCount) != EOF)
    {
        root = addBus(root, busNum, source, destination, time, seatCount);
    }
    fclose(file);
}

void saveWaitingList()
{
    FILE *file = fopen("waiting_list.txt", "w");
    if (file == NULL)
    {
        printf("Error saving waiting list.\n");
        return;
    }
    waiting *temp = waitingList;
    while (temp != NULL)
    {
        fprintf(file, "%d %d %d %s\n", temp->contact, temp->busNum, temp->seat, temp->name);
        temp = temp->next;
    }
    fclose(file);
}

void loadWaitingList()
{
    FILE *file = fopen("waiting_list.txt", "r");
    if (file == NULL)
    {
        printf("No saved waiting list data found.\n");
        return;
    }
    int contact, busNum, seat;
    char name[50];
    while (fscanf(file, "%d %d %d %[^\n]", &contact, &busNum, &seat, name) != EOF)
    {
        enqueueWaiting(contact, busNum, seat, name);
    }
    fclose(file);
}

void saveTicketHistory()
{
    FILE *file = fopen("ticket_history.txt", "w");
    if (file == NULL)
    {
        printf("Error saving ticket history.\n");
        return;
    }
    passenger *temp = stack;
    while (temp != NULL)
    {
        fprintf(file, "%d %d %d %d %s\n", temp->contact, temp->busNum, temp->seat, temp->book, temp->name);
        temp = temp->next;
    }
    fclose(file);
}

void loadTicketHistory()
{
    FILE *file = fopen("ticket_history.txt", "r");
    if (file == NULL)
    {
        printf("No saved ticket history found.\n");
        return;
    }
    int contact, busNum, seat, book;
    char name[50];
    while (fscanf(file, "%d %d %d %d %[^\n]", &contact, &busNum, &seat, &book, name) != EOF)
    {
        pushPassenger(&stack, contact, busNum, seat, book, name);
    }
    fclose(file);
}

void saveBusSeats(bus *Bus)
{
    if (Bus == NULL)
        return;

    char filename[20];
    sprintf(filename, "bus_%d_seats.txt", Bus->busNum);
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Error saving seats for Bus %d.\n", Bus->busNum);
        return;
    }

    seat *current = Bus->seats;
    while (current != NULL)
    {
        fprintf(file, "%d %d\n", current->num, current->book);
        current = current->next;
    }

    fclose(file);
}

void saveAllBusSeats(bus *root)
{
    if (root == NULL)
        return;

    saveBusSeats(root);
    saveAllBusSeats(root->left);
    saveAllBusSeats(root->right);
}

void loadBusSeats(bus *Bus)
{
    if (Bus == NULL)
        return;

    char filename[20];
    sprintf(filename, "bus_%d_seats.txt", Bus->busNum);
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("No saved seat data for Bus %d.\n", Bus->busNum);
        return;
    }

    int seatNum, bookStatus;
    while (fscanf(file, "%d %d", &seatNum, &bookStatus) != EOF)
    {
        seat *seat = searchSeat(Bus->seats, seatNum);
        if (seat != NULL)
        {
            seat->book = bookStatus;
        }
    }

    fclose(file);
}

void loadAllBusSeats(bus *root)
{
    if (root == NULL)
        return;

    loadBusSeats(root);
    loadAllBusSeats(root->left);
    loadAllBusSeats(root->right);
}

void saveAllData()
{
    saveBusData();
    saveAllBusSeats(root);
    saveWaitingList();
    saveTicketHistory();
}

void loadAllData()
{
    loadBusData();
    loadAllBusSeats(root);
    loadWaitingList();
    loadTicketHistory();
}

void freeBuses(bus *root)
{
    if (root == NULL)
        return;
    freeBuses(root->left);
    freeBuses(root->right);
    freeSeats(root->seats);
    free(root);
}

void freeWaitingList()
{
    waiting *current = waitingList;
    waiting *nextNode;
    while (current != NULL)
    {
        nextNode = current->next;
        free(current);
        current = nextNode;
    }
    waitingList = NULL;
}

void freePassengerStack()
{
    passenger *current = stack;
    passenger *nextPass;
    while (current != NULL)
    {
        nextPass = current->next;
        free(current);
        current = nextPass;
    }
    stack = NULL;
}