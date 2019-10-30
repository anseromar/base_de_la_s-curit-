/* GPLv2 (c) Airbus */
#include <debug.h>
#include <info.h>
#include <segmem.h>
#include <string.h>

#define NSEG 6 // number of segment descriptor
#define CINDEX 1 // code index
#define DINDEX 2 // data index

#define c0_sel gdt_krn_seg_sel(CINDEX)
#define d0_sel gdt_krn_seg_sel(DINDEX)

extern info_t *info;

seg_desc_t GDT[NSEG];

/*
*
*/
void add_dsc(seg_desc_t *GDT,uint64_t dpl, uint64_t type)
{
  GDT->raw     = 0;
  GDT->limit_1 = 0xffff;
  GDT->limit_2 = 0xf;
  GDT->dpl     = dpl;
  GDT->type    = type;
  GDT->d       = 1;
  GDT->g       = 1;
  GDT->s       = 1;
  GDT->p       = 1;
}
/*
*
*/
void add_dsc_overflow(seg_desc_t *GDT,uint64_t dpl, uint64_t type)
{
  GDT->raw     = 0;
  GDT->base_1  = 0x0000;
  GDT->base_2  = 0x60;
  GDT->base_3  = 0x00;
  GDT->limit_1 = 32 - 1;
//  GDT->limit_2 = 0x0;
  GDT->dpl     = dpl;
  GDT->type    = type;
  GDT->d       = 1;
  GDT->g       = 1;
  GDT->s       = 1;
  GDT->p       = 1;
}
/*
*
*/
void print_gdt()
{
  gdt_reg_t gdt_reg;
  int
    i,
    nseg;

  get_gdtr(gdt_reg);
  nseg=(gdt_reg.limit + 1) / sizeof(seg_desc_t);

  for (i = 0; i < nseg; i++) {
    seg_desc_t *seg_desc=&gdt_reg.desc[i];
    uint32_t base=seg_desc->base_3<<24 | seg_desc->base_2<<16 | seg_desc->base_1;
    uint32_t limit=seg_desc->limit_2<<16 | seg_desc->limit_1;
    debug("GDT[%d] = 0x%llx | base 0x%x | limit 0x%x | type 0x%x\n",
          i, seg_desc->raw, base, limit, seg_desc->type);
  }
  debug("--\n"); 
}

/*
*
*/
void init_gdt()
{
  gdt_reg_t gdt_reg;

  gdt_reg.limit=sizeof(GDT)-1;
  gdt_reg.desc=GDT;

  set_gdtr(gdt_reg);

  add_dsc(&GDT[CINDEX],0,SEG_DESC_CODE_XR);
  add_dsc(&GDT[DINDEX],0,SEG_DESC_DATA_RW);

  set_cs(c0_sel);
  set_ss(d0_sel);
  set_ds(d0_sel);
  set_es(d0_sel);
  set_fs(d0_sel);
  set_gs(d0_sel);
}

void overflow()
{
  add_dsc_overflow(&GDT[3],0,SEG_DESC_DATA_RW);
 
  char  src[64];
  char *dst = 0;
 
  memset(src,0xff,64);

  set_es(gdt_krn_seg_sel(3));

  //_memcpy8(dst,src,32); 
  _memcpy8(dst,src,64);
}

void tp()
{
  print_gdt();
  init_gdt();
  print_gdt();
  overflow();
  print_gdt();
}
