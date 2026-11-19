#ifndef HIGH_SCORE_H
#define HIGH_SCORE_H

#include <stdbool.h>

int high_score_load(void);
bool high_score_save(int score);

#endif