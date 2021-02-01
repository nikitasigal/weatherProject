#include <stdio.h>
#include <string.h>

#include "dataParser.h"

void dataParse(const char *currentString) {
    char date[STRING_SIZE], tempNight[STRING_SIZE], tempDay[STRING_SIZE], tempSense[STRING_SIZE], precipitation[STRING_SIZE],
            speed[STRING_SIZE], direction[STRING_SIZE], gusts[STRING_SIZE], pressure[STRING_SIZE], scene[STRING_SIZE];

    // Считывание строк из таблицы
    sscanf(currentString, "%s%s%s%s%s%s%s%s%s%s", date, tempNight, tempDay, tempSense, precipitation, speed, direction, gusts, pressure, scene);

    // Обработка даты
    sscanf(date, "%d.%d.%d", &curDate.day, &curDate.month, &curDate.year);
    strcpy(curDayStr[3].word[0], date);

    // Обработка температур (ночью - днем - по ощущениям)
    int countTempNight = sscanf(tempNight, "%d..%d", &curDayNums[0], &curDayNums[1]);
    if (countTempNight == 1)    // If there is only one number in column, then maximum = minimum
        curDayNums[1] = curDayNums[0];

    int countTempDay = sscanf(tempDay, "%d..%d", &curDayNums[2], &curDayNums[3]);
    if (countTempDay == 1)
        curDayNums[3] = curDayNums[2];

    int countTempSense = sscanf(tempSense, "%d..%d", &curDayNums[4], &curDayNums[5]);
    if (countTempSense == 1)
        curDayNums[5] = curDayNums[4];


    // Обработка осдадков
    curDayStr[0].size = sscanf(precipitation, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]", curDayStr[0].word[0], curDayStr[0].word[1],
                               curDayStr[0].word[2], curDayStr[0].word[3],
                               curDayStr[0].word[4], curDayStr[0].word[5]);
    if (strcmp(curDayStr[0].word[0], "нет") == 0)
        curDayStr[0].size = 0;

    // Обработка направлений ветра
    int countSpeed = sscanf(speed, "%d-%d", &curDayNums[6], &curDayNums[7]);
    if (countSpeed == 1)
        curDayNums[7] = curDayNums[6];

    curDayStr[1].size = sscanf(direction, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]", curDayStr[1].word[0], curDayStr[1].word[1],
                               curDayStr[1].word[2], curDayStr[1].word[3], curDayStr[1].word[4], curDayStr[1].word[5]);
    if (strcmp(curDayStr[1].word[0], "нет") == 0)
        curDayStr[1].size = 0;

    // Обработка направлений ветра
    for (int i = 0; i < curDayStr[1].size; ++i) {
        int first = (int) curDayStr[1].word[i][1];
        if (strlen(curDayStr[1].word[i]) == 5) {
            int second = (int) curDayStr[1].word[i][4];
            switch (first) {
                case -95:
                    strcpy(curDayStr[1].word[i], "северо-");
                    break;
                case -82:
                    strcpy(curDayStr[1].word[i], "юго-");
                    break;
                default:
                    break;
            }
            switch (second) {
                case -110:
                    strcat(curDayStr[1].word[i], "восточный");
                    break;
                case -105:
                    strcat(curDayStr[1].word[i], "западный");
                    break;
                default:
                    break;
            }
        } else {
            curDayStr[1].word[i][0] = '\0';     // UTF-8 must die
            switch (first) {
                case -95:
                    strcpy(curDayStr[1].word[i], "северный");
                    break;
                case -110:
                    strcat(curDayStr[1].word[i], "восточный");
                    break;
                case -82:
                    strcpy(curDayStr[1].word[i], "южный");
                    break;
                case -105:
                    strcat(curDayStr[1].word[i], "западный");
                    break;
                default:
                    break;
            }
        }
    }

    // Обработка порывов ветра
    int countGusts = sscanf(gusts, "%d-%d", &curDayNums[8], &curDayNums[9]);
    if (countGusts == 1)
        curDayNums[9] = curDayNums[8];

    // Обработка давления
    sscanf(pressure, "%d", &curDayNums[10]);

    // Обработка явлений
    curDayStr[2].size = sscanf(scene, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]", curDayStr[2].word[0], curDayStr[2].word[1],
                               curDayStr[2].word[2], curDayStr[2].word[3],
                               curDayStr[2].word[4], curDayStr[2].word[5]);
    if (strcmp(curDayStr[2].word[0], "нет") == 0)
        curDayStr[2].size = 0;
}
