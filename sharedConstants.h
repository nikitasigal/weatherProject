/*
 * %%%%%%%%%%
 * Часть речи:
 *  A - прилагательное
 *  B - наречие
 *  C - существительное
 *
 * Группа:
 *  номер группы в файле
 *
 * Род:
 *  A - мужской
 *  B - женский
 *  C - средний
 *  D - множественное число
 *
 * Падежи:
 *  A - Иван
 *  B - Родил
 *  C - Девчонку
 *  D - Велел
 *  E - Тащить
 *  F - Пелёнку
 *  %%%%%%%%%%%%
 *
 *  $$$$$$$$$$$$
 *  1-ый индекс:
 *    A - массив чисел
 *    B - массив слов
 *  2-ой индекс:
 *    Индексы элементов массивов (из main.c)
 *  $$$$$$$$$$$$
 *
 *  ************
 *  Один индекс - падеж
 *  ************
 */

#ifndef WEATHERPROJECT_SHAREDCONSTANTS_H
#define WEATHERPROJECT_SHAREDCONSTANTS_H

//string and word sizes for char arrays
#define WORD_SIZE 40
#define STRING_SIZE 350

//constants from parsers (input data parser)
#define NUMS_COUNT 11
#define WORD_COUNT 100
#define CATEGORIES_COUNT 5

#define ADJECTIVE 0
#define ADVERB 1

// UTF-8 feature, the first byte of character is a mark of language
#define RUSSIAN -48

// Categories
#define TEMPERATURE 0
#define PRECIPITATION 1
#define WIND 2
#define PRESSURE 3
#define EVENT 4
#define RATING 5

#define PRECIPITATION_COUNT 11

#endif //WEATHERPROJECT_SHAREDCONSTANTS_H
