#ifndef __FLASH_H
#define __FLASH_H

#include "g_def.h"
#include "MFCC.h"

#define FLASH_PAGE_SIZE			2048

#define Flash_Fail		3
#define Flash_Success	0

#define save_mask		12345

#define size_per_ftr	(4*1024)
#define page_per_ftr	(size_per_ftr/FLASH_PAGE_SIZE)
#define ftr_per_comm	4
#define size_per_comm	(ftr_per_comm*size_per_ftr)
#define comm_num		10
#define ftr_total_size	(size_per_comm*comm_num)
//#define ftr_end_addr	0x8080000
#define ftr_end_addr (size_per_ftr * ftr_per_comm * comm_num)
#define ftr_start_addr	0 //(ftr_end_addr-ftr_total_size)

#ifdef __cplusplus
extern "C" {
#endif


u8 save_ftr_mdl(v_ftr_tag *ftr, u32 addr);

//void init_voice_mdl(void);
extern v_ftr_tag ftr_save[20 * 4];

#ifdef __cplusplus
}
#endif

#endif

