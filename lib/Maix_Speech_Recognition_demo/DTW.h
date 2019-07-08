#ifndef _DTW_H
#define _DTW_H
#include "g_def.h"

#define dis_err	0xFFFFFFFF
#define dis_max	0xFFFFFFFF

#ifdef __cplusplus
extern "C" {
#endif


u32 dtw(v_ftr_tag *ftr_in, v_ftr_tag *frt_mdl);
u32 get_mdl(v_ftr_tag *ftr_in1, v_ftr_tag *ftr_in2, v_ftr_tag *ftr_mdl);

#ifdef __cplusplus
}
#endif

#endif
