#ifndef _MQ_H
#define _MQ_H

#include <stdint.h>
#include "link_list.h"

typedef struct mq *mq_t;
typedef void (*item_destroyer)(void*);
void default_mq_item_destroyer(void* item);

#define MQ_DEFAULT_ITEM_DESTROYER default_mq_item_destroyer

mq_t create_mq(uint32_t,item_destroyer);
void destroy_mq(mq_t*);
void mq_push(mq_t,struct list_node*);
void mq_push_now(mq_t,struct list_node*);
void mq_push_list(mq_t,struct link_list *,uint32_t timeout);
struct list_node* mq_pop(mq_t,uint32_t timeout);

void  init_mq_system();
void  mq_flush();
#endif
