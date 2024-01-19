#include "../headers/f1.h"

/**
 * Initialize the random number generator with the current time as the seed.
 */
void init_random() {
    srand48(time(0));
}

/**
 * Generate a random float between a lower and upper bound.
 *
 * @param lower The lower bound.
 * @param upper The upper bound.
 * @return A random float between the lower and upper bound.
 */
float genTime(float lower, float upper) {
    return (lower + (drand48() * (upper - lower)));
}

/**
 * Return true with a probability of p.
 *
 * @param p The probability.
 * @return True with a probability of p, false otherwise.
 */
bool probability(double p) {

    // Generate a random number between 0 and 1
    double r = drand48();

    // Return true if the random number is less than p
    return r < p;
}

/**
 * Initialize a car with a given ID.
 *
 * @param ptr A pointer to the car to initialize.
 * @param carId The ID to assign to the car.
 */
void init_car(car *ptr, int carId) {
    ptr->id = carId;
    ptr->best_s1 = 45;// 45 because we have to provide a value and 45 is the worst time for a sector
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

/**
 * This function initializes the number of cars based on the command-line argument.
 *
 * @param arg The command-line argument.
 * @return The number of cars.
 */
int init_num_cars(char* arg) {
    int num_cars;
    if (strcmp(arg, "-Q2") == 0) {
        num_cars = 15;
    } else if (strcmp(arg, "-Q3") == 0) {
        num_cars = 10;
    } else {
        num_cars = 20;
    }
    return num_cars;
}

/**
 * This function determines the number of laps and the filename based on the command-line argument.
 *
 * @param arg The command-line argument.
 * @param num_laps Pointer to the variable where the number of laps will be stored.
 * @param filename Pointer to the variable where the filename will be stored.
 */
void determine_race_parameters(char* arg, int* num_laps, char** filename) {
    if (strcmp(arg, "-P1") == 0) {
        *num_laps = 35;
        *filename = "P1.csv";
    } else if (strcmp(arg, "-P2") == 0) {
        *num_laps = 35;
        *filename = "P2.csv";
    } else if (strcmp(arg, "-P3") == 0) {
        *num_laps = 35;
        *filename = "P3.csv";
    } else if (strcmp(arg, "-Q1") == 0) {
        *num_laps = 7;
        *filename = "Q1.csv";
    } else if (strcmp(arg, "-Q2") == 0) {
        if (access("results/Q1.csv", F_OK) != -1) {
            *num_laps = 6;
            *filename = "Q2.csv";
        } else {
            fprintf(stderr, "Error: Q1 must be completed before Q2\n");
            exit(EXIT_FAILURE);
        }
    } else if (strcmp(arg, "-Q3") == 0) {
        if (access("results/Q2.csv", F_OK) != -1) {
            *num_laps = 5;
            *filename = "Q3.csv";
        } else {
            fprintf(stderr, "Error: Q2 must be completed before Q3\n");
            exit(EXIT_FAILURE);
        }
    } else if (strcmp(arg, "-sprint") == 0) {
        *num_laps = 25;
        *filename = "sprint.csv";
    } else if (strcmp(arg, "-race") == 0) {
        *num_laps = 52;
        *filename = "race.csv";
    } else {
        fprintf(stderr, "Error: Invalid argument\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * Simulate a lap for a car.
 *
 * @param ptr A pointer to the car to lap.
 */
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

/**
 * Read the IDs of the qualified cars from a CSV file.
 *
 * @param filename The name of the CSV file.
 * @param num_qualified_cars The number of qualified cars.
 * @return A pointer to an array of the IDs of the qualified cars.
 */
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

/**
 * Read the IDs of the qualified cars from a CSV file.
 *
 * @param filename The name of the CSV file.
 * @param num_qualified_cars The number of qualified cars.
 * @return A pointer to an array of the IDs of the qualified cars.
 */
void init_circuit_with_qualified_car_ids(car* circuit, int* qualified_car_ids, int num_qualified_cars) {
    for (int i = 0; i < num_qualified_cars; i++) {
        init_car(&circuit[i], qualified_car_ids[i]);
    }
}

/**
 * Run the cars for a number of laps and write the results to a CSV file.
 *
 * @param circuit A pointer to the array of cars.
 * @param num_cars The number of cars.
 * @param num_laps The number of laps.
 * @param filename The name of the CSV file.
 */
void run_cars_and_write_results_to_csv(car* circuit, int num_cars, int num_laps, char* filename) {
    for (int i = 0; i < num_cars; i++) {
        for (int j = 0; j < num_laps; j++) {
            lap_car(&circuit[i]);
        }
    }

    bubble_sort(circuit, num_cars);
    write_to_file(filename, filename, "w", ";", num_cars, circuit);
}
