#ifndef WEATHERPROJECT_EVALUATELEVELS_H
#define WEATHERPROJECT_EVALUATELEVELS_H

#include "constantParser.h"

int getTemperatureLevel(Data *data);

int getWindLevel(Data *data);

int getPressureLevel(Data *data);

int getPrecipitationOrEventGroup(const char *request);

int getDayLevel(Data *data);

double calcRating(int ctg, Data *data);

void sortCategories(Data *data);

#endif //WEATHERPROJECT_EVALUATELEVELS_H
