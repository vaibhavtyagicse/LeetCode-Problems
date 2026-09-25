void mergeSort(int* nums, int* indices, int* tempIndices,
               int* count, int left, int right) {

    if (left >= right) {
        return;
    }

    int mid = left + (right - left) / 2;

    mergeSort(nums, indices, tempIndices, count, left, mid);
    mergeSort(nums, indices, tempIndices, count, mid + 1, right);

    int i = left;
    int j = mid + 1;
    int k = left;

    int rightSmaller = 0;

    while (i <= mid && j <= right) {

        if (nums[indices[j]] < nums[indices[i]]) {

            // This right element is smaller than nums[i]
            rightSmaller++;

            tempIndices[k++] = indices[j++];
        }
        else {

            // All previously taken right elements are smaller
            count[indices[i]] += rightSmaller;

            tempIndices[k++] = indices[i++];
        }
    }

    // Remaining left elements
    while (i <= mid) {
        count[indices[i]] += rightSmaller;
        tempIndices[k++] = indices[i++];
    }

    // Remaining right elements
    while (j <= right) {
        tempIndices[k++] = indices[j++];
    }

    // Copy sorted indices back
    for (i = left; i <= right; i++) {
        indices[i] = tempIndices[i];
    }
}

int* countSmaller(int* nums, int numsSize, int* returnSize) {

    *returnSize = numsSize;

    int* result = (int*)calloc(numsSize, sizeof(int));

    if (numsSize == 0) {
        return result;
    }

    // Store original indices
    int* indices = (int*)malloc(numsSize * sizeof(int));
    int* tempIndices = (int*)malloc(numsSize * sizeof(int));

    for (int i = 0; i < numsSize; i++) {
        indices[i] = i;
    }

    mergeSort(nums, indices, tempIndices,
              result, 0, numsSize - 1);

    free(indices);
    free(tempIndices);

    return result;
}
