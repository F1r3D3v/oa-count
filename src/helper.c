#include "helper.h"
#include "config.h"
#include "log.h"
#include "math.h"
#include "timer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

BOOL timeExceed = FALSE;

// Check if one number is greater (or equal) from another number
BOOL isGreater(const char *greater, const char *lesser, BOOL equal) {

	if (!greater || !lesser) return FALSE;

	char *temp_greater = NULL, *temp_lesser = NULL;
	BOOL result = equal;

	temp_greater = malloc(strlen(greater) + 1);
	if (temp_greater)
	{
		strcpy(temp_greater, greater);
		removeZeros(&temp_greater);

		temp_lesser = malloc(strlen(lesser) + 1);
		if (temp_lesser)
		{
			strcpy(temp_lesser, lesser);
			removeZeros(&temp_lesser);

			// If one number have bigger length, it have bigger value
			// If number are equal in length compare corresponding digits
			// Number is larger when it has larger corresponding digit value
			if (strlen(temp_greater) > strlen(temp_lesser))
				result = TRUE;
			else if (strlen(temp_greater) < strlen(temp_lesser))
				result = FALSE;
			else {
				for (size_t i = 0; i < strlen(temp_greater); ++i) {
					if (temp_greater[i] == temp_lesser[i])
						continue;

					if (getIndex(temp_greater[i], system_representation, SIZE(system_representation)) > getIndex(temp_lesser[i], system_representation, SIZE(system_representation))) {
						result = TRUE;
						break;
					}
					else {
						result = FALSE;
						break;
					}
				}
			}
			free(temp_greater);
			free(temp_lesser);
			return result;
		}
		free(temp_greater);
	}
	return FALSE;
}

// Check if two numbers are equal
BOOL isEqual(const char *num1, const char *num2) {

	if (!num1 || !num2) return FALSE;

	char *temp_num1 = NULL, *temp_num2 = NULL;
	BOOL result = TRUE;

	temp_num1 = malloc(strlen(num1) + 1);
	if (temp_num1)
	{
		strcpy(temp_num1, num1);
		removeZeros(&temp_num1);

		temp_num2 = malloc(strlen(num2) + 1);
		if (temp_num2)
		{
			strcpy(temp_num2, num2);
			removeZeros(&temp_num2);

			// If one number have bigger length, it have bigger value
			if (strlen(temp_num1) == strlen(temp_num2)) {
				// Compare corresponding digits, if at least one pair is different, numbers are not equal
				for (size_t i = 0; i < strlen(temp_num1); ++i) {
					if (temp_num1[i] != temp_num2[i]) {
						result = FALSE;
						break;
					}
				}
			}
			else {
				result = FALSE;
			}
			free(temp_num1);
			free(temp_num2);
			return result;
		}
		free(temp_num1);
	}
	return FALSE;
}

// Convert base system to decimal
void convertSystemToDecimal(const char *value, char **buffer, size_t system, BOOL timelimit) {
	if (timelimit)
		StartTimer();

	if (!value) return;
	char *temp = NULL;

	// Converting from decimal system to decimal system requires nothing
	if (system == 10) {
		temp = realloc(*buffer, strlen(value) + 1);
		if (temp)
		{
			*buffer = temp;
			strcpy(*buffer, value);
		}
		return;
	}

	char *result = NULL;
	size_t value_size = strlen(value);

	temp = realloc(*buffer, 2);
	if (temp)
	{
		*buffer = temp;
		strcpy(*buffer, "0");

		char *str_system = malloc(21);
		if (str_system)
		{
			snprintf(str_system, 21, "%zu", system);

			char *value_length = malloc(21);
			if (value_length)
			{
				snprintf(value_length, 21, "%zu", value_size);

				char *str_value = malloc(20);
				if (str_value)
				{
					char *it = malloc(21);
					if (it)
					{
						// Multiply digit value by the base of 10 to the power of digit weight and add the result to the buffer
						for (size_t i = 0; i < value_size; ++i) {
							snprintf(it, 21, "%zu", i);
							snprintf(str_value, 20, "%zd", getIndex(value[value_size - i - 1], system_representation, SIZE(system_representation)));
							number_power(str_system, it, &result, 10, FALSE, timelimit);
							if (!timeExceed)
								number_multiplication(str_value, result, &result, 10, FALSE, timelimit);
							if (!timeExceed)
								number_addition(*buffer, result, buffer, 10, FALSE, timelimit);

							//Check Time Limit
							if(timelimit && (timeExceed || checkTimeLimitExceed()))
							{
								ResetTimer();
								timeExceed = TRUE;
								break;
							}
						}
						free(it);
						free(result);
					}
					free(str_value);
				}
				free(value_length);
			}
			free(str_system);
		}
	}
}

// Convert decimal to other base system
void convertDecimalToSystem(const char *value, char **buffer, size_t system, BOOL timelimit) {
	if (timelimit)
		StartTimer();

	if (!value) return;
	char *temp = NULL;

	// Converting from decimal system to decimal system requires nothing
	if (system == 10) {
		temp = realloc(*buffer, strlen(value) + 1);
		if (temp)
		{
			*buffer = temp;
			strcpy(*buffer, value);
		}
		return;
	}

	char *result = NULL, *rest = NULL, *str_system = NULL;

	temp = realloc(*buffer, 1);
	if (temp)
	{
		*buffer = temp;
		(*buffer)[0] = '\0';

		result = malloc(strlen(value) + 1);
		if (result)
		{
			strcpy(result, value);

			str_system = malloc(21);
			if (str_system)
			{
				snprintf(str_system, 21, "%zu", system);

				// Divide number by the system value and push back the rest from the division into the buffer
				while (!isEqual(result, "0")) {
					number_division(result, str_system, &result, &rest, 10, FALSE, timelimit);
					push_back(system_representation[atoi(rest)], buffer);

					//Check Time Limit
					if (timelimit && (timeExceed || checkTimeLimitExceed()))
					{
						ResetTimer();
						timeExceed = TRUE;
						break;
					}
				}

				free(rest);
				free(str_system);
			}
			free(result);
		}
	}
}

// Shift number to the left by given amount
void bitShiftLeft(char **buffer, size_t amount) {
	if (!*buffer) return;

	if (amount > 0) {
		char *temp = NULL;
		size_t buffer_size = strlen(*buffer);
		char *temp_buf = malloc(buffer_size + 1);
		if (temp_buf)
		{
			strcpy(temp_buf, *buffer);
			temp = realloc(*buffer, buffer_size + amount + 1);
			if (temp)
			{
				*buffer = temp;
				memset(*buffer + buffer_size, '0', amount);
				(*buffer)[buffer_size + amount] = '\0';
				memcpy(*buffer, temp_buf, buffer_size);
			}
			free(temp_buf);
		}
	}
	removeZeros(buffer);
}


// Shift number to the right by given amount
void bitShiftRight(char **buffer, size_t amount) {
	if (!*buffer) return;

	if (amount > 0) {
		char *temp = NULL;
		size_t buffer_size = strlen(*buffer);
		if (amount < buffer_size) {
			char *temp_buf = malloc(buffer_size + 1);
			if (temp_buf)
			{
				strcpy(temp_buf, *buffer);
				temp = realloc(*buffer, buffer_size - amount + 1);
				if (temp)
				{
					*buffer = temp;
					memcpy(*buffer, temp_buf, buffer_size - amount);
					(*buffer)[buffer_size - amount] = '\0';
				}
				free(temp_buf);
			}
		}
		else {
			// If amount of shift exceed the buffer size, return 0
			temp = realloc(*buffer, 2);
			if (temp)
			{
				*buffer = temp;
				strcpy(*buffer, "0");
			}
		}
	}
	removeZeros(buffer);
}

// Removes non significant zeros from the number
void removeZeros(char **array) {
	if (!*array) return;

	char *temp, *buffer;
	size_t size = strlen(*array);
	size_t i = 0;

	// Count initially occurring zeros
	while ((*array)[i] == '0') {
		++i;
	}

	// If amount of zeros is greater than 0 and it is not just "0", remove additional zeros
	if (i && !(i == size && i == 1)) {
		if (i == size) --i;
		buffer = malloc(size - i + 1);
		if (buffer)
		{
			memcpy(buffer, *array + i, size - i + 1);
			temp = realloc(*array, size - i + 1);
			if (temp)
			{
				*array = temp;
				memcpy(*array, buffer, size - i + 1);
			}
			free(buffer);
		}
	}
}

// Check if array is valid in the given numeral system
BOOL isValid(const char *array, size_t system) {
	if (!array) return FALSE;
	if (isEmpty(array)) return FALSE;
	for (size_t i = 0; array[i] != '\0'; ++i) {
		if (!exist(array[i], system_representation, system) || i > MAX_ARG_LENGTH - 1) return FALSE;
	}
	return TRUE;
}

// Check if given system is valid
BOOL isSystemValid(size_t system) {
	return (system >= MIN_NUMERAL_SYSTEM && system <= MAX_NUMERAL_SYSTEM && system <= SIZE(system_representation))
		? TRUE
		: FALSE;
}

BOOL isResultValid(const char *result)
{
	return (strlen(result) > MAX_RESULT_LENGTH) ? FALSE : TRUE;
}

// Check if given value exist in the array
BOOL exist(char value, const char *array, size_t size) {
	if (!array) return FALSE;
	for (size_t i = 0; i < size; ++i) {
		if (array[i] == value) return TRUE;
	}
	return FALSE;
}

// Get index of the first occurrence in given array
ssize_t getIndex(char value, const char *array, size_t size) {
	if (!array) return -1;
	for (size_t i = 0; i < size; ++i) {
		if (array[i] == value) return (ssize_t)i;
	}
	return -1;
}

// Push value to the back of the buffer
void push_back(char value, char **buffer) {
	char *temp = NULL;
	size_t buffer_size = strlen(*buffer);
	char *temp_buf = malloc(buffer_size + 1);
	if (temp_buf)
	{
		strcpy(temp_buf, *buffer);
		temp = realloc(*buffer, buffer_size + 2);
		if (temp)
		{
			*buffer = temp;
			(*buffer)[0] = value;
			memcpy(*buffer + 1, temp_buf, buffer_size + 1);
		}
		free(temp_buf);
	}
}

// Push value to the front of the buffer
void push_front(char value, char **buffer) {
	char *temp = NULL;
	size_t buffer_size = strlen(*buffer);
	char *temp_buf = malloc(buffer_size + 1);
	if (temp_buf)
	{
		strcpy(temp_buf, *buffer);
		temp = realloc(*buffer, buffer_size + 2);
		if (temp)
		{
			*buffer = temp;
			memcpy(*buffer, temp_buf, buffer_size);
			(*buffer)[buffer_size] = value;
			(*buffer)[buffer_size + 1] = '\0';
		}
		free(temp_buf);
	}
}

// Split given string into parts separated by delimeter, return part by index (starting from 0)
char *str_split(const char *string, char delimeter, size_t index) {
	if (!string) return NULL;

	char *splitstr;
	const char *delim, *last_delim = string;
	size_t size;
	size_t count = 0, i = 0;

	while (TRUE) {
		if (string[i] == delimeter || string[i] == '\0') {
			delim = &string[i];

			if (count == index) {
				size = delim - last_delim;
				splitstr = malloc(size + 1);
				if (splitstr)
				{
					memcpy(splitstr, last_delim, size);
					splitstr[size] = '\0';
				}
				return splitstr;
			}

			last_delim = delim + 1;
			++count;
		}

		if (string[i] == '\0')
			return NULL;

		++i;
	}
}

BOOL checkTimeLimitExceed()
{
	return (getDuration() > MAX_TIME_LIMIT_SECONDS);
}

void setTimeExceed(BOOL value)
{
	timeExceed = value;
}

BOOL getTimeExceed()
{
	return timeExceed;
}

BOOL isDigits(const char *string)
{
	if (!string) return FALSE;
	for (size_t i = 0; i < strlen(string); ++i)
	{
		if (!isdigit(string[i])) return FALSE;
	}
	return TRUE;
}
