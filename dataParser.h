//
// Created by kostya on 25.01.2021.
//

#ifndef WEATHERPROJECT_DATAPARSER_H
#define WEATHERPROJECT_DATAPARSER_H

// Size of most arrays
#define STRING_SIZE 300
#define NUMS_SIZE 11
#define WORD_SIZE 6
#define CATEGORIES_SIZE 3

// Local structure for storing a date
typedef struct {
    int day, month, year;
} Date;

int curDayNums[NUMS_SIZE];                               // 11 чисел из файла
char curDayStr[CATEGORIES_SIZE][WORD_SIZE][STRING_SIZE]; /* Первый индекс - категория (осадки, направление, явления), второй - конкретное явление
                                                         из файла. Предполагаю, что не может быть больше 6 явлений за один раз */
Date curDate;                                            // curDate.day / curDate.month / curDate.year

// Предыдущий день:
int prevDayNums[11];
char prevDayStr[3][6][STRING_SIZE];
Date prevDate;

int curCountDirections; // Количество направлений ветра в день. Нужно для переопределения предыдущего дня
int curCountScenes;     // Количество явлений в день. Нужно для переопределения предыдущего дня
int prevCountDirections;
int prevCountScenes;

int countDays;  // Количество дней

void dataParser(const char *currentString);

#endif //WEATHERPROJECT_DATAPARSER_H
