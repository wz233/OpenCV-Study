#ifndef __AIPCV_COMMON_H__
#define __AIPCV_COMMON_H__

#include "aipcv_type.h"

#include <cv.h>
#include <highgui.h>

#define __AIPCV_DEBUG    1

#if (__AIPCV_DEBUG == 1)
#include <stdio.h>
#endif

aipcv_image_t_ptr aipcv_new_image   (uint32_t nr, uint32_t nc);
void              aipcv_free_image  (aipcv_image_t *z);

IplImage         *aipcv_to_OpenCV   (aipcv_image_t_ptr x);
aipcv_image_t_ptr aipcv_from_OpenCV (IplImage *x);

#endif
