#include "macro.h"
#include <stddef.h>
#include <string.h>

void convertSystemToDecimal(const char *value, char **buffer, size_t system, BOOL timelimit);

void convertDecimalToSystem(const char *value, char **buffer, size_t system, BOOL timelimit);

void bitShiftLeft(char **buffer, size_t amount);

void bitShiftRight(char **buffer, size_t amount);

void removeZeros(char **array);

BOOL isGreater(const char *greater, const char *lesser, BOOL equal);

BOOL isEqual(const char *num1, const char *num2);

BOOL isValid(const char *array, size_t system);

BOOL exist(char value, const char *array, size_t size);

ssize_t getIndex(char value, const char *array, size_t size);

BOOL isSystemValid(size_t system);

BOOL isResultValid(const char *result);

void push_back(char value, char **buffer);

void push_front(char value, char **buffer);

char *str_split(const char *string, char delimeter, size_t index);

BOOL checkTimeLimitExceed();

void setTimeExceed(BOOL value);

BOOL getTimeExceed();

BOOL isDigits(const char *string);

static inline BOOL isEmpty(const char *string) { return (strlen(string) == 0); }
