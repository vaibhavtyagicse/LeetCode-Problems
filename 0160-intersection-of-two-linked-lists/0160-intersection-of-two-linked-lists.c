/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */
struct ListNode *getIntersectionNode(struct ListNode *headA, struct ListNode *headB) {
   struct ListNode *a,*b;
   a=headA;
   b=headB;
   while(a!=b){
    if(a!=NULL){
        a=a->next;
    }else{
        a=headB;
    }
    if(b!=NULL){
        b=b->next;
    }else{
        b=headA;
    }
   }
   return a;
}