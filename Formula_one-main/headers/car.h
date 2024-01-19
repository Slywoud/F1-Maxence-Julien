/**
 * Structure representing a car in a race.
 */
typedef struct{
    int id;
    float s1, s2, s3;
    float best_s1, best_s2, best_s3;
    int has_best_s1, has_best_s2, has_best_s3;
    float best_lap;
    float total_time;
    int pitstop;
    bool state_crash;
}car;
