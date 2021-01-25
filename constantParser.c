#include "constantParser.h"
#include <stdio.h>
#include <string.h>

void constantParse() {
    //wind force scale
    double windTemp[] = {0, 1.6, 5.5, 10.8, 17.2, 28.5};
    WindScale = windTemp;

    FILE *f; //universal file

    //average temperature and pressure by month of the year
    f = fopen("Dictionaries/averages.txt", "r");
    for (int i = 0; i < 12; ++i) {
        fscanf(f, "%d%d", &(AvTemp[i]), &(AvPressure[i]));
    }
    fclose(f);

    //endings table
    f = fopen("Dictionaries/endings.txt", "r");
    for (int i = 0; i < END_ROWS; ++i) {
        for (int j = 0; j < END_COLUMNS; ++j) {
            fscanf(f, "%s", Endings[i][j]);
        }
    }
    fclose(f);

    //dictionary variables
    int id; //group number
    int grid; //word number in the group
    char temp[WORD_SIZE]; //temporary string

    //adjectives groups
    f = fopen("Dictionaries/adjectives.txt", "r");
    id = 0;
    grid = 0;
    while (!feof(f)) {
        fscanf(f, "%s", temp);
        if (temp[0] == '!') {
            id++;
            AdjectivesCnt++;
            grid = 0;
        } else {
            strcpy(Adjectives[id].synonyms[grid].base, temp);
            fscanf(f, "%s", temp);
            strcpy(Adjectives[id].synonyms[grid].end, temp);
            grid++;
            Adjectives[id].size++;
        }
    }
    fclose(f);

    //adverbs groups
    f = fopen("Dictionaries/adverbs.txt", "r");
    id = 0;
    grid = 0;
    while (!feof(f)) {
        fgets(temp, WORD_SIZE, f);
        temp[strlen(temp) - 1] = 0;
        if (temp[0] == '!') {
            id++;
            AdverbsCnt++;
            grid = 0;
        } else {
            strcpy(Adverbs[id].synonyms[grid].base, temp);
            grid++;
            Adverbs[id].size++;
        }
    }
    fclose(f);

    //nouns groups
    f = fopen("Dictionaries/nouns.txt", "r");
    id = 0;
    grid = 0;
    while (!feof(f)) {
        fgets(temp, WORD_SIZE, f);
        temp[strlen(temp) - 1] = 0;
        if (temp[0] == '!') {
            id++;
            NounsCnt++;
            grid = 0;
        } else {
            strcpy(Nouns[id].synonyms[grid].base, temp);
            grid++;
            Nouns[id].size++;
        }
    }
    fclose(f);
}
