#ifndef WEATHERPROJECT_CONSTANTPARSER_H
#define WEATHERPROJECT_CONSTANTPARSER_H

//synonym groups
#define GROUP_COUNT 10
#define GROUP_SIZE 10
#define WORD_SIZE 100
#define END_SIZE 10
typedef struct ComplexWord {
    char base[WORD_SIZE], end[END_SIZE];
} COMP_WORD;
typedef struct SynonymGroup {
    int size;
    COMP_WORD synonyms[GROUP_SIZE];
} SYN_GROUP;
SYN_GROUP
        Adjectives[GROUP_COUNT],
        Adverbs[GROUP_COUNT],
        Nouns[GROUP_COUNT];
int
        AdjectivesCnt,
        AdverbsCnt,
        NounsCnt;

//endings table
#define END_ROWS 20
#define END_COLUMNS 6
char Endings[END_ROWS][END_COLUMNS][END_SIZE];


//simple values
int AvTemp[12], AvPressure[12];
double *WindScale;


//global parse function
void constantParse();

#endif //WEATHERPROJECT_CONSTANTPARSER_H
