#ifndef WEATHERPROJECT_CONSTANTPARSER_H
#define WEATHERPROJECT_CONSTANTPARSER_H

#include "sharedConstants.h"

//template categories
#define TEMP_CATEGORY_SIZE 20
#define TEMP_GROUP_SIZE 10
typedef struct TemplateGroup {
    int size;
    char tmp[TEMP_GROUP_SIZE][STRING_SIZE];
} TEMP_GROUP;
typedef struct TemplateCategory {
    int size;
    TEMP_GROUP group[TEMP_CATEGORY_SIZE];
} TEMP_CATEGORY;
TEMP_CATEGORY
        Temperature,
        Wind,
        Pressure,
        Events,
        TextBeginnings,
        TextFollowups;


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
DICTIONARY
        Adjectives,
        Adverbs;

//endings tables
#define ADJ_END_ROWS 20
#define ADJ_END_COLUMNS 6
char AdjEndings[ADJ_END_ROWS][ADJ_END_COLUMNS][WORD_SIZE];
#define NOUN_END_ROWS 3
#define NOUN_END_COLUMNS 6
char NounEndings[NOUN_END_ROWS][NOUN_END_COLUMNS][WORD_SIZE];


//simple values
int StatTemperature[12], StatPressure[12];   //Average statistical temperatures and pressures by month of the year
double *StatWindScale;                       //Scale of wind level relative to it's speed


//global parse function
void constantParse();

#endif //WEATHERPROJECT_CONSTANTPARSER_H
