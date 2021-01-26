#ifndef WEATHERPROJECT_DATAPARSER_H
#define WEATHERPROJECT_DATAPARSER_H

#include "sharedConstants.h"


typedef struct {
    int day, month, year;
} Date;

int curDayNums[NUMS_COUNT];                               // 11 чисел из файла
char curDayStr[CATEGORIES_COUNT][WORD_COUNT][STRING_SIZE]; /* Первый индекс - категория (осадки, направление, явления), второй - конкретное явление
                                                         из файла. Предполагаю, что не может быть больше 6 явлений за один раз */
Date curDate;                                            // curDate.day / curDate.month / curDate.year


int prevDayNums[NUMS_COUNT];
char prevDayStr[CATEGORIES_COUNT][WORD_COUNT][STRING_SIZE];
Date prevDate;


int curCountDirections; // Количество направлений ветра в день. Нужно для переопределения предыдущего дня
int curCountScenes;     // Количество явлений в день. Нужно для переопределения предыдущего дня
int prevCountDirections;
int prevCountScenes;

int countDays;  // Количество дней

void dataParse(const char *currentString);

#endif //WEATHERPROJECT_DATAPARSER_H
