#include <string.h>
#include <stdlib.h>

char* longestPalindrome(char* s) {
    int n = strlen(s);

    if (n <= 1)
        return strdup(s);

    int start = 0;
    int maxLen = 1;

    for (int i = 0; i < n; i++) {

        // Odd-length palindrome: center at i
        int left = i;
        int right = i;

        while (left >= 0 && right < n && s[left] == s[right]) {
            if (right - left + 1 > maxLen) {
                start = left;
                maxLen = right - left + 1;
            }

            left--;
            right++;
        }

        // Even-length palindrome: center between i and i+1
        left = i;
        right = i + 1;

        while (left >= 0 && right < n && s[left] == s[right]) {
            if (right - left + 1 > maxLen) {
                start = left;
                maxLen = right - left + 1;
            }

            left--;
            right++;
        }
    }

    char* result = malloc((maxLen + 1) * sizeof(char));

    strncpy(result, s + start, maxLen);
    result[maxLen] = '\0';

    return result;
}
