#include <string.h>
#include <time.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include "util.h"

FILE* logFile;
const char* LOG_PATH = "emic.log";

int uGetopt(int argc, char* argv[], int optc, const opt_config* optv, int* argIdx, int* optIdx) {
	int i = 0;
	*optIdx = -1;
	(*argIdx)++;
	if ((*argIdx) >= argc) return *optIdx;
	if (argv[*argIdx][0] != '-') return *optIdx;
	if (argv[*argIdx][1] == '-') for (; i < optc; i++) {
		if (optv[i].long_name == NULL) continue;
		if (!strcmp(optv[i].long_name, &argv[*argIdx][2])) {
			*optIdx = i;
			break;
		}
	} else for (; i < optc; i++) if (optv[i].short_name == argv[*argIdx][1]) {
		*optIdx = i;
		break;
	}
	return *optIdx;
}
int uLogOpen() {
	logFile = fopen(LOG_PATH, "a");
	if (logFile == NULL) return 1;
	uLog(
		"%s->%s (built %s)",
		GIT_BRANCH,
		GIT_COMMIT_HASH,
		GITHUB_BUILT ? "on GitHub" : "locally"
	);
	return 0;
}
void uLogClose() {
	if (logFile == NULL) return;
	uLog("Closing log file");
	// Not checking for possible flushing and/or closing failure as we won't be
	// able to do anything about that here
	fflush(logFile);
	fclose(logFile);
}
void uLog(const char* format, ...) {
	if (logFile == NULL) return;
	time_t ts = time(NULL);
	fprintf(logFile, "\n%jd ", ts == (time_t)(-1) ? 0 : (intmax_t)gmtime(&ts));
	va_list args;
	va_start(args, format);
	vfprintf(logFile, format, args);
	va_end(args);
}
