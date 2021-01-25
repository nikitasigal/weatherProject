#include <stdio.h>
#include <string.h>
#include <windows.h>

// Size of most arrays
#define SIZE 300

// Local structure for storing a date
typedef struct {
    int day, month, year;
} Date;

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    printf("Привет, мир!\n");

    FILE *data = fopen("Data Source/data.txt", "r");
    if (data == NULL) {
        printf("Error: file was not opened.");
        return 0;
    }

    char tempString[SIZE];
    fgets(tempString, SIZE, data); // Shift a carriage to the next line

    // Временные переменные для парсинга через sscanf
    char dateTemp[SIZE], tempNight[SIZE], tempDay[SIZE], tempSense[SIZE], precipitation[SIZE], speed[SIZE], direction[SIZE], gusts[SIZE], pressure[SIZE], scene[SIZE];

    int curDayNums[11];         // 11 чисел из файла
    char curDayStr[3][6][SIZE]; /* Первый индекс - категория (осадки, направление, явления), второй - конкретное явление
                                   из файла. Предполагаю, что не может быть больше 6 явлений за один раз */
    Date curDate;               // curDate.day / curDate.month / curDate.year

    // Предыдущий день:
    int prevDayNums[11];
    char prevDayStr[3][6][SIZE];
    Date prevDate;

    int curCountDirections = 0; // Количество направлений ветра в день. Нужно для переопределения предыдущего дня
    int curCountScenes = 0;     // Количество явлений в день. Нужно для переопределения предыдущего дня
    int prevCountDirections = 0;
    int prevCountScenes = 0;

    int countDays = 0;  // Количество дней
    char currentString[SIZE] = { 0 }; // Строка для чтения

    while (!feof(data) && fgets(currentString, SIZE, data)) {
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

    // DEBUG INFORMATION
    /*printf("%d %d %d | %d %d | %d %d | %d %d | %s | %d %d | ", prevDate.day, prevDate.month, prevDate.year, prevDayNums[0],
           prevDayNums[1], prevDayNums[2], prevDayNums[3], prevDayNums[4], prevDayNums[5], prevDayStr[0][0], prevDayNums[6], prevDayNums[7]);
    for (int i = 0; i < prevCountDirections; ++i) {
        printf("%s ", prevDayStr[1][i]);
    }
    printf("| ");
    printf("%d %d | %d | ", prevDayNums[8], prevDayNums[9], prevDayNums[10]);
    for (int i = 0; i < prevCountScenes; ++i) {
        printf("%s ", prevDayStr[2][i]);
    }
    printf("\n\n");*/

    /*printf("%d %d %d | %d %d | %d %d | %d %d | %s | %d %d | ", curDate.day, curDate.month, curDate.year, curDayNums[0],
           curDayNums[1], curDayNums[2], curDayNums[3], curDayNums[4], curDayNums[5], curDayStr[0][0], curDayNums[6], curDayNums[7]);
    for (int i = 0; i < curCountDirections; ++i) {
        printf("%s ", curDayStr[1][i]);
    }
    printf("| ");
    printf("%d %d | %d | ", curDayNums[8], curDayNums[9], curDayNums[10]);
    for (int i = 0; i < curCountScenes; ++i) {
        printf("%s ", curDayStr[2][i]);
    }
    printf("\n\n");*/


    fclose(data);
}
