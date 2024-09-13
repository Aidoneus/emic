#include <string.h>
#include <time.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include "util.h"

FILE* logFile;
const char* LOG_PATH = "emic.log";
uint32_t seed = 1;

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
		!strcmp(GITHUB_BUILT, "1") ? "on GitHub" : "locally"
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
	fprintf(logFile, "\n%lu ", ts == (time_t)(-1) ? 0 : (long int)ts);
	va_list args;
	va_start(args, format);
	vfprintf(logFile, format, args);
	va_end(args);
}
int uWfl(char* a, char* b) {
	int m = strlen(a), n = strlen(b), r1 = 0, r2 = 1, i, j;
	int d[2][n + 1];
	int c[3];
	for (j = 0; j <= n; j++) d[1][j] = j;
	for (i = 1; i <= m; i++) {
		r1 = !r1;
		r2 = !r2;
		d[r2][0] = i;
		for (j = 1; j <= n; j++) {
			c[0] = d[r1][j] + 1; // Deletion cost
			c[1] = d[r2][j - 1] + 1; // Insertion cost
			c[2] = d[r1][j - 1] + !!(a[i - 1] - b[j - 1]); // Substitution cost
			d[r2][j] = c[0] < c[1] ? ((c[0] < c[2]) ? c[0] : c[2]) : ((c[1] < c[2]) ? c[1] : c[2]);
		}
	}
	return d[r2][n];
}
void uPrintb(void* p, size_t l) {
	uint8_t* b = p;
	uint8_t j;
	for (size_t i = 0; i < l; i++) {
		for (j = 0; j < 8; j++) printf("%u", (*(b + i) >> j) & 1);
		if (i != (l - 1)) printf(" ");
	}
}
uint32_t uRnd() {
	uint32_t h, l;
	l = 16807 * (seed & 0xFFFF);
	h = 16807 * (seed >> 16);
	l += (h & 0x7FFF) << 16;
	l += h >> 15;
	if (l > 0x7FFFFFFF) l -= 0x7FFFFFFF;
	return (seed = (int32_t)l);
}
