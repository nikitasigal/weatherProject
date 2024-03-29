/*
 *  Основные массивы для данных (из inputDataParse.h):
 *     - curDate - структура даты текущего дня, имеет параметры day, month, year (curDate.day etc.)
 *     - prevDate - структура даты предыдущего дня (отсутствует для первого дня)
 *
 *     - curDayNums[11] - массив с числовыми данными текущего дня:
 *          • 0 - нижний порог ночной температуры - A
 *          • 1 - верхний порог ночной температуры - B
 *          • 2 - нижний порог дневной температуры - C
 *          • 3 - верхний порог дневной температуры - D
 *          • 4 - нижний порог ощущения температуры - E
 *          • 5 - верхний порог ощущения температуры - F
 *          • 6 - нижний порог скорости ветра - G
 *          • 7 - верхний порог скорости ветра - H
 *          • 8 - нижний порог порывов ветра - I
 *          • 9 - верхний поров порывов ветра - J
 *          • 10 - давление - K
 *          Если в данных только одно число, то нижний порог = верхнему порогу = числу
 *
 *     - curDayStr[4][6][300] - массив с строковыми данными текущего дня. Первый индекс - категория, второй - слово (не больше 6),
 *       третий - максимальная техническая длина строки:
 *          • 0 - категория "осадки" (не более 6) - A
 *          • 1 - направление ветра (не более 6) - B
 *          • 2 - явления (не более 6) - C
 *          • 3 - порывы ветра (в формате "10-14" или одним числом) - D
 *
 *     - prevDayNums[11] - массив с числовыми данными ПРЕДЫДУЩЕГО дня
 *     - prevDayStr[4][6][300] - массив со строковыми данными ПРЕДЫДУЩЕГО дня
 *
 *      Возможно, бесполезные переменные:
 *          int curCountDirections; // Количество направлений ветра в день. Нужно для переопределения предыдущего дня
 *          int curCountScenes;     // Количество явлений в день. Нужно для переопределения предыдущего дня
 *          int prevCountDirections;
 *          int prevCountScenes;
 *          int countDays;  // Количество дней
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#include "sharedConstants.h"
#include "parsers.h"
#include "sentenceHandler.h"
#include "evaluateLevels.h"

int main() {
    //set encoding for console
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    //random generateSimple
    srand(time(NULL));

    // Main parsed data
    Data* data = malloc(sizeof(Data));

    //global files parse
    constantParse(data);
    char filename[STRING_SIZE] = {0};
    printf("Enter forecast data file name: ");
    scanf("%s", filename);
    char dataFilename[] = "Forecast Data/";
    strcat(dataFilename, filename);

    //opening forecast dataFile file and output file
    FILE *dataFile = fopen(dataFilename, "r");
    FILE *outputFile = fopen("Forecast Data/output.txt", "w");
    if (dataFile == NULL) {
        printf("Error: forecast data file was not opened.");
        return EXIT_FAILURE;
    }
    if (outputFile == NULL) {
        printf("Error: output file was not opened.");
        return EXIT_FAILURE;
    }

    char currentString[STRING_SIZE] = {0}; // I think everyone knows what is this for
    fgets(currentString, STRING_SIZE, dataFile); // skipping the first lines with table headings

    //THE MAIN PROGRAM CYCLE
    //ALL THE MAGIC HAPPENS HERE
    while (!feof(dataFile) && fgets(currentString, STRING_SIZE, dataFile)) {
        inputDataParse(currentString, data);
        fprintf(outputFile, "%s - ", data->curDayStr[3].word[0]);
        printWeekDay(outputFile, data);

        sortCategories(data);
        generateSimple(outputFile, RATING, data);
        for (int i = 0; i < CATEGORIES_COUNT; ++i) {
            if (data->order[i].ctg == PRECIPITATION || data->order[i].ctg == EVENT)
                generateComplex(outputFile, data->order[i].ctg, data);
            else
                generateSimple(outputFile, data->order[i].ctg, data);
        }

        fprintf(outputFile, "\n\n");
    }

    //closing forecast dataFile file
    fclose(dataFile);
    fclose(outputFile);

    return 0;
}

