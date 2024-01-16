#include "f1.h"

void init_random() {
    srand48(time(0));
}

float genTime(float lower, float upper) {
    return (lower + (drand48() * (upper - lower)));
}

bool probability(double p) {

    // Generate a random number between 0 and 1
    double r = drand48();

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
    ptr->pitstop = 0;
    ptr->state_crash = false;
}

void lap_car(car *ptr) {
    
    if (ptr->state_crash) {
        // The car has crashed, so it can't continue racing.
        return;
    }

    float lap;

    ptr->s1 = genTime(25, 45);
    ptr->s2 = genTime(25, 45);
    ptr->s3 = genTime(25, 45);
    lap = ptr->s1 + ptr->s2 + ptr->s3;

    if (probability(0.1)) {
        ptr->pitstop += 1;
    }

    if (!ptr->state_crash && probability(0.02)) {
        ptr->state_crash = true;
    }

    if (ptr->s1 < ptr->best_s1)
        ptr->best_s1 = ptr->s1;
    if (ptr->s2 < ptr->best_s2)
        ptr->best_s2 = ptr->s2;
    if (ptr->s3 < ptr->best_s3)
        ptr->best_s3 = ptr->s3;
    if (lap < ptr->best_lap)
        ptr->best_lap = lap;
    ptr->total_time = ptr->total_time + lap;
}

int* read_qualified_car_ids_from_csv(char* filename, int num_qualified_cars) {
    char filepath[256];
    sprintf(filepath, "results/%s", filename);
    FILE* file = fopen(filepath, "a+");
    if (!file) {
        fprintf(stderr, "Error: Could not open or create file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    if (ftell(file) == 0) {
        // The file is empty, write default or random car IDs to the file
        for (int i = 0; i < num_qualified_cars; i++) {
            fprintf(file, "| %d |\n", i+1);  // Write default car IDs
        }
        rewind(file);  // Move the file pointer back to the start of the file
    }

    int* qualified_car_ids = malloc(num_qualified_cars * sizeof(int));
    if (!qualified_car_ids) {
        fprintf(stderr, "Error: Could not allocate memory for qualified_car_ids\n");
        exit(EXIT_FAILURE);
    }

    // Skip the first three lines
    char dummy[256];
    fgets(dummy, sizeof(dummy), file);
    fgets(dummy, sizeof(dummy), file);
    fgets(dummy, sizeof(dummy), file);
    fgets(dummy, sizeof(dummy), file);

    int i = 0;
    while (i < num_qualified_cars && !feof(file)) {
        // Read the car id directly from the file
        fscanf(file, "| %d |", &qualified_car_ids[i]);
        // Skip the rest of the line
        fgets(dummy, sizeof(dummy), file);
        i++;
    }

    fclose(file);
    return qualified_car_ids;
}

void init_circuit_with_qualified_car_ids(car* circuit, int* qualified_car_ids, int num_qualified_cars) {
    for (int i = 0; i < num_qualified_cars; i++) {
        init_car(&circuit[i], qualified_car_ids[i]);
    }
}

void run_cars_and_write_results_to_csv(car* circuit, int num_cars, int num_laps, char* filename) {
    for (int i = 0; i < num_cars; i++) {
        for (int j = 0; j < num_laps; j++) {
            lap_car(&circuit[i]);
        }
    }

    bubble_sort(circuit, num_cars);
    write_to_file(filename, filename, "w", ";", num_cars, circuit);
}
