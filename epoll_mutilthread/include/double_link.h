#ifndef _DOUBLE_LINK_H
#define _DOUBLE_LINK_H


struct double_link_node {
    struct double_link_node *pre;
    struct double_link_node *next;
};

struct double_link {
    struct double_link_node head;
    struct double_link_node tail;
};

static inline int32_t double_link_empty(struct double_link *dl) {
    return dl->head.next == &dl->tail ? 1:0;
}

static inline struct double_link_node *double_link_first(struct double_link *dl) {
    if(double_link_empty(dl))
        return NULL;
    return dl->head.next;
}

static inline struct double_link_node *double_link_last(struct double_link *dl) {
    if(double_link_empty(dl))
        return NULL;
    return dl->tail.pre;
}



static inline int32_t double_link_remove(struct double_link_node *dln) {
    if(!dln->pre || !dln->next)
        return -1;
    dln->pre->next = dln->next;
    dln->next->pre = dln->pre;
    dln->pre = dln->next = NULL;
    return 0;
}

static inline struct double_link_node *double_link_pop(struct double_link *dl) {
    if(double_link_empty(dl))
        return NULL;
    else {
        struct double_link_node *n = dl->head.next;
        double_link_remove(n);
        return n;
    }
}

static inline int32_t double_link_push(struct double_link *dl,struct double_link_node *dln) {
    if(dln->pre || dln->next)
        return -1;
    dl->tail.pre->next = dln;
    dln->pre = dl->tail.pre;
    dl->tail.pre = dln;
    dln->next = &dl->tail;
    return 0;
}

static inline void double_link_clear(struct double_link *dl) {
    dl->head.pre = dl->tail.next = NULL;
    dl->head.next = &dl->tail;
    dl->tail.pre = &dl->head;
}

#endif
