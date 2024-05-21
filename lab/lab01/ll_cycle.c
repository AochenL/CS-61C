#include <stddef.h>
#include "ll_cycle.h"

int ll_has_cycle(node *head) {
    if (head == NULL || head->next == NULL) {
        return 0;  // No cycle if the list is empty or has only one node
    }

    node *slow = head;   // Slow pointer moves one node at a time
    node *fast = head;   // Fast pointer moves two nodes at a time

    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;          // Move slow pointer one step
        fast = fast->next->next;    // Move fast pointer two steps

        if (slow == fast) {
            return 1;  // Cycle detected if slow and fast pointers meet
        }
    }

    return 0;  // No cycle found
}