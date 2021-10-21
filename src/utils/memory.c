#include "memory.h"

typedef struct memory_stats {
    u64 allocated_size[M_NUM];
    u64 freed_size[M_NUM];
} memory_stats;

static memory_stats stats;
static const char* tags[M_NUM] = { "STRING", "MANAGED_PTR", "SHADER", "VAO", "TEXTURE", "MESH", "CAMERA", "UNUSED" };

void* bmalloc(u64 size, u16 type) {
    stats.allocated_size[type] += size;
    void* data = malloc(size);
    if (!data) {
        bfatal("Unable to allocate data for type %s.", tags[type]);
        exit(EXIT_FAILURE);
    }
    return data;
}

void bfree(void* mem, u64 size, u16 type) {
    stats.freed_size[type] += size;
    free(mem);
    mem = NULL;
}

void print_memory_stats() {
    for (u32 i = 0; i < M_NUM; ++i) {
        bdebug("[%s]: allocated: %llu, freed: %llu, memory: %llu",
            tags[i], 
            stats.allocated_size[i], 
            stats.freed_size[i], 
            stats.allocated_size[i] - stats.freed_size[i]
        );
    }
    printf("\n");
}

void managed_ptr_free(managed_ptr_t ptr) {
    if (ptr.deallocator != NULL) {
        ptr.deallocator(ptr.mem);
    } else {
        bfree(ptr.mem, ptr.size, M_MANAGED_PTR);
        ptr.mem = NULL;
    }
}
