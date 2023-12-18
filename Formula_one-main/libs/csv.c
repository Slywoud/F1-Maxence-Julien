#include "f1.h"

void write_to_file(char* race, char* filename, char* mode, char* separator, int num_cars, car *bracket) {
    FILE *fpt;

    fpt = fopen(filename, mode);

    fprintf(fpt,"%s\nid%s best s1%s best s2 %s best s3 %s best lap%s total time\n",race, separator, separator, separator, separator, separator);

    for (int i = 0; i < num_cars; i++) {
        fprintf( fpt, "%2d", bracket[i].id);
        fprintf( fpt, "%s", separator);
        fprintf( fpt, "%-5.4g", bracket[i].best_s1);
        fprintf( fpt, "%s", separator);
        fprintf( fpt, "%-5.4g", bracket[i].best_s2);
        fprintf( fpt, "%s", separator);
        fprintf( fpt, "%-5.4g", bracket[i].best_s3);
        fprintf( fpt, "%s", separator);
        fprintf( fpt, "%-5.4g", bracket[i].best_lap);
        fprintf( fpt, "%s", separator);
        fprintf( fpt, "%-5.4g", bracket[i].total_time);
        fprintf( fpt, "%s", separator);
    }
    fprintf(fpt, "\n");
}

void fill_car(char line[], char* separator, car *temp) {
    const char* tok;

    tok = strtok(line, separator);
    temp->id = atoi(tok);
    tok = strtok(NULL, separator);
    temp->best_s1 = atof(tok);
    tok = strtok(NULL, separator);
    temp->best_s2 = atof(tok);
    tok = strtok(NULL, separator);
    temp->best_s3 = atof(tok);
    tok = strtok(NULL, separator);
    temp->best_lap = atof(tok);
    tok = strtok(NULL, separator);
    temp->total_time = atof(tok);

    temp->s1 = 0;
    temp->s2 = 0;
    temp->s3 = 0;
    temp->has_best_s1 = 0;
    temp->has_best_s2 = 0;
    temp->has_best_s3 = 0;
    temp->state_pitstop = false;
    temp->state_crash = false;
}

void read_from_file(char* filename, char* separator, char* race, int num_cars, car* bracket) {
    FILE *fpt;
    char line[1024];
    car test;

    fpt = fopen(filename, "r");

    while(fgets(line, 1024, fpt)){
        char* tmp = strdup(line);
        const char* tok = strtok(tmp, "\n");
        if(!strcmp(tok,race)){
            fgets(line, 1024, fpt);
            break;
        }
    }

    for(int i = 0; i < num_cars ; i++){
        fgets(line, 1024, fpt);
        fill_car(line, separator, &test);
        bracket[i] = test;
    }
}
