#ifndef WEATHERPROJECT_DATAPARSER_H
#define WEATHERPROJECT_DATAPARSER_H

#include "sharedConstants.h"

typedef struct {
    int day, month, year;
} Date;

typedef struct {
    int size;
    char word[WORD_COUNT][STRING_SIZE];
} Strings;

int curDayNums[NUMS_COUNT];                               // 11 чисел из файла
Strings curDayStr[CATEGORIES_COUNT];
Date curDate;                                            // curDate.day / curDate.month / curDate.year

void dataParse(const char *currentString);

#endif //WEATHERPROJECT_DATAPARSER_H
