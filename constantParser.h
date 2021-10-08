#ifndef WEATHERPROJECT_CONSTANTPARSER_H
#define WEATHERPROJECT_CONSTANTPARSER_H

#include "sharedConstants.h"

//template categories
#define TEMP_CATEGORY_SIZE 20
#define TEMP_GROUP_SIZE 20

typedef struct TemplateGroup {
    int size;
    char tmp[TEMP_GROUP_SIZE][STRING_SIZE];
} TEMP_GROUP;

typedef struct TemplateCategory {
    int size;
    TEMP_GROUP group[TEMP_CATEGORY_SIZE];
} TEMP_CATEGORY;

//synonym dictionaries
#define DICTIONARY_SIZE 20
#define SYNGROUP_SIZE 20

typedef struct ComplexWord {
    char base[WORD_SIZE], end[WORD_SIZE];
} COMP_WORD;

typedef struct SynonymGroup {
    int size;
    COMP_WORD syn[SYNGROUP_SIZE];
} SYN_GROUP;

typedef struct Dictionary {
    int size;
    SYN_GROUP group[DICTIONARY_SIZE];
} DICTIONARY;

//endings tables
#define ADJ_END_ROWS 20
#define ADJ_END_COLUMNS 6
#define NOUN_END_ROWS 3
#define NOUN_END_COLUMNS 6

// From dataParser.h
typedef struct {
    int day, month, year;
} Date;

typedef struct {
    int size;
    char word[WORD_COUNT][STRING_SIZE];
} Strings;

// From evaluateLevels.h
typedef struct Connection {
    double rating;
    int ctg;
} CONNECTION;

typedef struct {
    double StatWindScale[4];
    int StatTemperature[12];    //Average statistical temperatures by month of the year
    int StatPressure[12];       //Average statistical pressures by month of the year
    char AdjEndings[ADJ_END_ROWS][ADJ_END_COLUMNS][WORD_SIZE];
    char NounEndings[NOUN_END_ROWS][NOUN_END_COLUMNS][WORD_SIZE];
    DICTIONARY
            Adjectives,
            Adverbs;
    TEMP_CATEGORY
            Temperature,
            Wind,
            Pressure,
            Events,
            TextBeginnings,
            TextFollowups,
            BeginSentence;

    // Parsing data
    int curDayNums[NUMS_COUNT];                               // 11 чисел из файла
    Strings curDayStr[CATEGORIES_COUNT];
    Date curDate;                                            // curDate.day / curDate.month / curDate.year

    CONNECTION order[5];
} Data;

//global parse function
void constantParse(Data *data);

#endif //WEATHERPROJECT_CONSTANTPARSER_H
