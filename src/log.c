#include "log.h"
#include <stdlib.h>
#include <string.h>

char **error = NULL;
size_t counter = 0;

// Push error message on the stack
void setError(const char *msg) {
	char **temp = NULL;
	char **temp_buf = malloc((counter + 1) * sizeof(char *));
	if (temp_buf)
	{
		if (!error)
			error = malloc(sizeof(char *));

		if (error) {
			memcpy(temp_buf, error, (counter + 1) * sizeof(char *));
			temp = realloc(error, ((counter + 2) * sizeof(char *)));
			if (temp)
			{
				error = temp;
				memcpy(error, temp_buf, (counter + 1) * sizeof(char *));
				error[counter] = malloc(strlen(msg) + 1);
				if (error[counter])
				{
					strcpy(error[counter], msg);
					error[counter + 1] = NULL;
					++counter;
				}
			}
		}
		free(temp_buf);
	}
}

// Get size of stack
size_t getErrorCount() {
	size_t i = 0;
	while (error[i] != NULL) {
		++i;
	}
	return i;
}

// Pull error message from the stack
BOOL getError(char **buffer) {
	if (counter == 0 || !error[counter - 1] || !error)
		return FALSE;

	char *temp1 = NULL;
	char **temp2 = NULL;

	temp1 = realloc(*buffer, strlen(error[counter - 1]) + 1);
	if (temp1)
	{
		*buffer = temp1;
		strcpy(*buffer, error[counter - 1]);
	}

	char **temp_buf = malloc((counter + 1) * sizeof(char *));
	if (temp_buf)
	{
		memcpy(temp_buf, error, (counter + 1) * sizeof(char *));
		temp2 = realloc(error, counter * sizeof(char *));
		if (temp2)
		{
			error = temp2;
			memcpy(error, temp_buf, counter * sizeof(char *));
			free(error[counter - 1]);
			error[counter - 1] = NULL;
			--counter;
		}
	}

	return TRUE;
}