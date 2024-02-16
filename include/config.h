#define MAX_ARG_LENGTH 40
#define MAX_RESULT_LENGTH 80
#define MIN_NUMERAL_SYSTEM 2
#define MAX_NUMERAL_SYSTEM 16
#define MAX_TIME_LIMIT_SECONDS 5

static const char system_representation[MAX_NUMERAL_SYSTEM - MIN_NUMERAL_SYSTEM + 2] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
																						'A', 'B', 'C', 'D', 'E', 'F'};