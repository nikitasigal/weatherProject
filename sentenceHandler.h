#ifndef WEATHERPROJECT_SENTENCEHANDLER_H
#define WEATHERPROJECT_SENTENCEHANDLER_H

double calcRate(char *request);

void calcPercent(const char *request, int trigger);

void generateSentence(char *cat, int level);

#endif //WEATHERPROJECT_SENTENCEHANDLER_H
