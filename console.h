/* This is a simple file for dumping text to a console, including pretty
 * colours and some things like unicode smily faces. */
#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define DEBUG_VERY_HIGH 40
#define DEBUG_HIGH 30
#define DEBUG_MEDIUM 20
#define DEBUG_LOW 10
#define DEBUG_ALWAYS -10

#define DEBUG_LEVEL DEBUG_LOW

#define USE_COLOUR
#define USE_UNICODE

#ifdef USE_COLOUR
#define COLOUR_LIGHT_RED "\033[1;31m"
#define COLOUR_LIGHT_GREEN "\033[1;32m"
#define COLOUR_YELLOW "\033[1;33m"
#define COLOUR_LIGHT_BLUE "\033[1;34m"
#define COLOUR_LIGHT_MAGENTA "\033[1;35m"
#define COLOUR_LIGHT_CYAN "\033[1;36m"
#define COLOUR_WHITE "\033[1;37m"
#define COLOUR_UNCOLOUR "\033[0m"
#else
#define COLOUR_LIGHT_RED ""
#define COLOUR_LIGHT_GREEN ""
#define COLOUR_YELLOW ""
#define COLOUR_LIGHT_BLUE ""
#define COLOUR_LIGHT_MAGENTA ""
#define COLOUR_LIGHT_CYAN ""
#define COLOUR_WHITE ""
#define COLOUR_UNCOLOUR ""
#endif

#ifdef USE_UNICODE
#define SYMBOL_SMILEY "☺"
#define SYMBOL_INVERTED_SMILEY "☻"
#define SYMBOL_SADLY "☹"
#define SYMBOL_FATAL "☠"
#define SYMBOL_WARNING "☢"
#define SYMBOL_DEGREES "°"
#else
#define SYMBOL_SMILY ":)"
#define SYMBOL_INVERTED_SMILEY "(:"
#define SYMBOL_SADLEY ":("
#define SYMBOL_FATAL ">_<"
#define SYMBOL_WARNING "(!)"
#define SYMBOL_DEGREES "deg"
#endif

void logit(const char* format, ...);
void errorit(const char* format, ...);
void debugit(int level, const char* format, ...);
void warnit(const char* format, ...);

#endif
