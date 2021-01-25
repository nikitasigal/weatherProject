    #include "dataParser.h"
    #include <stdio.h>
    #include <string.h>

    void dataParser(const char *currentString) {
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

            strcpy(prevDayStr[0][0], curDayStr[0][0]);

            prevDayNums[6] = curDayNums[6];
            prevDayNums[7] = curDayNums[7];

            for (int i = 0; i < prevCountDirections; ++i) {
                strcpy(prevDayStr[1][i], "");
            }
            for (int i = 0; i < curCountDirections; ++i) {
                strcpy(prevDayStr[1][i], curDayStr[1][i]);
            }

            prevDayNums[8] = curDayNums[8];
            prevDayNums[9] = curDayNums[9];

            prevDayNums[10] = curDayNums[10];

            for (int i = 0; i < prevCountScenes; ++i) {
                strcpy(prevDayStr[2][i], "");
            }
            for (int i = 0; i < curCountScenes; ++i) {
                strcpy(prevDayStr[2][i], curDayStr[2][i]);
            }

            prevCountDirections = curCountDirections;
            prevCountScenes = curCountScenes;
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

        sscanf(precipitation, "%s", curDayStr[0][0]);
        //printf("%s\n", curDayStr[0][0]);

        int countSpeed = sscanf(speed, "%d-%d", &curDayNums[6], &curDayNums[7]);
        if (countSpeed == 1)
            curDayNums[7] = curDayNums[6];
        //printf("%d  %d\n", curDayNums[6], curDayNums[7]);

        curCountDirections = sscanf(direction, "%[^,\\n],%[^,\\n],%[^,\\n],%[^,\\n]", curDayStr[1][0], curDayStr[1][1],
                                    curDayStr[1][2], curDayStr[1][3]);
        /*for (int i = 0; i < countDirections; ++i) {
            printf("%s ", curDayStr[1][i]);
        }
        printf("\n");*/

        int countGusts = sscanf(gusts, "%d-%d", &curDayNums[8], &curDayNums[9]);
        if (countGusts == 1)
            curDayNums[9] = curDayNums[8];
        //printf("%d  %d\n", curDayNums[8], curDayNums[9]);


        sscanf(pressure, "%d", &curDayNums[10]);
        //printf("%d\n", curDayNums[10]);

        curCountScenes = sscanf(scene, "%[^,\\n],%[^,\\n],%[^,\\n],%[^,\\n],%[^,\\n],%[^,\\n]", curDayStr[2][0], curDayStr[2][1],
                                curDayStr[2][2], curDayStr[2][3],
                                curDayStr[2][4], curDayStr[2][5]);
        /*for (int i = 0; i < countScenes; ++i) {
            printf("%s ", curDayStr[2][i]);
        }
        printf("\n");*/

        // End of parsing and increasing of countDays by one
        ++countDays;
    }
