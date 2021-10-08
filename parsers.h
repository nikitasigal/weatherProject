#ifndef WEATHERPROJECT_PARSERS_H
#define WEATHERPROJECT_PARSERS_H

#include "sharedConstants.h"

//template categories
#define TEMP_CATEGORY_SIZE 20
#define TEMP_GROUP_SIZE 20

typedef struct {
    int size;
    char tmp[TEMP_GROUP_SIZE][STRING_SIZE];
} TemplateGroup;

typedef struct {
    int size;
    TemplateGroup group[TEMP_CATEGORY_SIZE];
} TemplateCategory;

//synonym dictionaries
#define DICTIONARY_SIZE 20
#define SYNGROUP_SIZE 20

typedef struct {
    char base[WORD_SIZE], end[WORD_SIZE];
} ComplexWord;

typedef struct {
    int size;
    ComplexWord syn[SYNGROUP_SIZE];
} SynonymGroup;

typedef struct {
    int size;
    SynonymGroup group[DICTIONARY_SIZE];
} Dictionary;

//endings tables
#define ADJ_END_ROWS 20
#define ADJ_END_COLUMNS 6
#define NOUN_END_ROWS 3
#define NOUN_END_COLUMNS 6

typedef struct {
    int day, month, year;
} Date;

typedef struct {
    int size;
    char word[WORD_COUNT][STRING_SIZE];
} Strings;

typedef struct {
    double rating;
    int ctg;
} Connection;

typedef struct {
    double StatWindScale[4];
    int StatTemperature[12];    //Average statistical temperatures by month of the year
    int StatPressure[12];       //Average statistical pressures by month of the year
    char AdjEndings[ADJ_END_ROWS][ADJ_END_COLUMNS][WORD_SIZE];
    char NounEndings[NOUN_END_ROWS][NOUN_END_COLUMNS][WORD_SIZE];
    Dictionary
            Adjectives,
            Adverbs;
    TemplateCategory
            Temperature,
            Wind,
            Pressure,
            Events,
            TextBeginnings,
            TextFollowups,
            BeginSentence;

    // Parsing data
    int curDayNums[NUMS_COUNT];                               // 11 чисел из файла
    Strings curDayStr[CATEGORIES_COUNT - 1];
    Date curDate;                                            // curDate.day / curDate.month / curDate.year

    Connection order[CATEGORIES_COUNT];
} Data;

//global parse functions
void constantParse(Data *data);

void inputDataParse(const char *currentString, Data *data);

#endif //WEATHERPROJECT_PARSERS_H
