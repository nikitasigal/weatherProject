//
// Created by paral on 25.01.2021.
//

#include "constantParser.h"
#include <stdio.h>
#include <string.h>

void constantParse() {
    FILE *averages = fopen("Dictionaries/averages.txt", "r");

    for (int i = 0; i < 12; ++i) {
        fscanf(averages, "%d%d", *(AvTemp[i]), *(AvPressure[i]));
    }

    fclose(averages);
}
