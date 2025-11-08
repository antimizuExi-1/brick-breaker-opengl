#ifndef BRICK_UTILS_H
#define BRICK_UTILS_H

#include "Export.h"

typedef enum
{
    Brk_INFO,
    Brk_WARNING,
    Brk_ERROR,
    Brk_CRITICAL
} Brk_LogLevel;

BrkAPI void Brk_SetLevelThreshold(Brk_LogLevel level);

void Brk_Logging(Brk_LogLevel level, const char* file, int line, const char* format, ...);

// Note: This function will malloc a new String to return, remember free it
BrkAPI char* Brk_LoadTextFile(const char* fileName);

#ifndef BrkLogging
#define BrkLogging(level, format, ...) Brk_Logging(level, __FILE__, __LINE__, format, ##__VA_ARGS__)
#endif

#endif //BRICK_UTILS_H
