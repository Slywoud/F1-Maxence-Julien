#include "libs/f1.h"

//TODO: different behaviour based on type of race (Q1-3, P1-3, s)
//TODO: saving results to csv
//TODO: starting cars based on results
//TODO: concurrence
//TODO: pit stop/crash
//TODO: add error handling/generating

int checkArguments(int argc, char *argv[]) {
    if (argc != 2 || (strcmp(argv[1], "-h") != 0 && strcmp(argv[1], "-P1") != 0 && strcmp(argv[1], "-P2") != 0 && strcmp(argv[1], "-P3") != 0
            && strcmp(argv[1], "-Q1") != 0 && strcmp(argv[1], "-Q2") != 0 && strcmp(argv[1], "-Q3") != 0
            && strcmp(argv[1], "-sprint") != 0 && strcmp(argv[1], "-race") != 0)) {
        fprintf(stderr, "Error: %s [-P1 | -P2 | -P3 | -Q1 | -Q2 | -Q3 | -sprint | -race]\n", argv[0]);
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    if (checkArguments(argc, argv) != 0){
        return 1;
    }
    if (argc > 1 && strcmp(argv[1], "-h") == 0) {
        // Print the help message
        printf("Usage: Formula_one [OPTION]\n");
        printf("  -h     display this help message\n");
        printf("  -P1     start the Practise 1 session (1H)\n");
        printf("  -P2     start the Practise 2 session (1H)\n");
        printf("  -P3     start the Practise 3 session (1H)\n");
        printf("  -Q1     start the Qualifiying 1 session (12min)\n");
        printf("  -Q2     start the Qualifiying 2 session (10min)\n");
        printf("  -Q3     start the Qualifiying 3 session (8min)\n");
        printf("  -sprint     start the sprint race (100-120 km)\n");
        printf("  -race     start the sunday race (300-3500 km)\n");
        return 0;
    }

    // Faire une vérification d'argument
    int shmid, cpid;

    // Initialize number of cars competing for a specific session
    int num_cars;
    if (strcmp(argv[1], "-Q2") == 0) {
        num_cars = 15;
    } else if (strcmp(argv[1], "-Q3") == 0) {
        num_cars = 10;
    } else {
        num_cars = 20;
    }
    
    int num_laps;
    char* filename;
    if (strcmp(argv[1], "-P1") == 0) {
        num_laps = 35;
        filename = "P1.csv";
    } else if (strcmp(argv[1], "-P2") == 0) {
        num_laps = 35;
        filename = "P2.csv";
    } else if (strcmp(argv[1], "-P3") == 0) {
        num_laps = 35;
        filename = "P3.csv";
    } else if (strcmp(argv[1], "-Q1") == 0) {
        num_laps = 7;
        filename = "Q1.csv";
    } else if (strcmp(argv[1], "-Q2") == 0) {
        if (access("results/Q1.csv", F_OK) != -1) {
            num_laps = 6;
            filename = "Q2.csv";
        } else {
            fprintf(stderr, "Error: Q1 must be completed before Q2\n");
            exit(EXIT_FAILURE);
        }
    } else if (strcmp(argv[1], "-Q3") == 0) {
        if (access("results/Q2.csv", F_OK) != -1) {
            num_laps = 5;
            filename = "Q3.csv";
        } else {
            fprintf(stderr, "Error: Q2 must be completed before Q3\n");
            exit(EXIT_FAILURE);
        }
    } else if (strcmp(argv[1], "-sprint") == 0) {
        num_laps = 17;
        filename = "Sprint.csv";
    } else if (strcmp(argv[1], "-race") == 0) {
        num_laps = 52;
        filename = "Race.csv";
    }

    //display
    initscr();
    start_color();
    init_pair(1,COLOR_MAGENTA, COLOR_BLACK);

    //shared memory
    shmid = shmget(IPC_PRIVATE, num_cars * sizeof(car), IPC_CREAT | 0666);
    if (shmid == -1)
        exit(EXIT_FAILURE);
    car *circuit = shmat(shmid, 0, 0);
    // schmat returns (void *)-1 on error
    if (circuit == (void *)-1)
        exit(EXIT_FAILURE);
    //printf("%d\n",num_cars);

    static int carIds[] = {1, 11, 16, 55, 63, 44, 31, 10, 4, 81, 77, 24,
                            14, 18, 20, 27, 22, 3, 23, 2};
    for (int i = 0; i < num_cars; i++) {
        init_car(&circuit[i], carIds[i]);
    }
    
    // Initialize the random number generator for crash
    init_random();

    for (int i = 0; i < num_cars; i++) {
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

        //shared memory
        shmctl(shmid, IPC_RMID, NULL);

        //display
        getch();
        endwin();
    }

    /* char* filename_copy = strdup(filename);
    char* session_name = strtok(filename_copy, ".");
    bubble_sort(circuit, num_cars);
    write_to_file(session_name, filename, "w", ";", num_cars, circuit);
    free(filename_copy); */

    // Lire le fichier CSV de la session précédente et initialiser le tableau circuit avec les ID des voitures qualifiées
    if (strcmp(argv[1], "-Q2") == 0 || strcmp(argv[1], "-Q3") == 0) {
        int num_qualified_cars = (strcmp(argv[1], "-Q2") == 0) ? 15 : 10;
        int* qualified_car_ids = read_qualified_car_ids_from_csv(filename, num_qualified_cars);
        init_circuit_with_qualified_car_ids(circuit, qualified_car_ids, num_qualified_cars);
        free(qualified_car_ids);
    }

    // Faire courir les voitures et écrire les résultats dans le fichier CSV de la session actuelle
    run_cars_and_write_results_to_csv(circuit, num_cars, num_laps, filename);


    //shared memory
    shmdt(circuit);

    exit(0);
}
