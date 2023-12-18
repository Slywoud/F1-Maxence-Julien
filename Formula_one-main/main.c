#include "libs/f1.h"

//TODO: different behaviour based on type of race (Q1-3, P1-3, s)
//TODO: saving results to csv
//TODO: starting cars based on results
//TODO: concurrence
//TODO: pit stop/crash
//TODO: add error handling/generating

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 3){
        fprintf(stderr, "Wrong number of arguments\n");
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
    int shmid, cpid, num_cars, shmkey = 420;
    num_cars = atoi(argv[1]);

    //display
    initscr();
    start_color();
    init_pair(1,COLOR_MAGENTA, COLOR_BLACK);

    //shared memory
    shmid = shmget(shmkey, num_cars * sizeof(car), IPC_CREAT | 0666);
    car *circuit = shmat(shmid, 0, 0);

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
            for(int j = 0; j < 20; j++){
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
        for(int i = 0; i < 20; i++) {
            car * buffer = malloc(num_cars * sizeof(car));
            memcpy(buffer,circuit,num_cars * sizeof(car));
            bubble_sort(buffer, num_cars);
            printw("%d\n", i);
            display_scores(buffer, num_cars);

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
