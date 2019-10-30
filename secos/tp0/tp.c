/* GPLv2 (c) Airbus */
#include <debug.h>
#include <info.h>

extern info_t   *info;
extern uint32_t __kernel_start__;
extern uint32_t __kernel_end__;

void tp() {
   debug("kernel mem [0x%x - 0x%x]\n", &__kernel_start__, &__kernel_end__);
   debug("MBI flags 0x%x\n", info->mbi->flags);

   multiboot_uint32_t debut = info->mbi->mmap_addr;
   multiboot_uint32_t fin = debut + info->mbi->mmap_length;

   while( debut < fin ){
     debug("0x%llx - 0x%llx (%d)\n",((memory_map_t*)debut)->addr, \
     ((memory_map_t*)debut)->addr + ((memory_map_t*)debut)->len, \
     ((memory_map_t*)debut)->type);


     debut = debut + (sizeof(memory_map_t));
   }

}
