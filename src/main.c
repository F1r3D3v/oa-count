#include "file.h"
#include "helper.h"
#include "log.h"
#include "math.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function declarations
void cleanup();
void getOutputFilename(const char *inputfilename, char **outputfilename);

// Variable definitions
FILE *input;
FILE *output;
char *outputFilename = NULL;
char *arg1 = NULL;
char *arg2 = NULL;
char *op = NULL;
char *result = NULL;
char *rest = NULL;
char *err = NULL;
char *temp = NULL;
char *line = NULL;
size_t len;
int op_system;
int convert_system_from;
int convert_system_to;
int arg_count = 0;
int newline_count = 0;
BOOL command = TRUE;
BOOL convert = FALSE;
BOOL last_line;
BOOL first_command = TRUE;
BOOL invalid_op = FALSE;

//Main program
int main(int argc, char *argv[]) {

	// Check if the user provided correct argument
	if (argc != 2) {
		printf("Invalid amount of arguments! (One is required)\n");
		return -1;
	}

	printf("Running...\n");

	// Create input file handler and check if file can be opened
	input = fopen(argv[1], "rb");
	if (!input)
	{
		printf("[ERROR]|[INPUT] - Can't open a file!\n");
		return -1;
	}

	//Get output basename from input basename (e.g. oa_00_in.txt --> oa_00_out.txt)
	getOutputFilename(argv[1], &outputFilename);

	// Create output file handler and check if file can be created
	output = fopen(outputFilename, "w");
	if (!output)
	{
		printf("[ERROR]|[OUTPUT] - Can't create a file!\n");
		return -1;
	}

	// Read input file line by line
	for (ssize_t read; (read = getLine(&line, &len, &last_line, input)) != -1 || last_line;)
	{
		// Make one line spacing between commands and arguments
		if (isEmpty(line))
		{
			if (!command)
				++newline_count;
			if ((newline_count != 2 || command) && !last_line)
			{
				if (command)
					continue;

				fwrite("\n", sizeof(char), 1, output);
			}
		}
		else if (!command && !isEmpty(line) && (newline_count == 0 || newline_count == 2))
		{
			printf("Wrong file formatting!\nExiting...\n");
			cleanup();
			return 0;
		}

		if (!isEmpty(line) && newline_count != 2)
		{
			if (!first_command && command)
				fwrite("\n", sizeof(char), 1, output);

			// Write line from input file to output file
			fwrite(line, sizeof(char), len, output);
			fwrite("\n", sizeof(char), 1, output);

			// Process command and argument(s) and perform the appropriate operation
			if (command)
			{
				// Process command
				if (str_split(line, ' ', 2) == NULL)
				{
					op = str_split(line, ' ', 0);
					if (op[0] == '+' || op[0] == '-' || op[0] == '*' || op[0] == '/' || op[0] == '^' || op[0] == '%')
					{
						op_system = atoi(str_split(line, ' ', 1));
					}
					else if (isDigits(str_split(line, ' ', 0)) && isDigits(str_split(line, ' ', 1)))
					{
						convert_system_from = atoi(str_split(line, ' ', 0));
						convert_system_to = atoi(str_split(line, ' ', 1));
						convert = TRUE;
					}
				}
				else
				{
					invalid_op = TRUE;
				}
				newline_count = 0;
				command = FALSE;
			}
			else {
				// Process arguments
				++arg_count;
				if (arg_count == 1)
				{
					temp = realloc(arg1, strlen(line) + 1);
					if (temp)
					{
						arg1 = temp;
						strcpy(arg1, line);
					}
					newline_count = 0;
				}
				else if (arg_count == 2 && !convert)
				{
					temp = realloc(arg2, strlen(line) + 1);
					if (temp)
					{
						arg2 = temp;
						strcpy(arg2, line);
					}
					arg_count = 0;
					newline_count = 0;
				}
				else
					--arg_count;
			}
		}

		if ((isEmpty(line) && !command && newline_count == 2) || (last_line && ((arg1 && arg2) || (arg1 && convert))))
		{
			// Perform operation with input data validation and time limit
			switch ((invalid_op) ? 0 : op[0]) {
			case '+':
				number_addition(arg1, arg2, &result, op_system, TRUE, TRUE);
				break;
			case '-':
				number_subtraction(arg1, arg2, &result, op_system, TRUE, TRUE);
				break;
			case '*':
				number_multiplication(arg1, arg2, &result, op_system, TRUE, TRUE);
				break;
			case '/':
				number_division(arg1, arg2, &result, &rest, op_system, TRUE, TRUE);
				break;
			case '^':
				number_power(arg1, arg2, &result, op_system, TRUE, TRUE);
				break;
			case '%':
				number_modulo(arg1, arg2, &result, op_system, TRUE, TRUE);
				break;
			default:
				if (convert)
				{
					system_convert(arg1, &result, convert_system_from, convert_system_to, TRUE, TRUE);
					convert = FALSE;
				}
				else
				{
					printf("[Error]|[Operation] - Invalid Operation!\n");
					invalid_op = TRUE;
				}
				break;
			}

			if (last_line && newline_count != 2)
				fwrite("\n", sizeof(char), 1, output);

			// Write result to output file
			if (getTimeExceed())
				fwrite("Timeout", sizeof(char), 7, output);
			else if (result)
				fwrite(result, sizeof(char), strlen(result), output);
			else if (invalid_op)
				fwrite("Invalid Operation", sizeof(char), 17, output);
			else
				fwrite("Error", sizeof(char), 5, output);

			fwrite("\n", sizeof(char), 1, output);

			// Prepare to the next command
			free(op);
			free(arg1);
			free(arg2);
			free(result);
			op = NULL;
			arg1 = NULL;
			arg2 = NULL;
			result = NULL;
			newline_count = 0;
			arg_count = 0;
			command = TRUE;
			invalid_op = FALSE;

			if (first_command)
				first_command = FALSE;
		}
	}

	// Print error messages if they occur 
	while (getError(&err)) {
		printf("%s\n", err);
	}

	// Clean and exit
	printf("Exiting...\n");
	cleanup();
	return 0;
}


//Get output basename from input basename
void getOutputFilename(const char *input_path, char **output_filename) {
	char *filename = path_basename(input_path, FALSE);
	char *extension = getExtension(input_path);
	char *split1 = str_split(filename, '_', 0);
	char *split2 = str_split(filename, '_', 1);

	if (!strcmp(filename, split1))
	{
		temp = realloc(*output_filename, strlen(filename) + 1);
		if (temp)
		{
			*output_filename = temp;
			strcpy(*output_filename, filename);
			strcat(*output_filename, "_out");
			if (extension)
				strcat(*output_filename, extension);
		}
		return;
	}

	size_t size = strlen(split1) + strlen(split2) + ((extension) ? strlen(extension) : 0) + 5;
	temp = realloc(*output_filename, size + 1);
	if (temp)
	{
		*output_filename = temp;
		strcpy(*output_filename, split1);
		strcat(*output_filename, "_");
		strcat(*output_filename, split2);
		strcat(*output_filename, "_out");
		if (extension)
			strcat(*output_filename, extension);
	}
}

// Program memory cleanup
void cleanup() {
	fclose(input);
	fclose(output);
	free(outputFilename);
	free(arg1);
	free(arg2);
	free(op);
	free(line);
	free(result);
	free(rest);
	free(err);
}