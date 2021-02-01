#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "sentenceHandler.h"
#include "dataParser.h"
#include "evaluateLevels.h"


/*
 * Следующая функция calcPercent используется для обработки служебного символа % (например, %ABCD). Она используется в
 * функции генератора. Принимает два параметра: аргументы % (т.е. сами буквы ABCD) и триггер для капитализации слова
 * в начале предложения (анализируется в функции генератора).
 * Для первого параметра % (часть речи) обработка идёт следующим образом: создаётся указатель на словарь, который указывает на нужный
 * нам словарь в зависимости от части речи.
 * Потом по второму аргументу (номер группы) выбирается рандомный синоним из этой группы.
 * По третьему (род) и четвёртому (падеж) аргументу выбирается окончание в соответствии с правилом русского языка
 */
void calcPercent(FILE *outputFile, const char *curTemplate, int *i) {    // Функция для %ABCD. Для удобства заносим параметры (ABCD) в отдельные переменные
    int trigger = 0;                            // Триггер для капитализации буквы в начале предложения
    if (*i == 0 || curTemplate[*i - 2] == '.')    // Если это начало предложения, то пишем его с большой буквы (ставим 'триггер' для этого)
        trigger = 1;                            // trigger = 1, значит надо капитализировать

    ++*i;                                        // Переходим к параметрам %
    char request[4];                            // Массив для хранения четырёх параметров %
    for (int j = 0; j < 4; ++j)                 // Заносим в массив
        request[j] = curTemplate[*i + j];

    *i += 3;
    // Предобработка %.... $..
    if (request[0] == 'A' && curTemplate[*i + 2] == '$' && curTemplate[*i + 3] == 'A' &&   // Длинное условие для обработки случая: жаркие 5 градусов / жаркий 1 градус
        abs(curDayNums[curTemplate[*i + 4] - 'A']) != 1) {       // Условие: если дальше идёт $ и в нём запрашивается число, отличное от 1,..
        // ..то выдаём абстрактное слово во множественном числе
        request[2] = 'D';
    }

    int partOfSpeech = request[0] - 'A';                // Часть речи
    int groupID = request[1] - 'A';                     // Группа
    int wordGenus = request[2] - 'A';                   // Род
    int wordCase = request[3] - 'A';                    // Падеж

    //выбор словаря по нужной части речи
    DICTIONARY dictionary;
    switch (partOfSpeech) {
        case 0:
            dictionary = Adjectives;
            break;
        case 1:
            dictionary = Adverbs;
            break;
        default:
            fprintf(outputFile, "Error");        //DEBUG
            break;
    }

    //выбор случайного слова из группы
    int randID = rand() % dictionary.group[groupID].size;
    char wordBase[STRING_SIZE];
    strcpy(wordBase, dictionary.group[groupID].syn[randID].base);

    //капитализция первой буквы при необходимости
    if (trigger) {
        if (wordBase[0] == -48) {
            wordBase[1] = (char) (wordBase[1] - 32);
        } else {
            wordBase[0] -= 1;
            wordBase[1] = (char) (wordBase[1] + 32);
        }
    }

    //выбор категории окончаний в случае, когда необходимо просклонять прилагательное
    int endingCategory = 0;
    if (partOfSpeech == 0) {
        char lastBaseLetter = wordBase[strlen(wordBase) - 1];                   // Берём последнюю букву основы
        char firstEndLetter = dictionary.group[groupID].syn[randID].end[1];     // Берём первую букву окончания

        switch (lastBaseLetter) {                   // Если последняя буква основы...
            case -77:
            case -70:
            case -123:                              // ..г, к или х
                endingCategory = 4;
                break;
            case -74:
            case -120:
            case -121:
            case -119:                              // ..ж, ш, ч или щ
                if (firstEndLetter == -72)          // ..если окончание начинается на и
                    endingCategory = 2;
                else
                    endingCategory = 3;
                break;
            default:
                if (firstEndLetter == -72)          // ..если окончание начинается на и
                    endingCategory = 1;
                break;
        }

        if ((wordGenus == 0) && (wordCase == 3))    //Мужской род и Винительный падеж использует окончание Именительного падежа из словаря
            wordCase = 0;
    }

    //вывод получившегося слова
    fprintf(outputFile, "%s", wordBase);
    if (wordGenus == 0 && wordCase == 0)
        fprintf(outputFile, "%s", dictionary.group[groupID].syn[randID].end);    //в этом случае выводится окончание из словаря (пустое для сущ. и нар.)
    else
        fprintf(outputFile, "%s", AdjEndings[endingCategory * 4 + wordGenus][wordCase]);
}

/*
 * Обработчик служебного символа $
 */
void calcDollar(FILE *outputFile, const char *curTemplate, int *lastNum, int *i) {
    ++(*i);                                                  // Смотрим первый символ в записи $
    switch (curTemplate[(*i)]) {                             // Если этот первый символ...
        case 'A': {                                          // ..'A', то нужно вставить число из массива curDayNums (массив с данными дня)
            ++(*i);                                          // Смотрим следующий символ в записи $. Он означает, какое число из массива нужно взять
            (*lastNum) = curDayNums[curTemplate[(*i)] - 'A']; // Берём число из массива
            fprintf(outputFile, "%d", (*lastNum));                // Выводим то, что просили
            (*lastNum) %= 100;                             // Запоминаем последние две цифры числа. Это нужно для склонения последующего слова (например, 11 градусОВ, 2 градусА)
            (*lastNum) = abs((*lastNum));                     // Нам не нужно запоминать знак числа. Берём по модулю для простоты
            break;                                      // $ обработан
        }
        case 'B': {                     // ..'B', то нужно вставить слово из массива curDayStr (массив с данными дня)
            ++(*i);
            // Капитализация первой буквы
            for (int j = 0; j < curDayStr[curTemplate[(*i)] - 'A'].size; ++j) {
                char *first = curDayStr[curTemplate[(*i)] - 'A'].word[j];
                if (j == 0)
                    if ((*i) == 0 || curTemplate[(*i) - 4] == '.') {
                        if (first[0] == -48) {
                            first[1] = (char) (first[1] - 32);
                        } else {
                            first[0] -= 1;
                            first[1] = (char) (first[1] + 32);
                        }
                    }

                if (j == curDayStr[curTemplate[(*i)] - 'A'].size - 1)
                    fprintf(outputFile, "%s", curDayStr[curTemplate[(*i)] - 'A'].word[j]);
                else
                    fprintf(outputFile, "%s, ", curDayStr[curTemplate[(*i)] - 'A'].word[j]);
            }
            break;
        }
        default:
            fprintf(outputFile, "Error");    // Отладка
    }
}

void calcAsterisk(FILE *outputFile, int lastNum, const char *curTemplate, int *i) {
    int category = 0;                       // Категория слова в соотвествии с правилом русского языка
    ++(*i);                                    // Смотрим, какого падежа требуется слово
    // Особый случай для чисел с окончанием на 11-14
    if (lastNum >= 11 && lastNum <= 14) {   // Если число оканчивается на 11-14, то окончание -ов (11 градусов)
        fprintf(outputFile, "ов");
    } else {
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
        int caseWord = curTemplate[*i] - 'A';                                        // Получение номера падежа
        if (!(category == 0 && (caseWord == 0 || caseWord == 3)))                   // Нуууу, выбор окончания..
            fprintf(outputFile, "%s", NounEndings[category][curTemplate[*i] - 'A']);  // Берём окончание из словаря
    }
}

/*
 * Функция генератора темплейтов. Принимает один аргумент - категорию. Сначала берёт рандомный темплейт и обрабатывает его посимвольно.
 * Если текущий символ просто буква, число или знак препинания, то печатаем. Если встретили один из служебных символов (%, $, *),
 * то обрабатываем его (подробнее об обработке см. в функции).
 */
void generateSimple(FILE *outputFile, int ctg) {
    int level;
    TEMP_CATEGORY parameter;
    switch (ctg) {
        case 0:
            level = getTemperatureLevel();
            parameter = Temperature;
            break;
        case 2:
            level = getWindLevel();
            parameter = Wind;
            break;
        case 3:
            level = getPressureLevel();
            parameter = Pressure;
            break;
        case 5:
            level = getDayLevel();
            parameter = BeginSentence;
            break;
        default:
            printf("Eror in generateSimple - wrong argument");
            return;
    }

    int randomTemplate = rand() % parameter.group[level].size;    // Random selection of template
    int lastNum = 0;    // Last inserted numerical value. Used in *function to connect nouns with their respected values


    char *curTemplate = parameter.group[level].tmp[randomTemplate]; // Рандомный выбор темплейта из нужной группы
    for (int i = 0; i < strlen(curTemplate); ++i) {                 // Идём по строке. Анализируем посимвольно
        switch (curTemplate[i]) {                                   // Если текущий символ темплейта...
            case '$': {                                             // ..служебный символ для подстановки числа/слова в текст (формата $AB)
                calcDollar(outputFile, curTemplate, &lastNum, &i);
                break;
            }
            case '%': {                                     // ..служебный символ для подстановки абстрактного слова (формат %ABCD)
                calcPercent(outputFile, curTemplate, &i);
                break;
            }
            case '*': {                                 // ..служебный символ для подстановки абстрактного слова (формат *A)
                calcAsterisk(outputFile, lastNum, curTemplate, &i);
                break;
            }
            default:                                    // Не служебный символ. Выводим
                fprintf(outputFile, "%c", curTemplate[i]);
                break;
        }
    }
    fprintf(outputFile, " ");
}

/*
 * Функция генерации текста по частям. Принимает один аргумент - категорию.
 */
void generateComplex(FILE *outputFile, int ctg) {
    int strID;
    if (ctg == 1)
        strID = 0;
    else if (ctg == 4)
        strID = 2;
    else {
        printf("Eror in generateComplex - wrong argument");
        return;
    }

    for (int i = 0; i < curDayStr[strID].size; ++i) {
        int beginningGroup = rand() % TextBeginnings.size;
        int beginningIndex = rand() % TextBeginnings.group[beginningGroup].size;
        fprintf(outputFile, "%s", TextBeginnings.group[beginningGroup].tmp[beginningIndex]);

        int followupIndex = rand() % TextFollowups.group[beginningGroup].size;
        fprintf(outputFile, "%s ", TextFollowups.group[beginningGroup].tmp[followupIndex]);

        int eventIndex = getPrecipitationOrEventGroup(curDayStr[strID].word[i]);
        for (int j = 0; j < strlen(curDayStr[strID].word[i]); ++j) {
            if (curDayStr[strID].word[i][j] != '.')
                fprintf(outputFile, "%c", curDayStr[strID].word[i][j]);
            else
                fprintf(outputFile, " ");
        }

        int endIndex = rand() % Events.group[eventIndex].size;
        fprintf(outputFile, "%s ", Events.group[eventIndex].tmp[endIndex]);
    }
}