int* productExceptSelf(int* nums, int numsSize, int* returnSize) {
    *returnSize = numsSize;

    int* answer = (int*)malloc(numsSize * sizeof(int));

    // Store product of all elements to the left
    answer[0] = 1;

    for (int i = 1; i < numsSize; i++) {
        answer[i] = answer[i - 1] * nums[i - 1];
    }

    // Multiply by product of all elements to the right
    int right = 1;

    for (int i = numsSize - 1; i >= 0; i--) {
        answer[i] = answer[i] * right;
        right = right * nums[i];
    }

    return answer;
}
