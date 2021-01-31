#ifndef WEATHERPROJECT_EVALUATELEVELS_H
#define WEATHERPROJECT_EVALUATELEVELS_H

int getTemperatureLevel();

int getWindLevel();

int getPressureLevel();

int getPrecipitationOrEventGroup(const char *request);

double calcRating(char *request);

#endif //WEATHERPROJECT_EVALUATELEVELS_H
