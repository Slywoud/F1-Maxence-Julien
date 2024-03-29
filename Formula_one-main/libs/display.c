#include "../headers/f1.h"

/**
 * Display the scores of the cars in the circuit in real time.
 *
 * @param circuit A pointer to the array of cars.
 * @param car_count The number of cars in the array.
 */
void display_scores(car *circuit, int car_count){
    printw("| id |   s1  |   s2  |   s3  | best lap | best s1 | best s2 | best s3 | pitstop | crash \n"
           "|----|-------|-------|-------|----------|---------|---------|---------|---------|-------\n");

    for (int i = 0; i < car_count; i++) {
        printw("| %2d | ", circuit[i].id);
        if (circuit[i].has_best_s1)attron(COLOR_PAIR(1));
        printw("%-5.4g", circuit[i].s1);
        if (circuit[i].has_best_s1)attroff(COLOR_PAIR(1));
        printw(" | ");
        if (circuit[i].has_best_s2)attron(COLOR_PAIR(1));
        printw("%-5.4g", circuit[i].s2);
        if (circuit[i].has_best_s2)attroff(COLOR_PAIR(1));
        printw(" | ");
        if (circuit[i].has_best_s3)attron(COLOR_PAIR(1));
        printw("%-5.4g", circuit[i].s3);
        if (circuit[i].has_best_s3)attroff(COLOR_PAIR(1));
        printw(" | %-8.4g", circuit[i].best_lap);
        printw(" | %-7.4g", circuit[i].best_s1);
        printw(" | %-7.4g", circuit[i].best_s2);
        printw(" | %-7.4g", circuit[i].best_s3);
        printw(" | %-7.1d", circuit[i].pitstop);
        printw(" | %s\n", circuit[i].state_crash ? "true" : "false");
    }
}
