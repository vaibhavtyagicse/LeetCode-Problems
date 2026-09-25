void merge(int* nums, int* temp, int left, int mid, int right) {

    int i = left;
    int j = mid + 1;
    int k = left;

    // Merge two sorted halves
    while (i <= mid && j <= right) {

        if (nums[i] <= nums[j]) {
            temp[k++] = nums[i++];
        } else {
            temp[k++] = nums[j++];
        }
    }

    // Remaining elements from left half
    while (i <= mid) {
        temp[k++] = nums[i++];
    }

    // Remaining elements from right half
    while (j <= right) {
        temp[k++] = nums[j++];
    }

    // Copy back into nums
    for (i = left; i <= right; i++) {
        nums[i] = temp[i];
    }
}

void mergeSort(int* nums, int* temp, int left, int right) {

    if (left >= right) {
        return;
    }

    int mid = left + (right - left) / 2;

    mergeSort(nums, temp, left, mid);
    mergeSort(nums, temp, mid + 1, right);

    merge(nums, temp, left, mid, right);
}

int* sortArray(int* nums, int numsSize, int* returnSize) {

    *returnSize = numsSize;

    if (numsSize <= 1) {
        return nums;
    }

    int* temp = (int*)malloc(numsSize * sizeof(int));

    mergeSort(nums, temp, 0, numsSize - 1);

    free(temp);

    return nums;
}
