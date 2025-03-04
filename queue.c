#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *head = malloc(sizeof(struct list_head));
    if (!head)
        return NULL;

    INIT_LIST_HEAD(head);
    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (!head)
        return;

    struct list_head *node, *safe;
    list_for_each_safe (node, safe, head) {
        element_t *elem = list_entry(node, element_t, list);
        q_release_element(elem);
        // free(elem);
    }
    free(head);
}



/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    element_t *new_element_t = malloc(sizeof(element_t));
    if (!new_element_t)
        return false;

    new_element_t->value = strdup(s);  // need to free it when delete head
    if (!new_element_t->value) {
        free(new_element_t);
        return false;
    }

    list_add(&(new_element_t->list), head);

    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    element_t *new_element_t = malloc(sizeof(element_t));
    if (!new_element_t)
        return false;

    new_element_t->value = strdup(s);
    if (!new_element_t->value) {
        free(new_element_t);
        return false;
    }

    list_add_tail(&(new_element_t->list), head);

    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;
    struct list_head *node = head->next;
    element_t *elem = list_entry(node, element_t, list);

    if (sp && bufsize > 0) {
        strncpy(sp, elem->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }

    list_del(node);
    return elem;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;
    struct list_head *node = head->prev;
    element_t *elem = list_entry(node, element_t, list);

    if (sp && bufsize > 0) {
        strncpy(sp, elem->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }

    list_del(node);
    return elem;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    int len = 0;
    struct list_head *node;
    list_for_each (node, head)
        len++;
    return len;
}


struct list_head *getmid(struct list_head *head)
{
    struct list_head *forward, *backward = head->prev;
    list_for_each (forward, head) {
        if ((forward->next == backward) || (forward == backward)) {
            break;
        }
        backward = backward->prev;
    }
    return forward;
}

struct list_head *merge_two_lists(struct list_head *left,
                                  struct list_head *right,
                                  bool descend)
{
    if (list_empty(left))
        return right;
    if (list_empty(right))
        return left;

    LIST_HEAD(temphead);
    while (!list_empty(left) && !list_empty(right)) {
        element_t *l = list_first_entry(left, element_t, list);
        element_t *r = list_first_entry(right, element_t, list);
        if ((descend && strcmp(l->value, r->value) > 0) ||
            (!descend && strcmp(l->value, r->value) <= 0))
            list_move_tail(&l->list, &temphead);
        else
            list_move_tail(&r->list, &temphead);
    }
    list_splice_tail_init(left, &temphead);
    list_splice_tail_init(right, &temphead);
    list_splice(&temphead, left);
    return left;
}

// https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
// {
//     struct list_head *fast = head;
//     struct list_head *slow;
//     list_for_each (slow, head) {
//         fast = fast->next->next;
//         if ((fast->next == head) || (fast == head)) {
//             break;
//         }
//     }
//     list_del(slow);
//     element_t *ele_deleted = list_entry(slow, element_t, list);
//     q_release_element(ele_deleted);

//     return true;
// }
{
    struct list_head *mid = getmid(head);
    list_del(mid);
    q_release_element(list_entry(mid, element_t, list));

    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return true;
    struct list_head *prev = head;
    struct list_head *cur = head->next;
    struct list_head *tmp;
    struct list_head *curnext;

    while (cur != head) {
        if (cur->next != head &&
            strcmp(list_entry(cur, element_t, list)->value,
                   list_entry(cur->next, element_t, list)->value) == 0) {
            while (cur->next != head &&
                   strcmp(list_entry(cur, element_t, list)->value,
                          list_entry(cur->next, element_t, list)->value) == 0) {
                tmp = cur;
                cur = cur->next;
                element_t *elem = list_entry(tmp, element_t, list);
                q_release_element(elem);
            }
            cur->next->prev = prev;
            prev->next = cur->next;
            curnext = cur->next;
            element_t *elem_cur = list_entry(cur, element_t, list);
            q_release_element(elem_cur);
            cur = curnext;

        } else {
            prev = cur;
            cur = cur->next;
        }
    }

    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
// {
//     if (head->next == head || head->next->next == head)
//         return;

//     struct list_head *newHead = head->next->next;
//     struct list_head *cur = head->next;
//     struct list_head *prev = head;

//     while (cur != head && cur->next != head) {
//         struct list_head *nextPair = cur->next->next;
//         struct list_head *second = cur->next;

//         second->next = cur;
//         cur->prev = second;
//         cur->next = nextPair;
//         nextPair->prev = cur;

//         prev->next = second;
//         second->prev = prev;

//         prev = cur;
//         cur = nextPair;
//     }
//     head->next = newHead;
//     newHead->prev = head;
// }
/*treat swap sa special case when K = 2*/
// {
//     q_reverseK(head, 2);
// }
// {
//     if (list_empty(head) || list_is_singular(head))
//         return;

//     struct list_head *cur = head->next;
//     struct list_head *nextpair;
//     while (cur != head && cur->next != head) {
//         nextpair = cur->next->next;

//         // 交換節點 cur 和 cur->next
//         cur->prev->next = cur->next;
//         cur->next->prev = cur->prev;

//         cur->next->next = cur;
//         cur->prev = cur->next;

//         cur->next = nextpair;
//         nextpair->prev = cur;

//         cur = nextpair;
//     }
// }
{
    if (list_empty(head) || list_is_singular(head))
        return;

    struct list_head *cur = head->next, *nextpair;


    while (cur != head && cur->next != head) {
        nextpair = cur->next->next;
        list_move(cur, cur->next);
        cur = nextpair;
    }
}



/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    struct list_head *cur = head, *nextnode;
    do {
        nextnode = cur->next;
        cur->next = cur->prev;
        cur->prev = nextnode;
        cur = nextnode;
    } while (cur != head);
}
// {
//     struct list_head *node;
//     struct list_head *prev = head;
//     list_for_each (node, head) {
//         prev->next = prev->prev;
//         prev->prev = node;
//         prev = node;
//     }
//     prev->next = prev->prev;
//     prev->prev = head;
// }

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
// {
//     // https://leetcode.com/problems/reverse-nodes-in-k-group/

//     if (!head || head->next->next == head || k == 1) return;


//     struct list_head* prev= head;
//     struct list_head* cur = head->next;

//     int count = q_size(head);

//     while (count >= k) {
//         cur = prev->next;
//         struct list_head* next = cur->next;

//         // reverse k nodes
//         // move last node to first each round
//         for (int i=0; i<k-1; i++){
//             cur->next = next->next;
//             next->next->prev = cur;

//             next->next = prev->next;
//             prev->next->prev = next;

//             prev->next = next;
//             next->prev = prev;
//             next = cur->next;
//         }

//         prev = cur;
//         count -= k;
//     }

// }
{
    if (list_empty(head) || list_is_singular(head) || k == 1)
        return;

    struct list_head *prev = head, *cur;
    int count = q_size(head);

    while (count >= k) {
        cur = prev->next;
        struct list_head *next = cur->next;

        /*
         * Reverse k nodes by moving the i-th node
         * to the front of the k-group each round.
         */
        for (int i = 1; i < k; i++) {
            list_move(next, prev);
            next = cur->next;
        }

        prev = cur;
        count -= k;
    }
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend)
{
    if (list_empty(head) || list_is_singular(head))
        return;

    struct list_head *mid = getmid(head);
    if (!mid)
        return;

    LIST_HEAD(lefthead);
    list_cut_position(&lefthead, head, mid);

    q_sort(&lefthead, descend);
    q_sort(head, descend);
    // struct list_head *merged = merge_two_lists(lh, head, descend);
    // split to two queue

    struct list_head *merged = merge_two_lists(&lefthead, head, descend);
    // head = merged;
    INIT_LIST_HEAD(head);
    list_splice_tail(merged, head);
}
// use q_merge() to sort
// {
//     struct list_head *node;
//     list_for_each(node, head) {
//         queue_contex_t *newcon = malloc(sizeof(queue_contex_t));

//     }


// }



/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
