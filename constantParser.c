#include "constantParser.h"
#include <stdio.h>
#include <string.h>

void categoryParse(CATEGORY *ctg, char filename[STRING_SIZE]) {
    FILE *f = fopen(filename, "r");
    int groupID = 0;
    int tmpID = 0;
    char temp[STRING_SIZE];
    while (!feof(f)) {
        fgets(temp, STRING_SIZE, f);
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

void dictionaryParse(DICTIONARY *dic, char filename[STRING_SIZE]) {
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
    double windTemp[] = {0, 1.6, 5.5, 10.8, 17.2, 28.5};
    WindScale = windTemp;

    FILE *f; //universal file

    //average temperature and pressure by month of the year
    f = fopen("Constants/averages.txt", "r");
    for (int i = 0; i < 12; ++i) {
        fscanf(f, "%d%d", &(AverageTemperature[i]), &(AveragePressure[i]));
    }
    fclose(f);

    //endings table
    f = fopen("Constants/endings.txt", "r");
    for (int i = 0; i < END_ROWS; ++i) {
        for (int j = 0; j < END_COLUMNS; ++j) {
            fscanf(f, "%s", Endings[i][j]);
        }
    }
    fclose(f);

    //dictionaries
    dictionaryParse(&Adjectives, "Dictionaries/adjectives.txt");
    dictionaryParse(&Adverbs, "Dictionaries/adverbs.txt");
    dictionaryParse(&Nouns, "Dictionaries/nouns.txt");

    //categories
    categoryParse(&Temperature, "Templates/temperature.txt");
}
