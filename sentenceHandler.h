#ifndef WEATHERPROJECT_SENTENCEHANDLER_H
#define WEATHERPROJECT_SENTENCEHANDLER_H

#include "constantParser.h"

void generateSimple(FILE *outputFile, int ctg, Data *data);

void generateComplex(FILE *outputFile, int ctg, Data *data);

void printWeekDay(FILE *outputFile, Data *data);

#endif //WEATHERPROJECT_SENTENCEHANDLER_H
