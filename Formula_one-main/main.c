#include "libs/f1.h"

//TODO: different behaviour based on type of race (Q1-3, P1-3, s)
//TODO: saving results to csv
//TODO: starting cars based on results
//TODO: concurrence
//TODO: pit stop/crash
//TODO: add error handling/generating

int checkArguments(int argc, char *argv[]) {
    if (argc != 3 || (strcmp(argv[1], "-sprint") != 0 && strcmp(argv[1], "-sunday") != 0)) {
        fprintf(stderr, "Error: %s [-sprint | -sunday] <num_cars>\n", argv[0]);
        return 1;
    }

    int num_cars = atoi(argv[2]);

    if (num_cars < 1 || num_cars > 20) {
        fprintf(stderr, "Error: <num_cars> must be a number between 1 and 20\n");
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
    printf("  -sprint     start the sprint race (100-120 km)\n");
    printf("  -sunday     start the sunday race (300-3500 km)\n");
    return 0;
    }
    // Faire une vérification d'argument
    int shmid, cpid, num_cars= 420;
    num_cars = atoi(argv[2]);
    int num_iterations = 20;
    if (strcmp(argv[1], "-sunday") == 0) {
        num_iterations = 40;
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

    static int carIds[] = {44, 63, 1, 11, 55, 16, 4, 3, 14, 31, 10, 22,
                           5, 18, 6, 23, 77, 24, 47, 9};
    for (int i = 0; i < num_cars; i++) {
        init_car(&circuit[i], carIds[i]);
    }

    for (int i = 0; i < num_cars; i++) {
        if ((cpid = fork()) == 0) {
            car child;
            child = circuit[i];

            //generate seed for rand based on
            srand48(time(0) + child.id);

            //printf("[son] pid %d from [parent] pid %d and car id is %d\n", getpid(), getppid(), child.id);
            for (int j = 0; j < num_iterations; j++) {
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
        for(int i = 0; i < num_iterations; i++) {
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

    write_to_file("p1","test.csv", "w", ";", num_cars, circuit);

    //shared memory
    shmdt(circuit);

    exit(0);
}
