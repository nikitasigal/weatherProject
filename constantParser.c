#include "constantParser.h"
#include <stdio.h>
#include <string.h>

void categoryParse(TEMP_CATEGORY *ctg, const char filename[STRING_SIZE]) {
    FILE *f = fopen(filename, "r");
    int groupID = 0;
    int tmpID = 0;
    char temp[STRING_SIZE];
    while (!feof(f)) {
        fgets(temp, STRING_SIZE, f);
        temp[strlen(temp) - 1] = 0;
        if (temp[0] == '!') {
            groupID++;
            (*ctg).size++;
            tmpID = 0;
        } else {
            strcpy((*ctg).group[groupID].tmp[tmpID], temp);
            tmpID++;
            (*ctg).group[groupID].size++;
        }
    }
    fclose(f);
}

void dictionaryParse(DICTIONARY *dic, const char filename[STRING_SIZE]) {
    FILE *f = fopen(filename, "r");
    int groupID = 0;
    int synID = 0;
    char temp[STRING_SIZE];
    while (!feof(f)) {
        fgets(temp, STRING_SIZE, f);
        temp[strlen(temp) - 1] = 0;
        if (temp[0] == '!') {
            groupID++;
            (*dic).size++;
            synID = 0;
        } else {
            char s1[WORD_SIZE] = {0}, s2[WORD_SIZE] = {0};
            sscanf(temp, "%s%s", s1, s2);
            strcpy((*dic).group[groupID].syn[synID].base, s1);
            strcpy((*dic).group[groupID].syn[synID].end, s2);
            synID++;
            (*dic).group[groupID].size++;
        }
    }
    fclose(f);
}

void constantParse() {
    //wind force scale
    //it is not possible to initialize global variables in the .h files
    //as a result, it will be initialized multiple times in different .c files and will cause conflicts
    StatWindScale[0] = 0;
    StatWindScale[1] = 5.4;
    StatWindScale[2] = 10.5;
    StatWindScale[3] = 20.8;

    FILE *f;  //universal file variable

    //average temperature and pressure by month of the year
    f = fopen("Constants/statistics.txt", "r");
    for (int i = 0; i < 12; ++i) {
        fscanf(f, "%d%d", &(StatTemperature[i]), &(StatPressure[i]));
    }
    fclose(f);

    //table of adjectives' endings used in %function
    f = fopen("Constants/adjectives_endings.txt", "r");
    for (int i = 0; i < ADJ_END_ROWS; ++i) {
        for (int j = 0; j < ADJ_END_COLUMNS; ++j) {
            fscanf(f, "%s", AdjEndings[i][j]);
        }
    }
    fclose(f);

    //table of nouns' endings used in *function
    f = fopen("Constants/nouns_endings.txt", "r");
    for (int i = 0; i < NOUN_END_ROWS; ++i) {
        for (int j = 0; j < NOUN_END_COLUMNS; ++j) {
            fscanf(f, "%s", NounEndings[i][j]);
        }
    }
    fclose(f);

    //extracting dictionaries
    dictionaryParse(&Adjectives, "Dictionaries/adjectives.txt");
    dictionaryParse(&Adverbs, "Dictionaries/adverbs.txt");

    //extracting weather template categories
    categoryParse(&Temperature, "Templates/temperature.txt");
    categoryParse(&Wind, "Templates/wind.txt");
    categoryParse(&Pressure, "Templates/pressure.txt");
    categoryParse(&Events, "Templates/events.txt");

    //extracting components of complex text templates
    categoryParse(&TextBeginnings, "Templates/Text Complex Parts/beginnings.txt");
    categoryParse(&TextFollowups, "Templates/Text Complex Parts/followups.txt");
}