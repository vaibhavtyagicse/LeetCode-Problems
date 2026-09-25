int firstMissingPositive(int* nums, int numsSize) {

    // Put each number in its correct position
    for (int i = 0; i < numsSize; i++) {

        while (nums[i] >= 1 &&
               nums[i] <= numsSize &&
               nums[nums[i] - 1] != nums[i]) {

            int temp = nums[i];
            nums[i] = nums[temp - 1];
            nums[temp - 1] = temp;
        }
    }

    // Find the first position that doesn't contain i + 1
    for (int i = 0; i < numsSize; i++) {
        if (nums[i] != i + 1) {
            return i + 1;
        }
    }

    // All numbers from 1 to numsSize exist
    return numsSize + 1;
}
