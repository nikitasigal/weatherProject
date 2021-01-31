#include <string.h>
#include "evaluateLevels.h"
#include "dataParser.h"
#include "constantParser.h"

char precipitationsAndEvents[11][STRING_SIZE] = {"дождь", "снег", "кислотныйдождь", "град", "туман", "метель", "гололедица",
                                                 "гроза", "конецсвета", "никитаgay", "слякоть"};

int getTemperatureLevel() {
    double averageTemperature = ((curDayNums[2] + curDayNums[3] + curDayNums[4] + curDayNums[5]) / 4.0) - StatTemperature[curDate.month - 1];
    if (curDate.month >= 4 && curDate.month <= 9) {
        if (averageTemperature < -5) {
            return 0;
        } else {
            if (averageTemperature > 5) {
                return 2;
            } else {
                return 1;
            }
        }
    } else {
        if (averageTemperature < -7) {
            return 3;
        } else {
            if (averageTemperature > 7) {
                return 5;
            } else {
                return 4;
            }
        }
    }
}

int getWindLevel() {
    double averageWind = ((curDayNums[6] + curDayNums[7]) + ((curDayNums[8] + curDayNums[9]) * 0.6)) / 4.0;
    for (int i = 1; i < 4; ++i) {
        if (averageWind < StatWindScale[i]) {
            return 4 - i;
        }
    }
    return 0;
}

int getPressureLevel() {
    int pressureDifference = curDayNums[10] - StatPressure[curDate.month];
    if (pressureDifference < -12) {
        return 2;
    } else {
        if (pressureDifference > 12) {
            return 0;
        } else {
            return 2;
        }
    }
}

int getPrecipitationOrEventGroup(const char* request) {
    for (int i = 0; i < 11; ++i) {
        if (strcmp(precipitationsAndEvents[i], request) == 0) {
            return i;
        }
    }
    return 0;
}