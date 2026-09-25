int findKthLargest(int* nums, int numsSize, int k) {

    int count[20001] = {0};

    // nums[i] is between -10000 and 10000
    for (int i = 0; i < numsSize; i++) {
        count[nums[i] + 10000]++;
    }

    // Start from the largest possible value
    for (int value = 10000; value >= -10000; value--) {

        k -= count[value + 10000];

        if (k <= 0) {
            return value;
        }
    }

    return 0;
}
