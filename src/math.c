#include "math.h"
#include "config.h"
#include "helper.h"
#include "log.h"
#include "timer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void number_addition(const char *num1, const char *num2, char **buffer, size_t system, BOOL validation, BOOL timelimit) {
	if (validation) {
		BOOL failed = FALSE;
		if (!isSystemValid(system)) {
			setError("[Error]|[Addition] - Given system is invalid! (System not supported)");
			failed = TRUE;
		}
		if (!isValid(num1, system) || !isValid(num2, system)) {
			setError("[Error]|[Addition] - Given numbers are invalid! (Invalid characters or length)");
			failed = TRUE;
		}
		if (failed)
			return;
	}

	if (timelimit)
		StartTimer();

	if (!num1 || !num2) return;

	char *temp = NULL;
	char *max = NULL, *min = NULL;
	size_t min_size = 0, max_size = 0, buffer_size;
	size_t a, b, sum;
	unsigned int carry = 0;
	size_t num1_size = strlen(num1);
	size_t num2_size = strlen(num2);
	BOOL greater = num1_size < num2_size;

	max = malloc(((greater) ? num2_size : num1_size) + 1);
	if (max)
	{
		if (greater)
			strcpy(max, num2);
		else
			strcpy(max, num1);

		removeZeros(&max);
		max_size = strlen(max);

		min = malloc(((greater) ? num1_size : num2_size) + 1);
		if (min)
		{
			if (greater)
				strcpy(min, num1);
			else
				strcpy(min, num2);

			removeZeros(&min);
			min_size = strlen(min);

			buffer_size = max_size + 1; // Addition result can be 1 digit larger than bigger number
			temp = realloc(*buffer, buffer_size + 1); // Allocate buffer memory (buffer size + 1 for null terminating character)
			if (temp)
			{
				*buffer = temp;
				(*buffer)[0] = '0'; // Set additional digit to zero (so it can be removed later in removeZeros function)
				(*buffer)[buffer_size] = '\0'; // Add null terminating character


				// Adding corresponding digits (from right to left) and set carry if necessary
				for (size_t i = 0; (i < max_size) || carry; ++i) {
					a = (i < max_size) ? getIndex(max[max_size - i - 1], system_representation, SIZE(system_representation)) : 0;

					b = (i < min_size) ? getIndex(min[min_size - i - 1], system_representation, SIZE(system_representation)) : 0;

					sum = a + b + carry;
					carry = 0;

					if (sum >= system) {
						carry = 1;
						sum %= system;
					}

					// Set resulting digit to appropriate place in buffer
					(*buffer)[buffer_size - i - 1] = system_representation[sum];

					//Check Time Limit
					if (timelimit && (getTimeExceed() || checkTimeLimitExceed()))
					{
						if (validation) setError("[Error]|[Addition] - Timeout!");
						ResetTimer();
						setTimeExceed(TRUE);
						break;
					}
				}
				removeZeros(buffer); // Removes added zero if digit doesn't change

				if (validation && !isResultValid(*buffer))
				{
					setError("[Error]|[Addition] - Given result exceeded maximum length!");
					free(*buffer);
					*buffer = NULL;
				}
			}
			free(min);
		}
		free(max);
	}
}

void number_subtraction(const char *num1, const char *num2, char **buffer, size_t system, BOOL validation, BOOL timelimit) {
	if (validation) {
		BOOL failed = FALSE;
		if (!isSystemValid(system)) {
			setError("[Error]|[Subtraction] - Given system is invalid! (System not supported)");
			failed = TRUE;
		}
		if (!isValid(num1, system) || !isValid(num2, system)) {
			setError("[Error]|[Subtraction] - Given numbers are invalid! (Invalid characters or length)");
			failed = TRUE;
		}
		if (!isGreater(num1, num2, TRUE)) {
			setError("[Error]|[Subtraction] - First number must be bigger than the second!"); // Negative numbers are not supported
			failed = TRUE;
		}
		if (failed)
			return;
	}

	if (timelimit)
		StartTimer();

	if (!num1 || !num2) return;

	char *temp = NULL;
	char *max = NULL, *min = NULL;
	size_t max_size, min_size, buffer_size;
	size_t a, b;
	ssize_t sub;
	unsigned int carry = 0;

	max = malloc(strlen(num1) + 1);
	if (max)
	{
		strcpy(max, num1);
		removeZeros(&max);
		max_size = strlen(max);

		min = malloc(strlen(num2) + 1);
		if (min)
		{
			strcpy(min, num2);
			removeZeros(&min);
			min_size = strlen(min);

			buffer_size = max_size; // Subtraction result can be as large as larger number
			temp = realloc(*buffer, buffer_size + 1); // Allocate buffer memory (buffer size + 1 for null terminating character)
			if (temp)
			{
				*buffer = temp;
				(*buffer)[buffer_size] = '\0'; // Add null terminating character

				// Subtracting corresponding digits (from right to left) and decrement next digit if necessary
				for (size_t i = 0; (i < max_size) || carry; ++i) {
					a = (i < max_size) ? getIndex(max[max_size - i - 1], system_representation, SIZE(system_representation)) : 0;

					b = (i < min_size) ? getIndex(min[min_size - i - 1], system_representation, SIZE(system_representation)) : 0;

					sub = a - b - carry;
					carry = 0;

					if (sub < 0) {
						carry = 1;
						sub += system;
					}

					// Set resulting digit to appropriate place in buffer
					(*buffer)[buffer_size - i - 1] = system_representation[sub];

					//Check Time Limit
					if (timelimit && (getTimeExceed() || checkTimeLimitExceed()))
					{
						if (validation) setError("[Error]|[Subtraction] - Timeout!");
						ResetTimer();
						setTimeExceed(TRUE);
						break;
					}
				}
				removeZeros(buffer); // Remove non significant zeros

				if (validation && !isResultValid(*buffer))
				{
					setError("[Error]|[Subtraction] - Given result exceeded maximum length!");
					free(*buffer);
					*buffer = NULL;
				}
			}
			free(min);
		}
		free(max);
	}
}

void number_multiplication(const char *num1, const char *num2, char **buffer, size_t system, BOOL validation, BOOL timelimit) {
	if (validation) {
		BOOL failed = FALSE;
		if (!isSystemValid(system)) {
			setError("[Error]|[Multiplication] - Given system is invalid! (System not supported)");
			failed = TRUE;
		}
		if (!isValid(num1, system) || !isValid(num2, system)) {
			setError("[Error]|[Multiplication] - Given numbers are invalid! (Invalid characters or length)");
			failed = TRUE;
		}
		if (failed)
			return;
	}

	if (timelimit)
		StartTimer();

	if (!num1 || !num2) return;

	char *temp = NULL;
	char *min = NULL, *max = NULL, *result = NULL;
	size_t min_size, max_size, result_size;
	size_t a, b, mul;
	unsigned int carry = 0;
	size_t num1_size = strlen(num1);
	size_t num2_size = strlen(num2);

	// Determine if first number is bigger than the other one (to make shorter loop)
	BOOL greater = isGreater(num1, num2, TRUE);

	max = malloc(((greater) ? num1_size : num2_size) + 1);
	if (max)
	{
		if (greater)
			strcpy(max, num1);
		else
			strcpy(max, num2);

		removeZeros(&max);
		max_size = strlen(max);

		min = malloc(((greater) ? num2_size : num1_size) + 1);
		if (min)
		{
			if (greater)
				strcpy(min, num2);
			else
				strcpy(min, num1);

			removeZeros(&min);
			min_size = strlen(min);

			result_size = max_size + 1; // Result of multiplication one number by digit can be as large as the sum of both number length, so the bigger number + 1
			temp = realloc(*buffer, 2); // Allocate buffer memory
			if (temp)
			{
				*buffer = temp;
				strcpy(*buffer, "0"); // Set buffer to 0

				for (size_t j = 0; (j < min_size); ++j) {
					b = getIndex(min[min_size - j - 1], system_representation, SIZE(system_representation));

					if (b == 0) continue; // If digit is zero we go to next digit because we will add insignificant zero to buffer

					temp = realloc(result, result_size + 1); // Realloc temporary result buffer because bitShiftLeft function truncate the buffer
					if (temp)
					{
						result = temp;
						result[0] = '0'; // Add zero in order to get rid of newly allocated garbage (rest will be set in the loop)
						result[result_size] = '\0'; // Add null terminating character

						for (size_t i = 0; (i < max_size) || carry; ++i) {
							a = (i < max_size) ? getIndex(max[max_size - i - 1], system_representation, SIZE(system_representation)) : 0;

							mul = (a * b) + carry;
							carry = 0;

							if (mul >= system) {
								carry = (unsigned int)(mul / system);
								mul %= system;
							}

							// Set resulting digit to appropriate place in temporary result buffer
							result[result_size - i - 1] = system_representation[mul];
						}

						bitShiftLeft(&result, j); // Bit shift the result by some digits (depending on loop iterator)
						number_addition(*buffer, result, buffer, system, FALSE, timelimit); // Add result to final buffer

						//Check Time Limit
						if (timelimit && (getTimeExceed() || checkTimeLimitExceed()))
						{
							if (validation) setError("[Error]|[Multiplication] - Timeout!");
							ResetTimer();
							setTimeExceed(TRUE);
							break;
						}

						if (validation && !isResultValid(*buffer))
						{
							setError("[Error]|[Multiplication] - Given result exceeded maximum length!");
							free(*buffer);
							*buffer = NULL;
						}
					}
				}
				if (result)
					free(result);
			}
			free(min);
		}
		free(max);
	}
}

void number_power(const char *value, const char *power, char **buffer, size_t system, BOOL validation, BOOL timelimit) {
	if (validation) {
		BOOL failed = FALSE;
		if (!isSystemValid(system)) {
			setError("[Error]|[Power] - Given system is invalid! (System not supported)");
			failed = TRUE;
		}
		if (!isValid(value, system) || !isValid(power, system)) {
			setError("[Error]|[Power] - Given numbers are invalid! (Invalid characters or length)");
			failed = TRUE;
		}
		if (failed)
			return;
	}

	if (timelimit)
		StartTimer();

	if (!value || !power) return;

	size_t size;
	char *binary = NULL;
	char *temp = NULL;
	temp = realloc(*buffer, 2); // Allocate buffer memory
	if (temp)
	{
		*buffer = temp;
		strcpy(*buffer, "1"); // Set buffer to 1

		// Allocate memory for temp variable (to use with my functions)
		char *temp_var = malloc(strlen(value) + 1);
		if (temp_var)
		{
			strcpy(temp_var, value); // Set temp_sum to value

			system_convert(power, &binary, system, 2, FALSE, TRUE);
			size = strlen(binary);
			for (size_t i = 0; i < size; ++i)
			{
				if (binary[size - i - 1] == '1')
					number_multiplication(*buffer, temp_var, buffer, system, FALSE, timelimit);
				if (!getTimeExceed())
					number_multiplication(temp_var, temp_var, &temp_var, system, FALSE, timelimit);

				//Check Time Limit
				if (timelimit && (getTimeExceed() || checkTimeLimitExceed()))
				{
					if (validation) setError("[Error]|[Power] - Timeout!");
					ResetTimer();
					setTimeExceed(TRUE);
					break;
				}

				if (validation && !isResultValid(*buffer))
				{
					setError("[Error]|[Power] - Given result exceeded maximum length!");
					free(*buffer);
					*buffer = NULL;
				}
			}
			free(temp_var);
		}
	}
}

void number_division(const char *divident, const char *divisor, char **buffer, char **rest, size_t system, BOOL validation, BOOL timelimit) {
	if (validation) {
		BOOL failed = FALSE;
		if (!isSystemValid(system)) {
			setError("[Error]|[Division] - Given system is invalid! (System not supported)");
			failed = TRUE;
		}
		if (!isValid(divident, system) || !isValid(divisor, system)) {
			setError("[Error]|[Division] - Given numbers are invalid! (Invalid characters or length)");
			failed = TRUE;
		}
		if (isEqual(divisor, "0")) {
			setError("[Error]|[Division] - Can't divide by zero!");
			failed = TRUE;
		}
		if (strlen(divident) > MAX_RESULT_LENGTH)
		{
			setError("[Error]|[Division] - Given result exceeded maximum length!");
			failed = TRUE;
		}
		if (failed)
			return;
	}

	if (timelimit)
		StartTimer();

	if (!divident || !divisor) return;

	char *temp = NULL;
	char *temp_divident = NULL, *temp_divisor = NULL, *temp_buf = NULL, *result = NULL;
	size_t temp_divident_size, buffer_size;
	size_t divident_size = strlen(divident);
	size_t divisor_size = strlen(divisor);

	temp_divident = malloc(divident_size + 1);
	if (temp_divident)
	{
		strcpy(temp_divident, divident);
		removeZeros(&temp_divident);
		temp_divident_size = strlen(temp_divident);

		temp_divisor = malloc(divisor_size + 1);
		if (temp_divisor)
		{
			strcpy(temp_divisor, divisor);
			removeZeros(&temp_divisor);

			// If divisor is greater than divident return 0 and appropriate rest (floating point calculations not supported)
			if (!isGreater(temp_divident, temp_divisor, TRUE))
			{
				temp = realloc(*buffer, 2);
				if (temp)
				{
					*buffer = temp;
					strcpy(*buffer, "0");
				}
				temp = realloc(*rest, temp_divident_size + 1);
				if (temp)
				{
					*rest = temp;
					strcpy(*rest, temp_divident);
				}
				return;
			}

			buffer_size = temp_divident_size; // Result of division can't be larger than the divident length
			result = malloc(1);
			if (result)
			{
				result[0] = '\0'; // Add null terminating character

				temp_buf = malloc(buffer_size);
				if (temp_buf)
				{
					size_t i, j;
					for (i = 0; i < buffer_size; ++i) {
						push_front(temp_divident[i], &result); // Append the consecutive digit from divident to temporary result buffer

						// If result is greater than divisor, count multiple of the divisor (by subtracting until result is less than the divisor)
						for (j = 0; isGreater(result, divisor, TRUE); ++j) {
							number_subtraction(result, divisor, &result, system, FALSE, timelimit); // Subtract
							if (getTimeExceed()) break;
						}

						// Set appropriate digit to temporary buffer
						temp_buf[i] = system_representation[j];

						//Check Time Limit
						if (timelimit && (getTimeExceed() || checkTimeLimitExceed()))
						{
							if (validation) setError("[Error]|[Division] - Timeout!");
							ResetTimer();
							setTimeExceed(TRUE);
							break;
						}
					}

					// Allocate final buffer memory, copy result from temporary buffer to final buffer, and add null terminating character
					temp = realloc(*buffer, buffer_size + 1);
					if (temp)
					{
						*buffer = temp;
						memcpy(*buffer, temp_buf, buffer_size);
						(*buffer)[buffer_size] = '\0';
					}

					removeZeros(buffer); // Remove non significant zeros

					// Allocate rest buffer and copy the rest from result buffer
					temp = realloc(*rest, strlen(result) + 1);
					if (temp)
					{
						*rest = temp;
						strcpy(*rest, result);
					}

					removeZeros(rest);

					free(temp_buf);
				}
				free(result);
			}
			free(temp_divisor);
		}
		free(temp_divident);
	}
}

void number_modulo(const char *divident, const char *divisor, char **rest, size_t system, BOOL validation, BOOL timelimit) {
	if (validation) {
		BOOL failed = FALSE;
		if (!isSystemValid(system)) {
			setError("[Error]|[Modulo] - Given system is invalid! (System not supported)");
			failed = TRUE;
		}
		if (!isValid(divident, system) || !isValid(divisor, system)) {
			setError("[Error]|[Modulo] - Given numbers are invalid! (Invalid characters or length)");
			failed = TRUE;
		}
		if (isEqual(divisor, "0")) {
			setError("[Error]|[Modulo] - Can't divide by zero!");
			failed = TRUE;
		}
		if (failed)
			return;
	}

	if (!divident || !divisor) return;

	char *temp = NULL;
	number_division(divident, divisor, &temp, rest, system, FALSE, timelimit);
	if (getTimeExceed() && validation) setError("[Error]|[Modulo] - Timeout!");
	if (validation && !isResultValid(*rest))
	{
		setError("[Error]|[Modulo] - Given result exceeded maximum length!");
		free(*rest);
		*rest = NULL;
	}
	free(temp);
}

void system_convert(const char *value, char **buffer, size_t from, size_t to, BOOL validation, BOOL timelimit) {
	if (validation) {
		BOOL failed = FALSE;
		if (!isSystemValid(from) || !isSystemValid(to)) {
			setError("[Error]|[Convert] - Given systems are invalid! (System not supported)");
			failed = TRUE;
		}
		if (!isValid(value, from)) {
			setError("[Error]|[Convert] - Given number is invalid! (Invalid characters or length)");
			failed = TRUE;
		}
		if (failed)
			return;
	}

	if (!value) return;

	char *temp = NULL;

	if (from != to) {
		char *decimal = NULL;

		convertSystemToDecimal(value, &decimal, from, timelimit);
		if (!getTimeExceed())
			convertDecimalToSystem(decimal, buffer, to, timelimit);

		if (getTimeExceed() && validation) setError("[Error]|[Convert] - Timeout!");
		free(decimal);
	}
	else {
		temp = realloc(*buffer, strlen(value) + 1);
		if (temp)
		{
			*buffer = temp;
			strcpy(*buffer, value);
		}
	}

	if (validation && !isResultValid(*buffer))
	{
		setError("[Error]|[Convert] - Given result exceeded maximum length!");
		free(*buffer);
		*buffer = NULL;
	}
}
