#include <string.h>
#include <math.h>

#include "evaluateLevels.h"

/*
 * Функция для подсчёта рейтинга дня по 5 параметрам: температура, осадки, ветер, давления и явления.
 * Принимает один аргумент: категория рейтинга. Возвращает значение рейтинга в виде вещественного числа (double) по запросу. Пример: calcRate("Рейтинг дня"),
 * calcRating ("Температура"). Для вычисления использовались некоторые математические функции, которые подбирались вручную (Mathway в помощь).
 * Чем выше рейтинг, тем хуже показатель (rate = 1000+ это уже конец света).
 * Предполагается, что норма по параметру ниже 5 единиц (касается как каждого параметра, так и рейтинга дня).
 * Но пока ничего не корректировалось и не тестировалось.
 */
double calcRating(int ctg, Data *data) {
    double rate = 0;

    // Подсчёт рейтинга по температуре

    //Отклонение среднедневной температуры от нормы
    double diffTemp = ((data->curDayNums[2] + data->curDayNums[3] + data->curDayNums[4] + data->curDayNums[5]) / 4.0) - data->StatTemperature[data->curDate.month - 1];
    double tempRate;

    switch (data->curDate.month) {
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

    if (ctg == TEMPERATURE)    // Если запрос был по температуре, то возвращаем посчитанное значение
        return tempRate;

    // Подсчёт рейтинга по осадкам
    double precipitationRate = 0;
    char precipitation[STRING_SIZE];
    for (int i = 0; i < data->curDayStr[0].size; ++i) {
        strcpy(precipitation, data->curDayStr[0].word[0]);
        if (strcmp(precipitation, "снег") == 0)
            precipitationRate += 4;
        if (strcmp(precipitation, "дождь") == 0)
            precipitationRate += 8;
        if (strcmp(precipitation, "град") == 0)
            precipitationRate += 15;
        if (strcmp(precipitation, "кислотный.дождь") == 0)
            precipitationRate += 300;
    }

    if (ctg == PRECIPITATION)
        return precipitationRate;

    // Подсчёт рейтинга по ветру
    // WARNING!!! Don't use wind-rate for division into levels. Use StatWindScale
    double windRate = 0;
    double averageWind = ((data->curDayNums[6] + data->curDayNums[7]) + ((data->curDayNums[8] + data->curDayNums[9]) * 0.6)) / 4.0;
    windRate += sqrt((averageWind * averageWind * averageWind) / 10.0);

    if (ctg == WIND)
        return windRate;

    // Подсчёт рейтинга по давлению
    double pressureRate = 0;
    double diffPressure = data->curDayNums[10] - data->StatPressure[data->curDate.month];
    pressureRate += sqrt(fabs(diffPressure * diffPressure * diffPressure)) / 14;

    if (ctg == PRESSURE)
        return pressureRate;

    // Подсчёт рейтинга по явлениям
    double scenesRate = 0;
    for (int i = 0; i < data->curDayStr[2].size; ++i) {
        char scene[STRING_SIZE];
        strcpy(scene, data->curDayStr[2].word[i]);
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

    if (ctg == EVENT)
        return scenesRate;

    // Среднее арифметическое рейтингов. Рейтинг дня
    rate += (fabs(tempRate) + precipitationRate + windRate + fabs(pressureRate) + scenesRate) / 5.0;

    if (ctg == RATING)
        return rate;

    return -9999;
}

void sortCategories(Data *data) {
    for (int i = 0; i < CATEGORIES_COUNT; ++i) {
        data->order[i].ctg = i;
        data->order[i].rating = calcRating(i, data);
    }

    for (int i = 0; i < CATEGORIES_COUNT; ++i) {
        for (int j = i + 1; j < CATEGORIES_COUNT; ++j) {
            if (data->order[i].rating < data->order[j].rating) {
                double temp = data->order[i].ctg;
                data->order[i].ctg = data->order[j].ctg;
                data->order[j].ctg = (int) temp;

                temp = data->order[i].rating;
                data->order[i].rating = data->order[j].rating;
                data->order[j].rating = temp;
            }
        }
    }
}

int getTemperatureLevel(Data *data) {
    double averageTemperature = ((data->curDayNums[2] + data->curDayNums[3] + data->curDayNums[4] + data->curDayNums[5]) / 4.0) - data->StatTemperature[data->curDate.month - 1];
    if (data->curDate.month >= 4 && data->curDate.month <= 9) {
        if (averageTemperature < -5) {
            return 2;
        } else if (averageTemperature > 5) {
            return 0;
        } else {
            return 1;
        }
    } else {
        if (averageTemperature < -7) {
            return 5;
        } else if (averageTemperature > 7) {
            return 3;
        } else {
            return 4;
        }

    }
}

int getWindLevel(Data *data) {
    double averageWind = ((data->curDayNums[6] + data->curDayNums[7]) + ((data->curDayNums[8] + data->curDayNums[9]) * 0.6)) / 4.0;
    for (int i = 1; i < 4; ++i) {
        if (averageWind < data->StatWindScale[i]) {
            return 4 - i;
        }
    }
    return 0;
}

int getPressureLevel(Data *data) {
    int pressureDifference = data->curDayNums[10] - data->StatPressure[data->curDate.month];
    if (pressureDifference < -12) {
        return 2;
    } else if (pressureDifference > 12) {
        return 0;
    } else {
        return 1;
    }
}

int getPrecipitationOrEventGroup(const char *request) {
    char precipitationsAndEvents[PRECIPITATION_COUNT][STRING_SIZE] = {"дождь", "снег", "кислотный.дождь", "град",
                                                                      "туман", "метель", "гололедица", "гроза",
                                                                      "конец.света", "никитаgay", "слякоть"};
    for (int i = 0; i < PRECIPITATION_COUNT; ++i) {
        if (strcmp(precipitationsAndEvents[i], request) == 0) {
            return i;
        }
    }
    return 0;
}

int getDayLevel(Data *data) {
    double currentDayRating = calcRating(RATING, data);
    if (currentDayRating < 2) {
        return 0;
    } else if (currentDayRating < 4) {
        return 1;
    } else if (currentDayRating < 6) {
        return 2;
    } else if (currentDayRating < 400) {
        return 3;
    } else {
        return 4;
    }
}