#include <stivale2.h>
#include <stddef.h>
#include <mm/vmm.h>

static uint8_t stack[4096];

struct stivale2_header_tag_smp smp_hdr_tag = {
	.tag = {
		.identifier = STIVALE2_HEADER_TAG_SMP_ID,
		.next = 0
	},
	.flags = 0,
};

struct stivale2_header_tag_framebuffer framebuffer_hdr_tag = {
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_FRAMEBUFFER_ID,
        .next = (uint64_t)&smp_hdr_tag
    },
    .framebuffer_width  = 0,
    .framebuffer_height = 0,
    .framebuffer_bpp    = 0
};

__attribute__((section(".stivale2hdr"), used))
struct stivale2_header stivale_hdr = {
    .entry_point = 0,
    .stack = (uintptr_t)stack + sizeof(stack),
    .flags = 0,
    .tags = (uint64_t) &framebuffer_hdr_tag
};

void *get_stivale2_tag(struct stivale2_struct *stivale2_struct, uint64_t id) {
    for (struct stivale2_tag* curr_tag = (void*)stivale2_struct->tags + VM_OFFSET; curr_tag; curr_tag = (void*)curr_tag->next + VM_OFFSET) {
        if (curr_tag->identifier == id) {
            return curr_tag;
        }
    }
    return 0;
}
