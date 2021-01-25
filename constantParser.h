//
// Created by paral on 25.01.2021.
//

#ifndef WEATHERPROJECT_CONSTANTPARSER_H
#define WEATHERPROJECT_CONSTANTPARSER_H

#define GROUP_COUNT 10
#define GROUP_SIZE 10
#define WORD_SIZE 100
#define END_SIZE 10

typedef struct ComplexWord {
    char base[WORD_SIZE], end[END_SIZE];
} COMP_WORD;
typedef struct SynonymGroup {
    COMP_WORD synonyms[GROUP_SIZE];
} SYN_GROUP;

SYN_GROUP
        Adjectives[GROUP_COUNT],
        Adverbs[GROUP_COUNT],
        Nouns[GROUP_COUNT];

int AvTemp[12], AvPressure[12];
double WindScale[] = {0, 1.6, 5.5, 10.8, 17.2, 28.5};

void constantParse();

#endif //WEATHERPROJECT_CONSTANTPARSER_H
