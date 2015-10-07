#ifndef __AIPCV_TYPE_H__
#define __AIPCV_TYPE_H__

#include <stdint.h>

typedef struct aipcv_hdr
{
  uint32_t nr;                          /*!> Number of Rows */
  uint32_t nc;                          /*!> Number of Columns */

  uint32_t oi;                          /*!> Origin i */
  uint32_t oj;                          /*!> Origin j */
} aipcv_hdr_t;

typedef struct aipcv_image
{
  aipcv_hdr_t *info;                    /*!> Pointer to AIPCV image header */
  uint8_t     **data;                   /*!> Pixel values (8-bit RAW Image) */
} aipcv_image_t, *aipcv_image_t_ptr;    /*!> Assign pointer name to this structure */

#endif
