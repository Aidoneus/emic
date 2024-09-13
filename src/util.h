#ifndef EMIC_UTIL_H
#define EMIC_UTIL_H
#include <stdarg.h>
typedef struct OptionConfig {
	char short_name; // Can be '\0' if not needed
	char* long_name; // Can be NULL if not needed
} opt_config;
/**
 * @brief Get an option from argc+argv
 * @param argc
 * @param argv
 * @param optc Possible options array length
 * @param optv Possible options array
 * @param argIdx Index of argument from argv
 * @param optIdx Index of option from opts
 * @return -1 if invalid argument was encountered, optIdx otherwise
 */
int uGetopt(int argc, char* argv[], int optc, const opt_config* optv, int* argIdx, int* optIdx);
/**
 * @brief Prepare to work with the log file
 * @return 0 on success, non-zero value otherwise
 */
int uLogOpen();
/**
 * @brief Finish working with the log file
 */
void uLogClose();
/**
 * @brief Write a string to the log file
 * @param format Format string to write
 * @param ... Possible data to print
 */
void uLog(const char* format, ...);
#endif /* EMIC_UTIL_H */
