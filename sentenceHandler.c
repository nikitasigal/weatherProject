#include "sentenceHandler.h"
#include "constantParser.h"
#include "dataParser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
// REMOVE!!!! TODO
//int randomTemplate = -1;    // Это сделано исключительно для тестирования. Позже уберём

/*
 * Функция для подсчёта рейтинга дня по 5 параметрам: температура, осадки, ветер, давления и явления.
 * Принимает один аргумент: категория рейтинга. Возвращает значение рейтинга в виде вещественного числа (double) по запросу. Пример: calcRate("Рейтинг дня"),
 * calcRate ("Температура"). Для вычисления использовались некоторые математические функции, которые подбирались вручную (Mathway в помощь).
 * Чем выше рейтинг, тем хуже показатель (rate = 1000+ это уже конец света).
 * Предполагается, что норма по параметру ниже 5 единиц (касается как каждого параметра, так и рейтинга дня).
 * Но пока ничего не корректировалось и не тестировалось.
 */

double calcRate(char* request) {
    double rate = 0;

    // Подсчёт рейтинга по температуре
    double diffTemp = ((curDayNums[2] + curDayNums[3] + curDayNums[1] + curDayNums[0]) / 4.0) - AverageTemperature[curDate.month - 1];//Отклонение среднедневной температуры от нормы
    double tempRate;
    if (diffTemp > 0)   // Если отклонение от нормы в положительную сторону, то это не так плохо, нежели в отрицательную
        tempRate = sqrt(fabs(diffTemp * diffTemp * diffTemp)) * 0.194;
    else
        tempRate = -sqrt(fabs(diffTemp * diffTemp * diffTemp)) * 0.31;

    if (strcmp(request, "Температура") == 0)    // Если запрос был по температуре, то возвращаем посчитанное значение
        return tempRate;

    // Подсчёт рейтинга по осадкам
    double precipitationRate = 0;
    char prec[STRING_SIZE];
    for (int i = 0; i < curDayStr[0].size; ++i) {
        strcpy(prec, curDayStr[0].word[0]);
        if (strcmp(prec, "снег") == 0)
            precipitationRate += 5;
        if (strcmp(prec, "дождь") == 0)
            precipitationRate += 5.5;
        if (strcmp(prec, "град") == 0)
            precipitationRate += 9.5;
        if (strcmp(prec, "кислотныйдождь") == 0)
            precipitationRate += 300;
    }

    if (strcmp(request, "Осадки") == 0)
        return precipitationRate;

    // Подсчёт рейтинга по ветру
    // WARNING!!! Don't use wind-rate for division into levels. Use WindScale
    double windRate = 0;
    double averageWind = (((curDayNums[6] + curDayNums[7]) / 2.0) + ((curDayNums[8] + curDayNums[9]) * 0.8 / 2.0)) / 2.5;
    windRate += sqrt((averageWind * averageWind * averageWind) / 10.0);

    if (strcmp(request, "Ветер") == 0)
        return windRate;

    // Подсчёт рейтинга по давлению
    double pressureRate = 0;
    double diffPressure = curDayNums[10] - 748;
    pressureRate += (sqrt(diffPressure * diffPressure * diffPressure) / 5.0);

    if (strcmp(request, "Давление") == 0)
        return pressureRate;

    // Подсчёт рейтинга по явлениям
    double scenesRate = 0;
    for (int i = 0; i < curDayStr[2].size; ++i) {
        char scene[STRING_SIZE];
        strcpy(scene, curDayStr[2].word[i]);
        if (strcmp(scene, "облачность") == 0)
            scenesRate += 2.3;
        if (strcmp(scene, "туман") == 0)
            scenesRate += 3.5;
        if (strcmp(scene, "гололедица") == 0)
            scenesRate += 4.5;
        if (strcmp(scene, "метель") == 0)
            scenesRate += 9.5;
        if (strcmp(scene, "гроза") == 0)
            scenesRate += 15;
        if (strcmp(scene, "конецсвета") == 0)
            scenesRate += 300;
    }

    if (strcmp(request, "Явления") == 0)
        return scenesRate;

    // Среднее арифметическое рейтингов. Рейтинг дня
    rate += (fabs(tempRate) + precipitationRate + windRate + fabs(pressureRate) + scenesRate) / 5;

    if (strcmp(request, "Рейтинг дня") == 0)
        return rate;

    // DEBUG INFORMATION
    printf("temp: %.2lf | prec: %.2lf | wind: %.2lf | pressure: %.2lf | scenes: %.2lf | rate: %.2lf\n", tempRate, precipitationRate,
           windRate, pressureRate, scenesRate, rate);

    return -9999;
}

/*
 * Следующая функция percent используется для обработки служебного символа % (например, %ABCD). Она используется в
 * функции генератора. Принимает два параметра: аргументы % (т.е. сами буквы ABCD) и триггер для капитализации слова
 * в начале предложения (анализируется в функции генератора).
 * Для первого параметра % (часть речи) обработка идёт следующим образом: создаётся указатель на словарь, который указывает на нужный
 * нам словарь в зависимости от части речи.
 * Потом по второму аргументу (номер группы) выбирается рандомный синоним из этой группы.
 * По третьему (род) и четвёртому (падеж) аргументу выбирается окончание в соответствии с правилом русского языка
 */
void percent(const char* request, int trigger) {    // Функция для %ABCD. Для удобства заносим параметры (ABCD) в отдельные переменные
    int partOfSpeech = request[0] - 'A';            // Часть речи
    int group = request[1] - 'A';                   // Группа
    int genus = request[2] - 'A';                   // Род
    int wordCase = request[3] - 'A';                // Падеж

    DICTIONARY dictionary;                          // Из какого словаря будем брать синонимы?
    switch(partOfSpeech) {
        case 0:
            dictionary = Adjectives;                // Словарь прилагательных
            break;
        case 1:
            dictionary = Adverbs;                   // Словарь наречий
            break;
        default:
            printf("Error");                // Отладки
            break;
    }

    // Choosing of random word
    int randomWord = rand() % dictionary.group[group].size;     // Берём случайное слово из словаря
    char base[STRING_SIZE];                                     // Переменная для этого слова
    strcpy(base, dictionary.group[group].syn[randomWord].base); // Запоминаем слово

    int category = 0;                                           // Дальше происходит обработка русского языка по правилам
    char last = base[strlen(base) - 1];                         // Берём последнюю букву основы (нужно для обработки)
    char first = dictionary.group[group].syn[randomWord].end[1];// Берём первую букву окончания (нужго для обработки)

    switch (last) {                                             // Если последняя буква основы...
        case -77: case -70: case -123:                          // ..г, к или х
            category = 4;
            break;
        case -74: case -120: case -121: case -119:              // ..ж, ш, ч или щ
            if (first == -72)                                   // ..если окончание начинается на и
                category = 2;
            else
                category = 3;
            break;
        default:
            if (first == -72)                                   // ..если окончание начинается на и
                category = 1;
            break;
    }
    if ((genus == 0) && (wordCase == 3))                        // Дальше творится правало русского языка. Это к Юле
        wordCase = 0;
    //Обработка начала предложения (капитализация буквы)
    if (trigger) {
        if (base[0] == -48) {
            base[1] = (char) (base[1] - 32);
        } else {
            base[0] -= 1;
            base[1] = (char) (base[1] + 32);
        }
    }
    printf("%s", base);
    if (genus == 0 && wordCase == 0)
        printf("%s", dictionary.group[group].syn[randomWord].end);
    else
        printf("%s", Endings[category * 4 + genus][wordCase]);

}

/*
 * Функция генератора. Принимает два параметра: категория (Температура, Ветер и т.п.) и уровень (0, 1, 2..).
 * Пример вызова функции: generator ("Температура", 0); Сначала берёт рандомный темплейт и обрабатывает его посимвольно.
 * Если текущий символ просто буква, число или знак препинания, то печатаем. Если встретили один из служебных символов (%, $, *),
 * то обрабатываем его (подробнее об обработке см. в функции).
 */

void generator(char* ctg, int level) {
    int randomTemplate = rand() % Temperature.group[level].size;    // Random selection of template
    //randomTemplate = (randomTemplate + 1) % Temperature.group[level].size;  // DEBUG
    printf("%d) ", randomTemplate + 1);                             // DEBUG
    int lastNum = 0;    // Last digit of previous number. Required to '*'

    CATEGORY parameter; // Выбор массива по выбранному параметру (температура, ветер, давление и т.п.)
    if (strcmp(ctg, "Температура") == 0)
        parameter = Temperature;
    char *curTemplate = parameter.group[level].tmp[randomTemplate]; // Рандомный выбор темплейта из нужной группы
    //char *curTemplate = "Достаточно %AAAA день - $AD градус*A днем, $AF по ощущениям. $BA.\n";
    for (int i = 0; i < strlen(curTemplate); ++i) { // Идём по строке. Анализируем посимвольно
        switch (curTemplate[i]) {                   // Если текущий символ темплейта...
            case '$': {                             // ..служебный символ для подстановки числа/слова в текст (формата $AB)
                ++i;                                // Смотрим первый символ в записи $
                switch (curTemplate[i]) {           // Если этот первый символ...
                    case 'A': {                     // ..'A', то нужно вставить число из массива curDayNums (массив с данными дня)
                        ++i;                        // Смотрим следующий символ в записи $. Он означает, какое число из массива нужно взять
                        lastNum = curDayNums[curTemplate[i] - 'A']; // Берём число из массива
                        printf("%d", lastNum);  // Выводим то, что просили
                        lastNum %= 100;                 // Запоминаем последние две цифры числа. Это нужно для склонения последующего слова (например, 11 градусОВ, 2 градусА)
                        lastNum = abs(lastNum);         // Нам не нужно запоминать знак числа. Берём по модулю для простоты
                        break;                          // $ обработан
                    }
                    case 'B': {                     // ..'B', то нужно вставить слово из массива curDayStr (массив с данными дня)
                        // TODO
                        ++i;
                        // Капитализация первой буквы
                        for (int j = 0; j < curDayStr[curTemplate[i] - 'A'].size; ++j) {
                            char* first = curDayStr[curTemplate[i] - 'A'].word[j];
                            if (j == 0)
                                if (i == 0 || curTemplate[i - 4] == '.') {
                                    if (first[0] == -48) {
                                        first[1] = (char) (first[1] - 32);
                                    } else {
                                        first[0] -= 1;
                                        first[1] = (char) (first[1] + 32);
                                    }
                                }

                            if (j == curDayStr[curTemplate[i] - 'A'].size - 1)
                                printf("%s", curDayStr[curTemplate[i] - 'A'].word[j]);
                            else
                                printf("%s, ", curDayStr[curTemplate[i] - 'A'].word[j]);
                        }
                        break;
                    }
                    default:
                        printf("Error");    // Отладка
                }

                break;
            }
            case '%': {                                     // ..служебный символ для подстановки абстрактного слова (формат %ABCD)
                int trigger = 0;                            // Триггер для капитализации буквы в начале предложения
                if (i == 0 || curTemplate[i - 2] == '.')    // Если это начало предложения, то пишем его с большой буквы (ставим 'триггер' для этого)
                    trigger = 1;                            // trigger = 1, значит надо капитализировать

                ++i;                                        // Переходим к параметрам %
                char request[4];                            // Массив для хранения четырёх параметров %
                for (int j = 0; j < 4; ++j) {               // Заносим в массив
                    request[j] = curTemplate[i + j];
                }

                i += 3;                                     // Данные запомнили, теперь
                // Предобработка %.... $..
                if (curTemplate[i + 2] == '$' && curTemplate[i + 3] == 'A' &&   // Длинное условие для обработки случая: жаркие 5 градусов / жаркий 1 градус
                    abs(curDayNums[curTemplate[i + 4] - 'A']) != 1) {       // Условие: если дальше идёт $ и в нём запрашивается число, отличное от 1,..
                                                                                // ..то выдаём абстрактное слово во множественном числе
                    request[2] = 'D';
                }

                percent(request, trigger);                   // Вызываем запрос абстрактного слова. Функция нужна для удобства, её часто придётся изменять

                break;
            }
            case '*': {                                 // ..служебный символ для подстановки абстрактного слова (формат *A)
                int category = 0;                       // Категория слова в соотвествии с правилом русского языка
                ++i;                                    // Смотрим, какого падежа требуется слово
                // Особый случай для чисел с окончанием на 11-14
                if (lastNum >= 11 && lastNum <= 14) {   // Если число оканчивается на 11-14, то окончание -ов (11 градусов)
                    printf("ов");
                }
                else {
                    switch (lastNum % 10) {             // В зависимости от последней цифры меняется окончание следующего слова
                        case 1:                         // Нулевое окончание (1 градус)
                            break;
                        case 2:
                        case 3:
                        case 4:
                            category = 1;               // Группа окончаний для цифр 2, 3, 4
                            break;
                        default:
                            category = 2;               // В любом другом случае
                            break;
                    }
                    int caseWord = curTemplate[i] - 'A';                                        // Получение номера падежа
                    if (!(category == 0 && (caseWord == 0 || caseWord == 3)))                   // Нуууу, выбор окончания..
                        printf("%s", EndingsMeasures[category][curTemplate[i] - 'A']);  // Берём окончание из словаря
                }

                break;
            }
            default:                                    // Не служебный символ. Выводим
                printf("%c", curTemplate[i]);
                break;
        }
    }
}