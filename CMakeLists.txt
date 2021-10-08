cmake_minimum_required(VERSION 3.17)
project(weatherProject C)

set(CMAKE_C_STANDARD 11)

add_executable(weatherProject main.c sharedConstants.h sentenceHandler.c sentenceHandler.h evaluateLevels.c evaluateLevels.h parsers.c parsers.h)