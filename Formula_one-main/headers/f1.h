#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <ncurses.h>
#include <ctype.h>
#include "car.h"

int init_num_cars(char* arg);

void determine_race_parameters(char* arg, int* num_laps, char** filename);

void bubble_sort(car *list, int car_count);

void best_sector(car *list, int car_count);

float genTime(float lower, float upper);

void init_random();

void init_car(car *ptr, int carId);

void lap_car(car *ptr);

void display_scores(car *circuit, int car_count);

void write_to_file(char* race, char* filename, char* mode, char* separator, int num_cars, car *bracket);

int* read_qualified_car_ids_from_csv(char* filename, int num_qualified_cars);

void init_circuit_with_qualified_car_ids(car* circuit, int* qualified_car_ids, int num_qualified_cars);

void run_cars_and_write_results_to_csv(car* circuit, int num_cars, int num_laps, char* filename);