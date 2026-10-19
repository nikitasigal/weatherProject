# weatherProject

A small C program that turns a table of numeric weather forecast data into a readable, Russian-language text forecast, one paragraph per day.

For each day it reads temperature, precipitation, wind, pressure and weather events, rates them, and builds sentences from templates and synonym dictionaries. The categories are ordered by how notable they are that day, and word endings are inflected to match gender and case.

## Project layout

| File | Purpose |
| --- | --- |
| `main.c` | Entry point: loads constants, reads the forecast file, writes the output |
| `parsers.c` / `parsers.h` | Parse constants, dictionaries, templates and each input row |
| `evaluateLevels.c` / `evaluateLevels.h` | Compute temperature, wind and pressure levels and rate and sort categories |
| `sentenceHandler.c` / `sentenceHandler.h` | Generate simple and complex sentences and print the weekday |
| `sharedConstants.h` | Shared sizes and category constants |

## Requirements

- A C11 compiler
- CMake 3.17 or newer
- Windows: `main.c` includes `windows.h` to switch the console to UTF-8

## Build

```sh
cmake -S . -B build
cmake --build build
```

## Run

The program loads these folders from the working directory at startup:

- `Constants/`: statistics and word-ending tables
- `Dictionaries/`: adjective and adverb synonym lists
- `Templates/`: sentence templates
- `Forecast Data/`: your input file, and where the output is written

Start the program, then enter the name of a forecast file inside `Forecast Data/` when prompted:

```
Enter forecast data file name: forecast.txt
```

The generated forecast is written to `Forecast Data/output.txt`. The first line of the input file is treated as a table header and skipped, and each following line is one day.

## Notes

The data folders are not part of this repository, so you need to supply them yourself before the program will run.
