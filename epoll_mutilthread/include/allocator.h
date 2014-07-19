#ifndef _ALLOCATOR_H
#define _ALLOCATOR_H
#include <stdint.h>

typedef struct allocator {
    void* (*_alloc)(struct allocator*,int32_t);
    void (*_dealloc)(struct allocator*,void*);
    void (*_destroy)(struct allocator**);
}*allocator_t;

#ifndef IMPLEMEMT
#define IMPLEMEMT(SUPER_CLASS) struct SUPER_CLASS super_class
#endif

#ifndef ALLOC
#define ALLOC(ALLOCATOR,SIZE)\
   ({ void *__result;\
       do \
	   if(ALLOCATOR)\
	     __result = ((struct allocator*)ALLOCATOR)->_alloc(ALLOCATOR,SIZE);\
	   else\
	     __result = calloc(1,SIZE);\
	   while(0);\
       __result;})
#endif

#ifndef FREE
#define FREE(ALLOCATOR,PTR)\
   ({\
       do \
	   if(ALLOCATOR)\
	     ((struct allocator*)ALLOCATOR)->_dealloc(ALLOCATOR,PTR);\
	   else\
	     free(PTR);\
	   while(0);\
		})
#endif

#ifndef DESTROY
#define DESTROY(ALLOCATOR)\
	((struct allocator*)(*(ALLOCATOR)))->_destroy((ALLOCATOR))
#endif

#endif
