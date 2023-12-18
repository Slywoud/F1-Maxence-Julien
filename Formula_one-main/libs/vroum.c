#include "f1.h"

float genTime(float lower, float upper) {
    return (lower + (drand48() * (upper - lower)));
}


bool probability(double p) {
    // Seed the random number generator with the current time
    srand(time(0));

    // Generate a random number between 0 and 1
    double r = (double) rand() / RAND_MAX;

    // Return true if the random number is less than p
    return r < p;
}

void init_car(car *ptr, int carId) {
    ptr->id = carId;
    ptr->best_s1 = 45;//initier a 45 parce que sinon on a une valeur random et 45 est le pire temps possible
    ptr->best_s2 = 45;
    ptr->best_s3 = 45;
    ptr->best_lap = 45 * 3;
    ptr->total_time = 0;
    ptr->has_best_s1 = 0;
    ptr->has_best_s2 = 0;
    ptr->has_best_s3 = 0;
    ptr->state_pitstop = false;
    ptr->state_crash = false;
}
void lap_car(car *ptr){
    //TODO: pit stop / crash all cars pit at same time, no crashing implemented
    float lap;
 
    ptr->s1 = genTime(25,45);
    ptr->s2 = genTime(25,45);
    ptr->s3 = genTime(25,45);
    lap = ptr->s1 + ptr->s2 + ptr->s3;
    

    if(ptr->s1 < ptr->best_s1)ptr->best_s1 = ptr->s1;
    if(ptr->s2 < ptr->best_s2)ptr->best_s2 = ptr->s2;
    if(ptr->s3 < ptr->best_s3)ptr->best_s3 = ptr->s3;
    if(lap < ptr->best_lap)ptr->best_lap = lap;
    ptr->total_time = ptr->total_time + lap;
}
