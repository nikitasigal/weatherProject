#include "parsers.h"

#include <stdio.h>
#include <string.h>

void inputDataParse(const char *currentString, Data *data) {
    char date[STRING_SIZE], tempNight[STRING_SIZE], tempDay[STRING_SIZE], tempSense[STRING_SIZE], precipitation[STRING_SIZE],
            speed[STRING_SIZE], direction[STRING_SIZE], gusts[STRING_SIZE], pressure[STRING_SIZE], scene[STRING_SIZE];

    // Считывание строк из таблицы
    sscanf(currentString, "%s%s%s%s%s%s%s%s%s%s", date, tempNight, tempDay, tempSense, precipitation, speed, direction, gusts, pressure, scene);

    // Обработка даты
    sscanf(date, "%d.%d.%d", &data->curDate.day, &data->curDate.month, &data->curDate.year);
    strcpy(data->curDayStr[3].word[0], date);

    // Обработка температур (ночью - днем - по ощущениям)
    int countTempNight = sscanf(tempNight, "%d..%d", &data->curDayNums[0], &data->curDayNums[1]);
    if (countTempNight == 1)    // If there is only one number in column, then maximum = minimum
        data->curDayNums[1] = data->curDayNums[0];

    int countTempDay = sscanf(tempDay, "%d..%d", &data->curDayNums[2], &data->curDayNums[3]);
    if (countTempDay == 1)
        data->curDayNums[3] = data->curDayNums[2];

    int countTempSense = sscanf(tempSense, "%d..%d", &data->curDayNums[4], &data->curDayNums[5]);
    if (countTempSense == 1)
        data->curDayNums[5] = data->curDayNums[4];


    // Обработка осдадков
    data->curDayStr[0].size = sscanf(precipitation, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]", data->curDayStr[0].word[0], data->curDayStr[0].word[1],
                                     data->curDayStr[0].word[2], data->curDayStr[0].word[3],
                                     data->curDayStr[0].word[4], data->curDayStr[0].word[5]);
    if (strcmp(data->curDayStr[0].word[0], "нет") == 0)
        data->curDayStr[0].size = 0;

    // Обработка направлений ветра
    int countSpeed = sscanf(speed, "%d-%d", &data->curDayNums[6], &data->curDayNums[7]);
    if (countSpeed == 1)
        data->curDayNums[7] = data->curDayNums[6];

    data->curDayStr[1].size = sscanf(direction, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]", data->curDayStr[1].word[0], data->curDayStr[1].word[1],
                                     data->curDayStr[1].word[2], data->curDayStr[1].word[3], data->curDayStr[1].word[4], data->curDayStr[1].word[5]);
    if (strcmp(data->curDayStr[1].word[0], "нет") == 0)
        data->curDayStr[1].size = 0;

    // Обработка направлений ветра
    for (int i = 0; i < data->curDayStr[1].size; ++i) {
        int first = (int) data->curDayStr[1].word[i][1];
        if (strlen(data->curDayStr[1].word[i]) == 5) {
            int second = (int) data->curDayStr[1].word[i][4];
            switch (first) {
                case -95:
                    strcpy(data->curDayStr[1].word[i], "северо-");
                    break;
                case -82:
                    strcpy(data->curDayStr[1].word[i], "юго-");
                    break;
                default:
                    break;
            }
            switch (second) {
                case -110:
                    strcat(data->curDayStr[1].word[i], "восточный");
                    break;
                case -105:
                    strcat(data->curDayStr[1].word[i], "западный");
                    break;
                default:
                    break;
            }
        } else {
            data->curDayStr[1].word[i][0] = '\0';     // UTF-8 must die
            switch (first) {
                case -95:
                    strcpy(data->curDayStr[1].word[i], "северный");
                    break;
                case -110:
                    strcat(data->curDayStr[1].word[i], "восточный");
                    break;
                case -82:
                    strcpy(data->curDayStr[1].word[i], "южный");
                    break;
                case -105:
                    strcat(data->curDayStr[1].word[i], "западный");
                    break;
                default:
                    break;
            }
        }
    }

    // Обработка порывов ветра
    int countGusts = sscanf(gusts, "%d-%d", &data->curDayNums[8], &data->curDayNums[9]);
    if (countGusts == 1)
        data->curDayNums[9] = data->curDayNums[8];

    // Обработка давления
    sscanf(pressure, "%d", &data->curDayNums[10]);

    // Обработка явлений
    data->curDayStr[2].size = sscanf(scene, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]", data->curDayStr[2].word[0], data->curDayStr[2].word[1],
                                     data->curDayStr[2].word[2], data->curDayStr[2].word[3],
                                     data->curDayStr[2].word[4], data->curDayStr[2].word[5]);
    if (strcmp(data->curDayStr[2].word[0], "нет") == 0)
        data->curDayStr[2].size = 0;
}


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

void constantParse(Data *data) {
    //wind force scale
    //it is not possible to initialize global variables in the .h files
    //as a result, it will be initialized multiple times in different .c files and will cause conflicts.
    data->StatWindScale[0] = 0;
    data->StatWindScale[1] = 5.4;
    data->StatWindScale[2] = 10.5;
    data->StatWindScale[3] = 20.8;

    FILE *f;  //universal file variable

    //average temperature and pressure by month of the year
    f = fopen("Constants/statistics.txt", "r");
    for (int i = 0; i < 12; ++i) {
        fscanf(f, "%d%d", &(data->StatTemperature[i]), &(data->StatPressure[i]));
    }
    fclose(f);

    //table of adjectives' endings used in %function
    f = fopen("Constants/adjectives_endings.txt", "r");
    for (int i = 0; i < ADJ_END_ROWS; ++i) {
        for (int j = 0; j < ADJ_END_COLUMNS; ++j) {
            fscanf(f, "%s", data->AdjEndings[i][j]);
        }
    }
    fclose(f);

    //table of nouns' endings used in *function
    f = fopen("Constants/nouns_endings.txt", "r");
    for (int i = 0; i < NOUN_END_ROWS; ++i) {
        for (int j = 0; j < NOUN_END_COLUMNS; ++j) {
            fscanf(f, "%s", data->NounEndings[i][j]);
        }
    }
    fclose(f);

    //extracting dictionaries
    dictionaryParse(&data->Adjectives, "Dictionaries/adjectives.txt");
    dictionaryParse(&data->Adverbs, "Dictionaries/adverbs.txt");

    //extracting weather template categories
    categoryParse(&data->Temperature, "Templates/temperature.txt");
    categoryParse(&data->Wind, "Templates/wind.txt");
    categoryParse(&data->Pressure, "Templates/pressure.txt");
    categoryParse(&data->Events, "Templates/events.txt");
    categoryParse(&data->BeginSentence, "Templates/beginSentence.txt");

    //extracting components of complex text templates
    categoryParse(&data->TextBeginnings, "Templates/Text Complex Parts/beginnings.txt");
    categoryParse(&data->TextFollowups, "Templates/Text Complex Parts/followups.txt");
}
