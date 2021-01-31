/*
 *  Основные массивы для данных (из dataParse.h):
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
#include <string.h>
#include <time.h>
#include <windows.h>

#include "sharedConstants.h"
#include "constantParser.h"
#include "dataParser.h"
#include "sentenceHandler.h"

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    //random generateSentence
    srand(time(NULL));

    //global files parse
    constantParse();

    //opening forecast data file
    FILE *data = fopen("Forecast Data/data.txt", "r");
    if (data == NULL) {
        printf("Error: file was not opened.");
        return 0;
    }

    char currentString[STRING_SIZE] = { 0 }; // I think everyone knows what is this for
    fgets(currentString, STRING_SIZE, data); // skipping the first lines with table headings

    //THE MAIN PROGRAM CYCLE
    //ALL THE MAGIC HAPPENS HERE
    while (!feof(data) && fgets(currentString, STRING_SIZE, data)) {
        dataParse(currentString);
        generateSentence("Температура", 2);    // Вызов генератора для тестаВ
        //generator("Температура", 1);    // Вызов генератора для тестаВ
        calcRate("");
        generateSentence("Температура", 2);

        // Вывод UTF-блевотины
        /*char* alphabet = "АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдежзийклмнопрстуфхцчшщъыьэюя";
        for (int i = 0; i < strlen(alphabet); i += 2) {
            printf("%c%c - %d, %d\n", alphabet[i], alphabet[i + 1], (int) (alphabet[i]), (int) (alphabet[i + 1]));
        }*/
        //printf("%lf\n", calcRate("Ветер"));
    }

    //closing forecast data file
    fclose(data);



    //DEBUG for constantParse
    //printf("%s", Temperature.group[5].tmp[4]);

    // DEBUG for dataParse
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

    return 0;
}

