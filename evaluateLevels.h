#ifndef WEATHERPROJECT_EVALUATELEVELS_H
#define WEATHERPROJECT_EVALUATELEVELS_H

typedef struct Connection {
    double rating;
    int ctg;
} CONNECTION;
CONNECTION Order[5];

int getTemperatureLevel();

int getWindLevel();

int getPressureLevel();

int getPrecipitationOrEventGroup(const char *request);

int getDayLevel();

double calcRating(int ctg);

void sortCategories();

#endif //WEATHERPROJECT_EVALUATELEVELS_H
