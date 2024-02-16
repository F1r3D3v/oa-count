#include "file.h"
#include "helper.h"
#include <stdlib.h>
#include <string.h>

//Get file extension, including the dot
char *getExtension(const char *path) {
	char *offset = NULL, *extension = NULL;
	size_t size;

	if ((offset = strrchr(path, '.')) == 0) return NULL;
	size = strlen(offset);
	extension = malloc(size + 1);
	if (extension)
		memcpy(extension, offset, size + 1);

	return extension;
}

// Get file basename from given path (with or without extension)
char *path_basename(const char *path, int extension) {
	char *offset = NULL, *extension_offset = NULL, *basename = NULL;
	size_t size;

	if ((offset = strrchr(path, (!strcmp(PLATFORM, "Windows")) ? '\\' : '/')) == 0)
		offset = (char *)path;
	else
		++offset;

	if (!extension && (extension_offset = strchr(offset, '.')) != 0)
		size = strlen(offset) - strlen(extension_offset);
	else
		size = strlen(offset);

	basename = malloc(size + 1);
	if (basename)
	{
		memcpy(basename, offset, size);
		basename[size] = '\0';
	}

	return basename;
}

// Get size of the file in bytes
size_t getFileSize(FILE *file) {
	if (file == NULL)
		return 0;

	fseek(file, 0, SEEK_END);
	long file_size = ftell(file);
	fseek(file, 0, SEEK_SET);

	return file_size;
}

// Get line from the file and move the file pointer to the next line, file must be opened in binary mode
// TODO: Complete Rework, only God knows what this code do
ssize_t getLine(char **line, size_t *size, BOOL *last_line, FILE *file) {
	if (file == NULL) return -1;

	char *temp = NULL;
	char last = 0, before = 0, curr = 0;
	static BOOL var = FALSE;
	long line_pos = ftell(file);
	BOOL first_line = (line_pos == 0 || var) ? TRUE : FALSE;
	static BOOL _last_line;
	*last_line = FALSE;
	size_t i;

	if (_last_line) {
		_last_line = FALSE;
		return -1;
	}

	for (i = 0; ((last != '\r' && last != '\n') || (!first_line && i == 3)) && !feof(file); ++i) {
		last = before;
		before = curr;
		curr = (char)fgetc(file);
	}

	var = (first_line && last == '\n' && i == 3);

	if (before == 0 && curr == EOF)
		*size = i - 1;
	else if ((last != '\r' && last != '\n') && (before == '\r' || before == '\n') && curr == EOF)
		*size = i - 3;
	else
		*size = i - ((first_line) ? 3 : 4);

	fseek(file, line_pos + ((first_line) ? 0 : 1), SEEK_SET);

	temp = realloc(*line, *size + 1);
	if (temp)
	{
		*line = temp;
		fread(*line, sizeof(char), *size, file);
		(*line)[*size] = '\0';
	}

	if (first_line && ((last == '\r' && before == '\n' && curr != EOF) || (last == '\n' && i == 3)))
		fseek(file, 1, SEEK_CUR);
	else if (!first_line)
		fseek(file, (last == '\r' && before == '\n' && curr != EOF) ? 1 : 0, SEEK_CUR);

	if (((before == '\r' || before == '\n') || (last == '\r' && before == '\n')) && curr == EOF)
		fseek(file, 0, SEEK_END);
	else if (curr == EOF)
		*last_line = _last_line = TRUE;

	return (ssize_t)(*size);
}