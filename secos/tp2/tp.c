/* GPLv2 (c) Airbus */
#include <debug.h>
#include <info.h>
#include <intr.h>

extern info_t *info;

void loop_inter(){
    while(1)
    asm volatile("sti");
}
void bp_handler()
{
  debug("Debug message\n");
}

void bp_trigger()
{
  asm volatile("int3");
}

void tp()
{
//  loop_inter();
    idt_reg_t idt;
    get_idtr(idt);

    debug("Loading address: 0x%llx\n",idt.desc);

    int_desc_t *int_desc;
    int_desc = &idt.desc[3];

    void (*bp_hand)() = &bp_handler;
    debug("0x%llx\n", bp_hand);

    int_desc->offset_1 = ((uint16_t)((uint32_t)bp_hand));
    int_desc->offset_2 = (uint16_t)(((uint32_t)bp_hand)>>16);
    debug("0x%llx\n",(int_desc->offset_2 << 16 | int_desc->offset_1));
    bp_trigger();
}
