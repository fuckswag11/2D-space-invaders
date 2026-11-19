
#include "high_score.h"
#include <stdio.h>

#define HIGH_SCORE_FILE "highscore.dat"

int high_score_load(void) {
    FILE *file = fopen(HIGH_SCORE_FILE, "r");

    if (file == NULL) {
        return 0;
    }

    int score = 0;
    int result = fscanf(file, "%d", &score);

    fclose(file);

    if (result != 1 || score < 0) {
        return 0;
    }

    return score;
}

bool high_score_save(int score) {
    if (score < 0) {
        return false;
    }

    FILE *file = fopen(HIGH_SCORE_FILE, "w");

    if (file == NULL) {
        return false;
    }

    bool success = fprintf(file, "%d\n", score) >= 0;

    if (fclose(file) != 0) {
        success = false;
    }

    return success;
}
