double findMedianSortedArrays(int* nums1, int nums1Size,
                              int* nums2, int nums2Size) {

    // Always binary search on the smaller array
    if (nums1Size > nums2Size) {
        return findMedianSortedArrays(
            nums2, nums2Size,
            nums1, nums1Size
        );
    }

    int m = nums1Size;
    int n = nums2Size;

    int left = 0;
    int right = m;

    int totalLeft = (m + n + 1) / 2;

    while (left <= right) {

        // Partition nums1
        int partition1 = left + (right - left) / 2;

        // Partition nums2
        int partition2 = totalLeft - partition1;

        // Elements immediately around the partitions
        int left1 = (partition1 == 0) ? INT_MIN : nums1[partition1 - 1];
        int right1 = (partition1 == m) ? INT_MAX : nums1[partition1];

        int left2 = (partition2 == 0) ? INT_MIN : nums2[partition2 - 1];
        int right2 = (partition2 == n) ? INT_MAX : nums2[partition2];

        // Correct partition
        if (left1 <= right2 && left2 <= right1) {

            // Odd total number of elements
            if ((m + n) % 2 == 1) {
                return (double)((left1 > left2) ? left1 : left2);
            }

            // Even total number of elements
            int maxLeft = (left1 > left2) ? left1 : left2;
            int minRight = (right1 < right2) ? right1 : right2;

            return ((double)maxLeft + (double)minRight) / 2.0;
        }

        // nums1 partition is too far right
        if (left1 > right2) {
            right = partition1 - 1;
        }
        // nums1 partition is too far left
        else {
            left = partition1 + 1;
        }
    }

    return 0.0;
}
