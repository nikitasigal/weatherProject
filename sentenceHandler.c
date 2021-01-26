#include "sentenceHandler.h"
#include "constantParser.h"
#include "dataParser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

double calcRate(char* request) {
    double rate = 0;

    double diffTemp = ((curDayNums[2] + curDayNums[3] + curDayNums[1] + curDayNums[0]) / 4.0) - AverageTemperature[curDate.month - 1];
    double tempRate = 0;
    if (diffTemp > 0)
        tempRate = sqrt(fabs(diffTemp * diffTemp * diffTemp)) * 0.15;
    else
        tempRate = -sqrt(fabs(diffTemp * diffTemp * diffTemp)) * 0.3;

    double precipitationRate = 0;
    char prec[STRING_SIZE];
    strcpy(prec, curDayStr[0][0]);
    if (strcmp(prec, "снег") == 0)
        precipitationRate += 5;
    if (strcmp(prec, "дождь") == 0)
        precipitationRate += 4.5;
    if (strcmp(prec, "град") == 0)
        precipitationRate += 6.5;
    if (strcmp(prec, "кислотныйдождь") == 0)
        precipitationRate += 300;

    double windRate = 0;
    double averageWind = (((curDayNums[6] + curDayNums[7]) / 2.0) + ((curDayNums[8] + curDayNums[9]) * 0.8 / 2.0)) / 2.5;
    windRate += sqrt((averageWind * averageWind * averageWind) / 10.0);

    double pressureRate = 0;
    double diffPressure = abs(curDayNums[10] - 748);
    pressureRate += (sqrt(diffPressure * diffPressure * diffPressure) / 4.3);

    double scenesRate = 0;
    for (int i = 0; i < curCountScenes; ++i) {
        char scene[STRING_SIZE];
        strcpy(scene, curDayStr[2][i]);
        if (strcmp(scene, "облачность") == 0)
            scenesRate += 1.1;
        if (strcmp(scene, "туман") == 0)
            scenesRate += 2.0;
        if (strcmp(scene, "гололедица") == 0)
            scenesRate += 2.6;
        if (strcmp(scene, "метель") == 0)
            scenesRate += 4.5;
        if (strcmp(scene, "гроза") == 0)
            scenesRate += 10;
        if (strcmp(scene, "конецсвета") == 0)
            scenesRate += 300;
    }

    rate += (fabs(tempRate) + precipitationRate + windRate + pressureRate + scenesRate) / 5;

    if (strcmp(request, "Температура") == 0)
        return tempRate;
    if (strcmp(request, "Осадки") == 0)
        return precipitationRate;
    if (strcmp(request, "Ветер") == 0)
        return windRate;
    if (strcmp(request, "Давление") == 0)
        return pressureRate;
    if (strcmp(request, "Явления") == 0)
        return scenesRate;
    if (strcmp(request, "Рейтинг дня") == 0)
        return rate;

    // DEBUG INFORMATION
    //printf("temp: %.2lf | prec: %.2lf | wind: %.2lf | pressure: %.2lf | scenes: %.2lf | rate: %.2lf\n", tempRate, precipitationRate,
    //       windRate, pressureRate, scenesRate, rate);

    return rate;
}

void percent(const char* request, int trigger) {
    int partOfSpeech = request[0] - 'A';
    int group = request[1] - 'A';
    int genus = request[2] - 'A';
    int wordCase = request[3] - 'A';

    DICTIONARY word;
    switch(partOfSpeech) {
        case 0:
            word = Adjectives;
            break;
        case 1:
            word = Adverbs;
            break;
        default:
            break;
    }

    // Choosing of random word
    int random = rand() % word.group[group].size;
    char base[STRING_SIZE];
    strcpy(base, word.group[group].syn[random].base);

    int category = 0;
    char last = base[strlen(base) - 1];
    char first = word.group[group].syn[random].end[1];
    switch (last) {
        case -77: case -70: case -123:
            category = 4;
            break;
        case -74: case -120: case -121: case -119:
            if (first == -72)
                category = 2;
            else
                category = 3;
            break;
        default:
            if (first == -72)
                category = 1;
            break;
    }
    if ((genus == 0) && (wordCase == 3))
        wordCase = 0;
    //Обработка начала предложения (капитализация буквы)
    if (trigger)
        base[1] = (char)(base[1] - 32);
    printf("%s", base);
    if (genus == 0 && wordCase == 0)
        printf("%s", word.group[group].syn[random].end);
    else
        printf("%s", Endings[category * 4 + genus][wordCase]);

}

void generator(char* cat, int level) {
    int randomTemplate = rand() % Temperature.group[level].size;    // Random selection of template
    int lastNum = 0;    // Last digit of previous number. Required to '*'
    CATEGORY parameter;
    if (strcmp(cat, "Температура") == 0)
        parameter = Temperature;
    char *curTemplate = parameter.group[0].tmp[randomTemplate];
    //char *curTemplate = "Никаких аномальных %ABDB или %ADDB температур - напротив, %AEBA около $AD градус*B.";
    for (int i = 0; i < strlen(curTemplate); ++i) {
        switch (curTemplate[i]) {
            case '$':
                if (curTemplate[i] == '$') {
                    ++i;
                    switch (curTemplate[i]) {
                        case 'A':
                            ++i;
                            lastNum  = curDayNums[curTemplate[i] - 'A'];
                            printf("%d", lastNum);
                            lastNum %= 10;
                            lastNum = abs(lastNum);
                            break;
                        case 'B':
                            // TODO
                            break;
                        default:
                            printf("Error");
                    }
                }
                break;
            case '%':
                if (curTemplate[i] == '%') {
                    // Триггер для капитализации буквы в начале предложения
                    int trigger = 0;
                    if (i == 0 || curTemplate[i - 2] == '.')
                        trigger = 1;

                    ++i;
                    char request[4];
                    for (int j = 0; j < 4; ++j) {
                        request[j] = curTemplate[i + j];
                    }

                    i += 3;
                    // Предобработка %.... $..
                    if (curTemplate[i + 2] == '$' && curTemplate[i + 3] == 'A' && abs(curDayNums[curTemplate[i + 4] - 'A']) != 1) {
                        request[2] = 'D';
                    }

                    percent(request, trigger);
                }
                break;
            case '*':
                if (curTemplate[i] == '*') {
                    int category = 0;
                    ++i;
                    switch (lastNum) {
                        case 1:
                            break;
                        case 2:
                        case 3:
                        case 4:
                            category = 1;
                            break;
                        default:
                            category = 2;
                            break;
                    }
                    int caseWord = curTemplate[i] - 'A';
                    if (!(category == 0 && (caseWord == 0 || caseWord == 3)))
                        printf("%s", EndingsMeasures[category][curTemplate[i] - 'A']);
                }
                break;
            default:
                printf("%c", curTemplate[i]);
                break;
        }
    }
}