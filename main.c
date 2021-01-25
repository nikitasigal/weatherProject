/*
 *  Основные массивы для данных (из dataParser.h):
 *     - curDate - структура даты текущего дня, имеет параметры day, month, year (curDate.day etc.)
 *     - prevDate - структура даты предыдущего дня (отсутствует для первого дня)
 *
 *     - curDayNums[11] - массив с числовыми данными текущего дня:
 *          • 0 - нижний порог ночной температуры
 *          • 1 - верхний порог ночной температуры
 *          • 2 - нижний порог дневной температуры
 *          • 3 - верхний порог дневной температуры
 *          • 4 - нижний порог ощущения температуры
 *          • 5 - верхний порог ощущения температуры
 *          • 6 - нижний порог скорости ветра
 *          • 7 - верхний порог скорости ветра
 *          • 8 - нижний порог порывов ветра
 *          • 9 - верхний поров порывов ветра
 *          • 10 - давление
 *          Если в данных только одно число, то нижний порог = верхнему порогу = числу
 *
 *     - curDayStr[3][6][300] - массив с строковыми данными текущего дня. Первый индекс - категория, второй - слово (не больше 6),
 *       третий - максимальная техническая длина строки:
 *          • 0 - категория "осадки" (всегда одно слово. Предполагаю, что несколько видов осадков быть не может)
 *          • 1 - направление ветра
 *          • 2 - явления
 *
 *     - prevDayNums[11] - массив с числовыми данными ПРЕДЫДУЩЕГО дня
 *     - prevDayStr[11] - массив с числовыми данными ПРЕДЫДУЩЕГО дня
 *
 *      Возможно, бесполезные переменные:
 *          int curCountDirections; // Количество направлений ветра в день. Нужно для переопределения предыдущего дня
 *          int curCountScenes;     // Количество явлений в день. Нужно для переопределения предыдущего дня
 *          int prevCountDirections;
 *          int prevCountScenes;
 *          int countDays;  // Количество дней
 */

#include <stdio.h>
#include <windows.h>
#include "dataParser.h"

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    printf("Привет, мир!\n");

    FILE *data = fopen("Data Source/data.txt", "r");
    if (data == NULL) {
        printf("Error: file was not opened.");
        return 0;
    }

    char currentString[STRING_SIZE] = { 0 }; // Строка для чтения
    fgets(currentString, STRING_SIZE, data); // Shift a carriage to the next line

    while (!feof(data) && fgets(currentString, STRING_SIZE, data)) {
        dataParser(currentString);
    }


    // DEBUG INFORMATION (DON'T REMOVE PLEASE)
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


