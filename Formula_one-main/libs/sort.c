#include "f1.h"

/**
 * Sort an array of cars in ascending order of their best lap time using the bubble sort algorithm.
 *
 * @param list A pointer to the array of cars to sort.
 * @param car_count The number of cars in the array.
 */
void bubble_sort(car *list, int car_count){
    //TODO: optimise
    car buffer;
    int count, count2;

    for(count = 0; count < car_count; count++){
        for(count2 = 0; count2 < car_count; count2++){
            if(list[count2].best_lap > list[count].best_lap){
                buffer = list[count];
                list[count] = list[count2];
                list[count2] = buffer;
            }
        }
    }
}

/**
 * Determine the cars with the best times for each sector.
 *
 * @param list A pointer to the array of cars.
 * @param car_count The number of cars in the array.
 */
void best_sector(car *list, int car_count){
    float s1 = 45, s2 = 45, s3 = 45;
    int s1_best, s2_best, s3_best;
    for(int i = 0; i < car_count; i++){
        if(list[i].best_s1 < s1){
            s1 = list[i].best_s1;
            s1_best = i;
        }
        if(list[i].best_s2 < s2){
            s2 = list[i].best_s2;
            s2_best = i;
        }
        if(list[i].best_s3 < s3){
            s3 = list[i].best_s3;
            s3_best = i;
        }
    }
    list[s1_best].has_best_s1 = 1;
    list[s2_best].has_best_s2 = 1;
    list[s3_best].has_best_s3 = 1;
}