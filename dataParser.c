#include <stdio.h>
#include <string.h>

#include "dataParser.h"

void dataParse(const char *currentString) {
    // Copy curDay struct to prevDay (excepting the first day)
    if (countDays != 0) {
        prevDate.year = curDate.year;
        prevDate.month = curDate.month;
        prevDate.day = curDate.day;

        prevDayNums[0] = curDayNums[0];
        prevDayNums[1] = curDayNums[1];

        prevDayNums[2] = curDayNums[2];
        prevDayNums[3] = curDayNums[3];

        prevDayNums[4] = curDayNums[4];
        prevDayNums[5] = curDayNums[5];

        for (int i = 0; i < curDayStr[0].size; ++i) {
            strcpy(prevDayStr[0].word[i], curDayStr[0].word[i]);
        }

        prevDayNums[6] = curDayNums[6];
        prevDayNums[7] = curDayNums[7];

        for (int i = 0; i < curDayStr[1].size; ++i) {
            strcpy(prevDayStr[1].word[i], curDayStr[1].word[i]);
        }

        prevDayNums[8] = curDayNums[8];
        prevDayNums[9] = curDayNums[9];

        prevDayNums[10] = curDayNums[10];

        for (int i = 0; i < curDayStr[2].size; ++i) {
            strcpy(prevDayStr[2].word[i], curDayStr[2].word[i]);
        }

        strcpy(prevDayStr[3].word[0], curDayStr[3].word[0]);

        prevDayStr[0].size = curDayStr[0].size;
        prevDayStr[1].size = curDayStr[1].size;
        prevDayStr[2].size = curDayStr[2].size;
        prevDayStr[3].size = curDayStr[3].size;
    }

    char dateTemp[STRING_SIZE], tempNight[STRING_SIZE], tempDay[STRING_SIZE], tempSense[STRING_SIZE], precipitation[STRING_SIZE],
            speed[STRING_SIZE], direction[STRING_SIZE], gusts[STRING_SIZE], pressure[STRING_SIZE], scene[STRING_SIZE];

    sscanf(currentString, "%s%s%s%s%s%s%s%s%s%s", dateTemp, tempNight, tempDay, tempSense, precipitation, speed, direction, gusts, pressure, scene);
    //printf("%s %s %s %s %s %s %s %s %s %s\n", dateTemp, tempNight, tempDay, tempSense, precipitation, speed, direction, gusts, pressure, scene);

    sscanf(dateTemp, "%d.%d.%d", &curDate.day, &curDate.month, &curDate.year);
    //printf("%d %d %d\n", curDate.day, curDate.month, curDate.year);

    int countTempNight = sscanf(tempNight, "%d..%d", &curDayNums[0], &curDayNums[1]);
    if (countTempNight == 1)    // If there is only one number in column, then maximum = minimum
        curDayNums[1] = curDayNums[0];
    //printf("%d  %d\n", curDayNums[0], curDayNums[1]);

    int countTempDay = sscanf(tempDay, "%d..%d", &curDayNums[2], &curDayNums[3]);
    if (countTempDay == 1)
        curDayNums[3] = curDayNums[2];
    //printf("%d  %d\n", curDayNums[2], curDayNums[3]);

    int countTempSense = sscanf(tempSense, "%d..%d", &curDayNums[4], &curDayNums[5]);
    if (countTempSense == 1)
        curDayNums[5] = curDayNums[4];
    //printf("%d  %d\n", curDayNums[4], curDayNums[5]);

    curDayStr[0].size = sscanf(precipitation, "%[^,\n],%[^,\n],%[^,\n],%[^,\n],%[^,\n],%[^,\n]", curDayStr[0].word[0], curDayStr[0].word[1],
                            curDayStr[0].word[2], curDayStr[0].word[3],
                            curDayStr[0].word[4], curDayStr[0].word[5]);
    if (strcmp(curDayStr[0].word[0], "нет") == 0)
        curDayStr[0].size = 0;
    /*for (int i = 0; i < curDayStr[0].size; ++i) {
        printf("%s ", curDayStr[0][i]);
    }
    printf("\n");*/

    int countSpeed = sscanf(speed, "%d-%d", &curDayNums[6], &curDayNums[7]);
    if (countSpeed == 1)
        curDayNums[7] = curDayNums[6];
    //printf("%d  %d\n", curDayNums[6], curDayNums[7]);

    curDayStr[1].size = sscanf(direction, "%[^,\n],%[^,\n],%[^,\n],%[^,\n],%[^,\n],%[^,\n]", curDayStr[1].word[0], curDayStr[1].word[1],
                                curDayStr[1].word[2], curDayStr[1].word[3], curDayStr[1].word[4], curDayStr[1].word[5]);
    if (strcmp(curDayStr[1].word[0], "нет") == 0)
        curDayStr[1].size = 0;
    // Обработка направлений ветра
    for (int i = 0; i < curDayStr[1].size; ++i) {
        int first = (int)curDayStr[1].word[i][1];
        if (strlen(curDayStr[1].word[i]) == 5) {
            int second = (int)curDayStr[1].word[i][4];
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
    /*for (int i = 0; i < curDayStr[1].size; ++i) {
        printf("%s ", curDayStr[1].word[i]);
    }
    printf("\n");*/

    strcpy(curDayStr[3].word[0], gusts);     // Добавление порывов в массив строк
    curDayStr[3].size = 1;
    int countGusts = sscanf(gusts, "%d-%d", &curDayNums[8], &curDayNums[9]);
    if (countGusts == 1)
        curDayNums[9] = curDayNums[8];
    //printf("%d  %d\n", curDayNums[8], curDayNums[9]);


    sscanf(pressure, "%d", &curDayNums[10]);
    //printf("%d\n", curDayNums[10]);

    curDayStr[2].size = sscanf(scene, "%[^,\n],%[^,\n],%[^,\n],%[^,\n],%[^,\n],%[^,\n]", curDayStr[2].word[0], curDayStr[2].word[1],
                            curDayStr[2].word[2], curDayStr[2].word[3],
                            curDayStr[2].word[4], curDayStr[2].word[5]);
    if (strcmp(curDayStr[2].word[0], "нет") == 0)
        curDayStr[2].size = 0;
    /*for (int i = 0; i < curDayStr[2].size; ++i) {
        printf("%s ", curDayStr[2][i]);
    }
    printf("\n");*/

    // End of parsing and increasing of countDays by one
    ++countDays;
}
