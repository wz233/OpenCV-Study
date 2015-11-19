#include "aipcv_common.h"

#include <malloc.h>
#include <fcntl.h>
#include <io.h>

aipcv_image_t_ptr aipcv_new_image(uint32_t nr, uint32_t nc) {
  aipcv_image_t_ptr x;
  uint32_t i;

  if (nr < 0 || nc < 0) {
#if (__AIPCV_DEBUG == 1)
    printf("Error: Bad Image Size (%d:%d)\n", nr, nc);
#endif
    return 0;
  }

  x = (aipcv_image_t *)malloc(sizeof(aipcv_image_t));

  if (!x) {
#if (__AIPCV_DEBUG == 1)
    printf("Out of storage in creating new AIPCV Image!\n");
#endif    
    return 0;
  }

  x->info = (aipcv_hdr_t *)malloc(sizeof(aipcv_hdr_t));
  if (!(x->info)) {
#if (__AIPCV_DEBUG == 1)
    printf("Out of storage in creating new AIPCV Image!\n");
#endif    
    return 0;
  }

  x->info->nr = nr;
  x->info->nc = nc;
  x->info->oi = 0;
  x->info->oj = 0;

  x->data = (uint8_t **)malloc(sizeof(uint8_t*)*nr);
  if (!(x->data)) {
#if (__AIPCV_DEBUG == 1)
    printf("Out of storage in creating new AIPCV Image!\n");
#endif    
    return 0;
  }

  x->data[0] = (uint8_t *)malloc(nr * nc);
  if (x->data[0] == 0) {
#if (__AIPCV_DEBUG == 1)
    printf("Out of storage in creating new AIPCV Image!\n");
#endif    
    return 0;
  }

#if (1)
  for (i = 1; i < nr; i++) {
    x->data[i] = (x->data[i - 1] + nc);   /*!> Update ROW pointers */
  }
#endif

  return (x);
}

void aipcv_free_image(aipcv_image_t *z) {
  if (z != 0) {
    free(z->info);
    free(z->data);
    free(z);
  }

  return;
}

// TODO: Figure out the source of cvSet2D & cvGet2D function.

#if (0)
IplImage *aipcv_to_OpenCV(aipcv_image_t_ptr x) {
  IplImage *img;
  uint8_t  *z;
  uint32_t i;

  z = x->data[0];

  img = cvCreateImage(cvSize(x->info->nc, x->info->nr), IPL_DEPTH_8U, 1);

  if (img) {
#if (__AIPCV_DEBUG == 1)
    printf("Successfully created IplImage: %dx%d!\n", x->info->nc, x->info->nr);
#endif    

    for (i = 0; i < ((x->info->nc) * (x->info->nr)); i++)
      img->imageData[i] = *z++;
  }

  return img;
}


aipcv_image_t_ptr aipcv_from_OpenCV(IplImage *x) {
  aipcv_image_t_ptr img;

  uint8_t  *z;
  uint8_t  *y;
  uint32_t color = 0;
  uint32_t i = 0;
  uint8_t  k = 0;

  if ((x->depth == IPL_DEPTH_8U) && (x->nChannels == 1))
    img = aipcv_new_image(x->height, x->width);
  else if ((x->depth == IPL_DEPTH_8U) && (x->nChannels == 3)) {
    color = 1;
    img = aipcv_new_image(x->height, x->width);
  }
  else
    return 0;

  z = (uint8_t*)(x->imageData);
  y = img->data[0];

  for (i = 0; i < ((x->height) * (x->width)); i++) {
    if (color) k = ((*z++) + (*z++) + (*z++)) / 3;
    else       k = *z++;
    *(y++) = k;
  }

  return img;
}
#endif

IplImage *aipcv_to_OpenCV(aipcv_image_t_ptr x) {
  IplImage *img;
  
  uint32_t i;
  uint32_t j;

  CvScalar s;

  img = cvCreateImage(cvSize(x->info->nc, x->info->nr), IPL_DEPTH_8U, 1);

  if (img) {
#if (__AIPCV_DEBUG == 1)
    printf("Successfully created IplImage: %dx%d!\n", x->info->nc, x->info->nr);
#endif    

    for (i = 0; i < x->info->nr; i++) {
      for (j = 0; j < x->info->nc; j++) {
        s.val[0] = x->data[i][j];
        cvSet2D(img, i, j, s);          /*!> These two instructions are different with each other! */
        // img->imageData[i * x->info->nc + j] = s.val[0];
      }
    }
  }

  return img;
}

aipcv_image_t_ptr aipcv_from_OpenCV(IplImage *x) {
  aipcv_image_t_ptr img;

  uint32_t color = 0;
  int32_t  i = 0;
  int32_t  k = 0;
  int32_t  j = 0;

  CvScalar s;

  if ((x->depth == IPL_DEPTH_8U) && (x->nChannels == 1))
    img = aipcv_new_image(x->height, x->width);
  else if ((x->depth == IPL_DEPTH_8U) && (x->nChannels == 3)) {
    color = 1;
    img = aipcv_new_image(x->height, x->width);
  }
  else
    return 0;

  for (i = 0; i < x->height; i++) {
    for (j = 0; j < x->width; j++) {
      s = cvGet2D(x, i, j);
      if (color)     k = (uint8_t)((s.val[0] + s.val[1] + s.val[2]) / 3);
      else           k = (uint8_t)(s.val[0]);

      img->data[i][j] = k;
    }
  }

  return img;
}