#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "brick/Utils.h"


static Brk_LogLevel LogLevelThreshold = Brk_INFO;

void Brk_SetLevelThreshold(Brk_LogLevel level)
{
    LogLevelThreshold = level;
}

void Brk_Logging(Brk_LogLevel level, const char* file, int line, const char* format, ...)
{
    if (level < LogLevelThreshold) return;

    char* head = NULL;
    switch (level)
    {
    case Brk_INFO:
        head = "INFO";
        break;
    case Brk_WARNING:
        head = "WARNING";
        break;
    case Brk_ERROR:
        head = "ERROR";
        break;
    case Brk_CRITICAL:
        head = "CRITICAL";
        break;
    }
    if (level <= Brk_WARNING)
    {
        fprintf(stdout, "[%s] ", head);
        va_list args;
        va_start(args, format);
        vfprintf(stdout, format, args);
        va_end(args);
    }
    else
    {
        fprintf(stderr, "[%s] %s(%d) ", head, file, line);
        va_list args;
        va_start(args, format);
        vfprintf(stderr, format, args);
        va_end(args);
    }
}


char* Brk_LoadTextFile(const char* fileName)
{
    char* text = NULL;
    FILE* file = fopen(fileName, "r");
    if (file == NULL)
    {
        BrkLogging(Brk_ERROR, "File: %s not found\n", fileName);
    }
    else
    {
        fseek(file, 0, SEEK_END);
        const size_t file_size = ftell(file);
        fseek(file, 0, SEEK_SET);

        text = calloc(file_size + 1, sizeof(char));
        if (text == NULL)
        {
            BrkLogging(Brk_ERROR, "calloc failed\n");
            return NULL;
        }
        fread(text, sizeof(char), file_size, file);
        text[file_size] = '\0';
        fclose(file);
    }
    return text;
}
