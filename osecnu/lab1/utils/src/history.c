#include "../include/history.h"

int phistory(int n) {
    if (n == 0) {
        printf("Usage(history): history n(n > 0).\n");
        return -1;
    } else if (n > kMaxHistory) {
        fprintf(stderr, "history: n=%d is too large!\n", n);
        return -1;
    }

    FILE* shfp = fopen(HISTORY_PATH, "r");
    if (shfp) {
        char lines[kMaxHistory][kMaxTokenLen];
        int i = 0;
        while (fgets(lines[i % kMaxHistory], kMaxTokenLen, shfp) != NULL) {
            i++;
        }

        // Exclude the last line "history n"
#ifdef SHDEBUG
        for (int _ = 0; _ < i; _++) {
            printf("history read i = %d, %s", _, lines[_]);
        }
#endif
        int j;
        for (j = 1; j <= n; j++) {
            printf("%s", lines[i - j % kMaxHistory]);
        }
        fclose(shfp);

        return 0;
    } else {
        printf("No history file, DO NOT DELETE IT MANUALLY!\n");
        return -1;
    }
}