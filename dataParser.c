#include <stdio.h>
#include <string.h>

#include "dataParser.h"

void dataParse(const char *currentString, Data *data) {
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
