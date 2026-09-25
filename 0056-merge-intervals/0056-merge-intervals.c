int compare(const void* a, const void* b) {
    int* intervalA = *(int**)a;
    int* intervalB = *(int**)b;

    return intervalA[0] - intervalB[0];
}

/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned in *returnColumnSizes.
 * Note: Both returned array and returnedColumnSizes must be malloced,
 * assume caller calls free().
 */
int** merge(int** intervals, int intervalsSize, int* intervalsColSize,
            int* returnSize, int** returnColumnSizes) {

    if (intervalsSize == 0) {
        *returnSize = 0;
        *returnColumnSizes = NULL;
        return NULL;
    }

    // Sort intervals by start time
    qsort(intervals, intervalsSize, sizeof(int*), compare);

    int** result = (int**)malloc(intervalsSize * sizeof(int*));
    *returnColumnSizes = (int*)malloc(intervalsSize * sizeof(int));

    int count = 0;

    // Start with the first interval
    result[0] = (int*)malloc(2 * sizeof(int));
    result[0][0] = intervals[0][0];
    result[0][1] = intervals[0][1];

    (*returnColumnSizes)[0] = 2;
    count = 1;

    for (int i = 1; i < intervalsSize; i++) {

        // Current interval overlaps with the last merged interval
        if (intervals[i][0] <= result[count - 1][1]) {

            if (intervals[i][1] > result[count - 1][1]) {
                result[count - 1][1] = intervals[i][1];
            }

        } else {
            // No overlap, add a new interval
            result[count] = (int*)malloc(2 * sizeof(int));

            result[count][0] = intervals[i][0];
            result[count][1] = intervals[i][1];

            (*returnColumnSizes)[count] = 2;
            count++;
        }
    }

    *returnSize = count;

    return result;
}
