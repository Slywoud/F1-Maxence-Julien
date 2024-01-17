#include "libs/f1.h"

//TODO: different behaviour based on type of race (Q1-3, P1-3, s)
//TODO: starting cars based on results
//TODO: award points for world championship

/**
 * This function checks the command-line arguments passed to the program.
 * It expects exactly one argument, which must be one of the following options:
 * "-h", "-P1", "-P2", "-P3", "-Q1", "-Q2", "-Q3", "-sprint", "-race".
 * 
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @return 0 if the arguments are valid, 1 otherwise.
 */
int checkArguments(int argc, char *argv[]) {
    if (argc != 2 || (strcmp(argv[1], "-h") != 0 && strcmp(argv[1], "-P1") != 0 && strcmp(argv[1], "-P2") != 0 && strcmp(argv[1], "-P3") != 0
            && strcmp(argv[1], "-Q1") != 0 && strcmp(argv[1], "-Q2") != 0 && strcmp(argv[1], "-Q3") != 0
            && strcmp(argv[1], "-sprint") != 0 && strcmp(argv[1], "-race") != 0)) {
        fprintf(stderr, "Error: %s [-P1 | -P2 | -P3 | -Q1 | -Q2 | -Q3 | -sprint | -race]\n", argv[0]);
        return 1;
    }
    return 0;
}

/**
 * This function displays the help message for the program.
 */
void displayHelpMessage() {
    printf("Usage: ./f1 [OPTION]\n");
    printf("  -h     display this help message\n");
    printf("  -P1     start the Practise 1 session (1H)\n");
    printf("  -P2     start the Practise 2 session (1H)\n");
    printf("  -P3     start the Practise 3 session (1H)\n");
    printf("  -Q1     start the Qualifiying 1 session (12min)\n");
    printf("  -Q2     start the Qualifiying 2 session (10min)\n");
    printf("  -Q3     start the Qualifiying 3 session (8min)\n");
    printf("  -sprint     start the sprint race (100-120 km)\n");
    printf("  -race     start the sunday race (300-3500 km)\n");
}

int main(int argc, char *argv[]) {

    // Check the command-line arguments using the checkArguments function.
    if (checkArguments(argc, argv) != 0){
        return 1;
    }

    // If the first argument is "-h", display a help message
    if (argc > 1 && strcmp(argv[1], "-h") == 0) {
        displayHelpMessage();
        return 0;
    }

    // Declare integer variables to hold the shared memory ID (shmid) and child process ID (cpid)
    int shmid, cpid;

    // Initialize the number that will participate to the session
    int num_cars = init_num_cars(argv[1]);
    
    // Declare an integer to hold the number of laps and a char pointer for the name of the file were the results of sessions will be saved
    int num_laps;
    char* filename;
    // This function will set the values of num_laps and filename based on the command-line argument.
    determine_race_parameters(argv[1], &num_laps, &filename);

    // For a better display : initialize the ncurses library, start color functionality and Initialize a color pair with the number 1
    initscr();
    start_color();
    init_pair(1, COLOR_MAGENTA, COLOR_BLACK);

    // Create a shared memory. Size = the number of cars * the size of 1 car. Read and write permissions for everyone.
    shmid = shmget(IPC_PRIVATE, num_cars * sizeof(car), IPC_CREAT | 0666);
    if (shmid == -1)
        exit(EXIT_FAILURE);

    // Attach the shared memory segment to our address space(car *circuit) using shmat.
    car *circuit = shmat(shmid, 0, 0);
    if (circuit == (void *)-1)
        exit(EXIT_FAILURE);

    // Declare an array of car IDs. These are the IDs of the drivers.
    static int carIds[] = {1, 11, 16, 55, 63, 44, 31, 10, 4, 81, 77, 24,
                                14, 18, 20, 27, 22, 3, 23, 2};

    // Initialize each car
    for (int i = 0; i < num_cars; i++) {
        init_car(&circuit[i], carIds[i]);
    }
    
    // Initialize the random number generator for crash
    init_random();

    // Loop over the number of cars and create a new process for each car using fork
    for (int i = 0; i < num_cars; i++) {
        // Child
        if ((cpid = fork()) == 0) {
            car *child = &circuit[i];

            //generate seed for rand based on
            srand48(time(0) + child->id);

            //printf("[son] pid %d from [parent] pid %d and car id is %d\n", getpid(), getppid(), child.id);
            for (int j = 0; j < num_laps; j++) {
                sleep(1);
                //circuit[i].s1 = j;
                lap_car(&circuit[i]);
            }
            //printf("car %d has a lap time of %g\n", child.id, child.total_time);
            exit(0);
        }
    }
    // Parent
    if(cpid != 0){
        halfdelay(5);
        for(int i = 0; i < num_laps; i++) {
            car * buffer = malloc(num_cars * sizeof(car));
            // Malloc check
            if (buffer == NULL) {
                printw("Erreur d'allocation mémoire.\n");
                // Methods used when there is an error of allocation
                getch();
                endwin();
                exit(EXIT_FAILURE);
            }
            // Copy the cars from the circuit to the buffer
            memcpy(buffer,circuit,num_cars * sizeof(car));
            bubble_sort(buffer, num_cars);
            printw("%d\n", i);
            display_scores(buffer, num_cars);
            // Memory freed, no more leaks
            free(buffer);

            getch();
            erase();
            refresh();
        }

        // Detach the shared memory segment (circuit) from our address space
        shmctl(shmid, IPC_RMID, NULL);

        //display
        getch();
        endwin();
    }

    //Only for Q2 and Q3 session.
    // Determine the number of qualified cars based on the session
    // Read the IDs of the qualified cars from the CSV file of the previous session
    // Initialize the circuit with the qualified cars
    if (strcmp(argv[1], "-Q2") == 0 || strcmp(argv[1], "-Q3") == 0) {
        int num_qualified_cars = (strcmp(argv[1], "-Q2") == 0) ? 15 : 10;
        int* qualified_car_ids = read_qualified_car_ids_from_csv(filename, num_qualified_cars);
        init_circuit_with_qualified_car_ids(circuit, qualified_car_ids, num_qualified_cars);
        free(qualified_car_ids);
    }

    run_cars_and_write_results_to_csv(circuit, num_cars, num_laps, filename);

    // Detach the shared memory segment (circuit) from our address space
    shmdt(circuit);

    exit(0);
}
