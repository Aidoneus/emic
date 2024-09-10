#include <string.h>
#include "util.h"

int u_getopt(int argc, char* argv[], int optc, const opt_config* optv, int* argIdx, int* optIdx) {
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
