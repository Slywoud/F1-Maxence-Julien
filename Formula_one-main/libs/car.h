typedef struct{
    int id;
    float s1, s2, s3;
    float best_s1, best_s2, best_s3;
    int has_best_s1, has_best_s2, has_best_s3;
    float best_lap;
    float total_time;
    bool state_pitstop;
    bool state_crash;
}car;

static int carIds[] = {44, 63, 1, 11, 55, 16, 4, 3, 14, 31, 10, 22,
                       5, 18, 6, 23, 77, 24, 47, 9};
