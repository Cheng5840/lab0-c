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
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head) {}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
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
