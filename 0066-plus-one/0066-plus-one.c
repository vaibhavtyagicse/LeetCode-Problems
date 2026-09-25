int* plusOne(int* digits, int digitsSize, int* returnSize){
    int* ans, i;
    for(i = digitsSize - 1; i >= 0; i--){
        if(digits[i] < 9){
            digits[i]++;
            *returnSize = digitsSize;
            return digits;
        }
        digits[i] = 0;
    }
    
    *returnSize = digitsSize + 1;
    ans = (int*)malloc((*returnSize) * sizeof(int));
    
    ans[0] = 1;
    for(i = 1; i < *returnSize; i++){
        ans[i] = 0;
    }
    
    return ans;
}
