#ifndef EMIC_UTIL_H
#define EMIC_UTIL_H
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
#endif /* EMIC_UTIL_H */
