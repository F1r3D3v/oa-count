#include "macro.h"

void number_addition(const char *num1, const char *num2, char **buffer, size_t system, BOOL validation, BOOL timelimit);

void number_subtraction(const char *num1, const char *num2, char **buffer, size_t system, BOOL validation, BOOL timelimit);

void number_multiplication(const char *num1, const char *num2, char **buffer, size_t system, BOOL validation, BOOL timelimit);

void number_power(const char *value, const char *power, char **buffer, size_t system, BOOL validation, BOOL timelimit);

void number_division(const char *divident, const char *divisor, char **buffer, char **rest, size_t system, BOOL validation, BOOL timelimit);

void number_modulo(const char *divident, const char *divisor, char **rest, size_t system, BOOL validation, BOOL timelimit);

void system_convert(const char *value, char **buffer, size_t from, size_t to, BOOL validation, BOOL timelimit);