#ifndef WEATHERPROJECT_CONSTANTPARSER_H
#define WEATHERPROJECT_CONSTANTPARSER_H

#include "sharedConstants.h"

//template categories
#define CATEGORY_SIZE 10
#define TEMPGROUP_SIZE 10
typedef struct TemplateGroup {
    int size;
    char tmp[TEMPGROUP_SIZE][STRING_SIZE];
} TEMP_GROUP;
typedef struct Category {
    int size;
    TEMP_GROUP group[CATEGORY_SIZE];
} CATEGORY;
CATEGORY
        Temperature,
        Precipitation,
        Wind,
        Pressure,
        Phenomenon;

//synonym dictionaries
#define DICTIONARY_SIZE 10
#define SYNGROUP_SIZE 10
#define END_SIZE 10

#define END_MEASURE_ROWS 3
#define END_MEASURE_COLUMNS 6

typedef struct ComplexWord {
    char base[WORD_SIZE], end[END_SIZE];
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
        Adverbs,
        Nouns;

//endings table
#define END_ROWS 20
#define END_COLUMNS 6
char Endings[END_ROWS][END_COLUMNS][END_SIZE];
char EndingsMeasures[END_MEASURE_ROWS][END_MEASURE_COLUMNS][END_SIZE];


//simple values
int AverageTemperature[12], AveragePressure[12];
double *WindScale;


//global parse function
void constantParse();

#endif //WEATHERPROJECT_CONSTANTPARSER_H
