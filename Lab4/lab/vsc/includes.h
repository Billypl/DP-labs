#include <stdio.h>
#include <stdbool.h>

#define RED "\033[0;31m"
#define GREEN "\033[0;32m"

#define NOCOLOR "\033[0m"
#define PURPLE "\033[0;35m"
#define UGreen "\033[4;32m"
#define On_Red "\033[41m"

#define INDENTATION_CHAR "-"
static int indentationCount = 0;
void indent() { indentationCount++; }
void deindent() { indentationCount--; }

#define LOGGER(printer)                        \
    for (int i = 0; i < indentationCount; i++) \
    {                                          \
        printf(INDENTATION_CHAR);              \
    }                                          \
    printf(" ");                               \
    printer;

#define printColored(printer, color) \
    printf("%s", color);             \
    printer;                         \
    printf("%s", NOCOLOR);

#define setColor(color) printf("%s", color);
#define setDefault() printf("%s", NOCOLOR);

#define RANDOM_CONST 2137

#define MAX_PROCESSES_NUMBER 100
#define MAX_THREADS_NUMBER 100
#define LOADING_BAR_LENGTH 50
#define LOADING_BAR_CHAR "#"
#define LOADING_BARS_OFFSET 20