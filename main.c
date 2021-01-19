#include <stdio.h>
#include <string.h>
#include <windows.h>

// Size of most arrays
#define SIZE 300

// Local structure for storing a date
typedef struct {
    int day, month, year;
} Date;

// Local structure for storing a minimum and maximum value
typedef struct {
    int low, high;
} LOW_HIGH;

typedef struct {
    Date date;                  // Дата
    LOW_HIGH tempNight;         // Температура ночью
    LOW_HIGH tempDay;           // Температура днём
    LOW_HIGH tempSense;         // Ощущается
    char precipitation[SIZE];   // Осадки
    LOW_HIGH speed;             // Скорость ветра
    char direction[SIZE];       // Направление ветра
    LOW_HIGH gusts;             // Порывы ветра
    int pressure;               // Давление

    char scene[10][SIZE];       // Явления
    int sceneCount;             // Количество явлений (ограничение: 6 явлений)
} DAY;


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

    char date[SIZE], tempNight[SIZE], tempDay[SIZE], tempSense[SIZE], precipitation[SIZE], speed[SIZE], direction[SIZE], gusts[SIZE], pressure[SIZE], scene[SIZE];
    DAY curDay;     // Текущий день
    DAY prevDay;    // Предыдущий день
    int countDays = 0;  // Количество дней

    while (!feof(data)) {
        // Copy curDay struct to prevDay (excepting the first day)
        if (countDays != 0) {
            prevDay.date.year = curDay.date.year;
            prevDay.date.month = curDay.date.month;
            prevDay.date.day = curDay.date.day;

            prevDay.tempNight.low = curDay.tempNight.low;
            prevDay.tempNight.high = curDay.tempNight.high;

            prevDay.tempDay.low = curDay.tempDay.low;
            prevDay.tempDay.high = curDay.tempDay.high;

            prevDay.tempSense.low = curDay.tempSense.low;
            prevDay.tempSense.high = curDay.tempSense.high;

            strcpy(prevDay.precipitation, curDay.precipitation);

            prevDay.speed.low = curDay.speed.low;
            prevDay.speed.high = curDay.speed.high;

            strcpy(prevDay.direction, curDay.direction);

            prevDay.gusts.low = curDay.gusts.low;
            prevDay.gusts.high = curDay.gusts.high;

            prevDay.pressure = curDay.pressure;

            for (int i = 0; i < curDay.sceneCount; ++i)
                strcpy(prevDay.scene[i], curDay.scene[i]);

            prevDay.sceneCount = curDay.sceneCount;
        }

        // Reading a line and write it into temporary variables (for comfortable)
        char currentString[SIZE];
        fgets(currentString, SIZE, data);
        sscanf(currentString, "%s%s%s%s%s%s%s%s%s%s", date, tempNight, tempDay, tempSense, precipitation, speed, direction, gusts, pressure, scene);
        //printf("%s %s %s %s %s %s %s %s %s %s\n", date, tempNight, tempDay, tempSense, precipitation, speed, direction, gusts, pressure, scene);


        // Parsing every column data to struct
        sscanf(date, "%d.%d.%d", &curDay.date.day, &curDay.date.month, &curDay.date.year);
        //printf("%d.%d.%d\n", curDay.date.day, curDay.date.month, curDay.date.year);

        int countTempNight = sscanf(tempNight, "%d..%d", &curDay.tempNight.low, &curDay.tempNight.high);
        if (countTempNight == 1)    // If there is only one number in column, then maximum = minimum
            curDay.tempNight.high = curDay.tempNight.low;
        //printf("%d  %d\n", curDay.tempNight.low, curDay.tempNight.high);

        int countTempDay = sscanf(tempDay, "%d..%d", &curDay.tempDay.low, &curDay.tempDay.high);
        if (countTempDay == 1)
            curDay.tempDay.high = curDay.tempDay.low;
        //printf("%d  %d\n", curDay.tempDay.low, curDay.tempDay.high);

        int countTempSense = sscanf(tempSense, "%d..%d", &curDay.tempSense.low, &curDay.tempSense.high);
        if (countTempSense == 1)
            curDay.tempSense.high = curDay.tempSense.low;
        //printf("%d  %d\n", curDay.tempSense.low, curDay.tempSense.high);

        sscanf(precipitation, "%s", curDay.precipitation);
        //printf("%s\n", precipitation);

        int countSpeed = sscanf(speed, "%d-%d", &curDay.speed.low, &curDay.speed.high);
        if (countSpeed == 1)
            curDay.speed.high = curDay.speed.low;
        //printf("%d  %d\n", curDay.speed.low, curDay.speed.high);

        sscanf(direction, "%s", curDay.direction);
        //printf("%s\n", curDay.direction);

        int countGusts = sscanf(gusts, "%d-%d", &curDay.gusts.low, &curDay.gusts.high);
        if (countGusts == 1)
            curDay.gusts.high = curDay.gusts.low;
        //printf("%d  %d\n", curDay.gusts.low, curDay.gusts.high);

        sscanf(pressure, "%d", &curDay.pressure);
        //printf("%d\n", curDay.pressure);

        int countScenes = sscanf(scene, "%s%s%s%s%s%s", curDay.scene[0], curDay.scene[1],
                                 curDay.scene[2], curDay.scene[3],
                                 curDay.scene[4], curDay.scene[5]);
        curDay.sceneCount = countScenes;
        //for (int i = 0; i < curDay.sceneCount; ++i) {
        //    printf("%s\n", curDay.scene[i]);
        //}

        // End of parsing and increasing of countDays by one
        ++countDays;
    }

    // pattern for previous day output and current day output
    /*printf("Prev Day:\n");
    printf("%d %d %d | %d %d | %d %d | %d %d | %s | %d %d | %s | %d %d | %d | ", prevDay.date.day, prevDay.date.month,
           prevDay.date.year, prevDay.tempNight.low, prevDay.tempNight.high, prevDay.tempDay.low, prevDay.tempDay.high,
           prevDay.tempSense.low, prevDay.tempSense.high, prevDay.precipitation, prevDay.speed.low, prevDay.speed.high,
           prevDay.direction, prevDay.gusts.low, prevDay.gusts.high, prevDay.pressure);
    for (int i = 0; i < prevDay.sceneCount; ++i) {
        printf("%s ", prevDay.scene[i]);
    }
    printf("\nCur Day:\n");

    printf("%d %d %d | %d %d | %d %d | %d %d | %s | %d %d | %s | %d %d | %d | ", curDay.date.day, curDay.date.month,
           curDay.date.year, curDay.tempNight.low, curDay.tempNight.high, curDay.tempDay.low, curDay.tempDay.high,
           curDay.tempSense.low, curDay.tempSense.high, curDay.precipitation, curDay.speed.low, curDay.speed.high,
           curDay.direction, curDay.gusts.low, curDay.gusts.high, curDay.pressure);

    for (int i = 0; i < curDay.sceneCount; ++i) {
        printf("%s ", curDay.scene[i]);
    }*/

    fclose(data);
}
