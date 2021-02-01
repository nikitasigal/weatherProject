#include <string.h>
#include <stdio.h>
#include <math.h>

#include "evaluateLevels.h"
#include "dataParser.h"
#include "constantParser.h"

char precipitationsAndEvents[11][STRING_SIZE] = {"дождь", "снег", "кислотный.дождь", "град", "туман", "метель", "гололедица",
                                                 "гроза", "конец.света", "никитаgay", "слякоть"};

/*
 * Функция для подсчёта рейтинга дня по 5 параметрам: температура, осадки, ветер, давления и явления.
 * Принимает один аргумент: категория рейтинга. Возвращает значение рейтинга в виде вещественного числа (double) по запросу. Пример: calcRate("Рейтинг дня"),
 * calcRating ("Температура"). Для вычисления использовались некоторые математические функции, которые подбирались вручную (Mathway в помощь).
 * Чем выше рейтинг, тем хуже показатель (rate = 1000+ это уже конец света).
 * Предполагается, что норма по параметру ниже 5 единиц (касается как каждого параметра, так и рейтинга дня).
 * Но пока ничего не корректировалось и не тестировалось.
 */
double calcRating(int ctg) {
    double rate = 0;

    // Подсчёт рейтинга по температуре
    double diffTemp = ((curDayNums[2] + curDayNums[3] + curDayNums[4] + curDayNums[5]) / 4.0) - StatTemperature[curDate.month - 1];//Отклонение среднедневной температуры от нормы
    double tempRate;

    switch (curDate.month) {
        case 10:
        case 11:
        case 12:
        case 1:
        case 2:
        case 3:
        case 4:
            tempRate = sqrt(fabs(diffTemp * diffTemp * diffTemp)) * 0.25;
            if (diffTemp > 0) {
                tempRate *= 0.63;
            }
            break;
        default:
            tempRate = sqrt(fabs(diffTemp * diffTemp * diffTemp)) * 0.45;
    }

    if (ctg == 0)    // Если запрос был по температуре, то возвращаем посчитанное значение
        return tempRate;

    // Подсчёт рейтинга по осадкам
    double precipitationRate = 0;
    char precipitation[STRING_SIZE];
    for (int i = 0; i < curDayStr[0].size; ++i) {
        strcpy(precipitation, curDayStr[0].word[0]);
        if (strcmp(precipitation, "снег") == 0)
            precipitationRate += 5;
        if (strcmp(precipitation, "дождь") == 0)
            precipitationRate += 8.5;
        if (strcmp(precipitation, "град") == 0)
            precipitationRate += 10;
        if (strcmp(precipitation, "кислотный.дождь") == 0)
            precipitationRate += 300;
    }

    if (ctg == 1)
        return precipitationRate;

    // Подсчёт рейтинга по ветру
    // WARNING!!! Don't use wind-rate for division into levels. Use StatWindScale
    double windRate = 0;
    double averageWind = ((curDayNums[6] + curDayNums[7]) + ((curDayNums[8] + curDayNums[9]) * 0.6)) / 4.0;
    windRate += sqrt((averageWind * averageWind * averageWind) / 10.0);

    if (ctg == 2)
        return windRate;

    // Подсчёт рейтинга по давлению
    double pressureRate = 0;
    double diffPressure = curDayNums[10] - 748;
    pressureRate += (sqrt((fabs)(diffPressure * diffPressure * diffPressure)) / 14);

    if (ctg == 3)
        return pressureRate;

    // Подсчёт рейтинга по явлениям
    double scenesRate = 0;
    for (int i = 0; i < curDayStr[2].size; ++i) {
        char scene[STRING_SIZE];
        strcpy(scene, curDayStr[2].word[i]);
        if (strcmp(scene, "слякоть") == 0)
            scenesRate += 4;
        if (strcmp(scene, "туман") == 0)
            scenesRate += 8.5;
        if (strcmp(scene, "гололедица") == 0)
            scenesRate += 5.5;
        if (strcmp(scene, "метель") == 0)
            scenesRate += 12;
        if (strcmp(scene, "гроза") == 0)
            scenesRate += 25;
        if (strcmp(scene, "конец.света") == 0)
            scenesRate += 300;
    }

    if (ctg == 4)
        return scenesRate;

    // Среднее арифметическое рейтингов. Рейтинг дня
    rate += (fabs(tempRate) + precipitationRate + windRate + fabs(pressureRate) + scenesRate) / 5.0;

    if (ctg == 5)
        return rate;

    // DEBUG INFORMATION
    printf("temp: %.2lf | prec: %.2lf | wind: %.2lf | pressure: %.2lf | scenes: %.2lf | rate: %.2lf\n", tempRate, precipitationRate,
           windRate, pressureRate, scenesRate, rate);

    return -9999;
}

void sortCategories() {
    for (int i = 0; i < 5; ++i) {
        Order[i].ctg = i;
        Order[i].rating = calcRating(i);
    }

    for (int i = 0; i < 5; ++i) {
        for (int j = i + 1; j < 5; ++j) {
            if (Order[i].rating < Order[j].rating) {
                double temp = Order[i].ctg;
                Order[i].ctg = Order[j].ctg;
                Order[j].ctg = (int) temp;

                temp = Order[i].rating;
                Order[i].rating = Order[j].rating;
                Order[j].rating = temp;
            }
        }
    }
}

int getTemperatureLevel() {
    double averageTemperature = ((curDayNums[2] + curDayNums[3] + curDayNums[4] + curDayNums[5]) / 4.0) - StatTemperature[curDate.month - 1];
    if (curDate.month >= 4 && curDate.month <= 9) {
        if (averageTemperature < -5) {
            return 2;
        } else {
            if (averageTemperature > 5) {
                return 0;
            } else {
                return 1;
            }
        }
    } else {
        if (averageTemperature < -7) {
            return 5;
        } else {
            if (averageTemperature > 7) {
                return 3;
            } else {
                return 4;
            }
        }
    }
}

int getWindLevel() {
    double averageWind = ((curDayNums[6] + curDayNums[7]) + ((curDayNums[8] + curDayNums[9]) * 0.6)) / 4.0;
    for (int i = 1; i < 4; ++i) {
        if (averageWind < StatWindScale[i]) {
            return 4 - i;
        }
    }
    return 0;
}

int getPressureLevel() {
    int pressureDifference = curDayNums[10] - StatPressure[curDate.month];
    if (pressureDifference < -12) {
        return 2;
    } else {
        if (pressureDifference > 12) {
            return 0;
        } else {
            return 1;
        }
    }
}

int getPrecipitationOrEventGroup(const char *request) {
    for (int i = 0; i < 11; ++i) {
        if (strcmp(precipitationsAndEvents[i], request) == 0) {
            return i;
        }
    }
    return 0;
}

int getDayLevel() {
    double currentDayRating = calcRating(5);
    if (currentDayRating < 2) {
        return 0;
    } else {
        if (currentDayRating < 4) {
            return 1;
        } else {
            if (currentDayRating < 6) {
                return 2;
            } else {
                return 3;
            }
        }
    }
}