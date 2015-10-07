#include "aipcv_thr_glh.h"

static float aipcv_otsu_nu(float *p, uint32_t k, float ut, float vt);
static float aipcv_otsu_u (float *p, uint32_t k);
static float aipcv_otsu_w (float *p, uint32_t k);

static float aipcv_otsu_w (float *p, uint32_t k) {
  uint32_t i;
  float    x = 0.0f;

  for (i = 1; i < k; i++)
    x += p[i];

  return x;
}

static float aipcv_otsu_u (float *p, uint32_t k) {
  uint32_t i;
  float    x = 0.0f;

  for (i = 1; i < k; i++)
    x += (float)(i * p[i]);

  return x;
}

static float aipcv_otsu_nu(float *p, uint32_t k, float ut, float vt) {
  float x, y;

  y = aipcv_otsu_w(p, k);
  x = ut * y - aipcv_otsu_u(p, k);
  x = x * x;
  y = y * (1.0f - y);

  if (y > 0)  x = x / y;
  else        x = 0.0f;

  return (x / vt);
}

#if (0)  /*!> Original Implementation */
void aipcv_thr_glh(aipcv_image_t_ptr x) {
  int32_t i, j, k, n, m, h[260], t;
  float   y, z, p[260];
  uint8_t *pp;
  float   ut = 0.0f;                      /*!> Global mean value */
  float   vt = 0.0f;                      /*!> Global variance value */

  n = (x->info->nr) * (x->info->nc);      /*!> Total number of pixels */
    
  for (i = 0; i < 260; i++) {		          /*!> Zero the histograms	*/
    h[i] = 0;
    p[i] = 0.0f;
  }

  /*!> Accumulate a histogram: Get the number of pixels for each gray level */

  for (i = 0; i < (int32_t)x->info->nr; i++) {
    for (j = 0; j < (int32_t)x->info->nc; j++) {
      k = x->data[i][j];
      h[k + 1] += 1;
    }
  }
    
  for (i = 1; i <= 256; i++)		          /*!> Normalize into a distribution */
    p[i] = (float)h[i] / (float)n;        /*!> Note: i stands for gray value */

  ut = aipcv_otsu_u (p, 256);		          /*!> Global mean */
  for (i = 1; i <= 256; i++)              /*!> Calculate Global Variance */
    vt += (i - ut) * (i - ut) * p[i];

  j = -1; k = -1;
  for (i = 1; i <= 256; i++) {
    if ((j < 0) && (p[i] > 0.0f)) j = i;	/*!> First index */
    if (p[i] > 0.0f)              k = i;	/*!> Last index  */
  }

  z = -1.0f;
  m = -1;
  for (i = j; i <= k; i++) {
    y = aipcv_otsu_nu(p, i, ut, vt);	    /*!> Compute NU */
    if (y >= z) {			                    /*!> Is it the biggest? */
      z = y;			                        /*!> Yes. Save value and i */
      m = i;
    }
  }

  t = m;
  printf("Threshold found is %d\n", t);

  /*!> Threshold */
  pp = x->data[0];
  for (i = 0; i < n; i++) {
    if (*pp < t)      *pp++ = 0;
    else              *pp++ = 255;
  }
}

#endif

void aipcv_thr_glh(aipcv_image_t_ptr x) {
  int32_t h;
  int32_t w;
  int32_t gv;
  
  uint32_t n_pixels;

  float histogram[256] = { 0.0f };

  for (h = 0; h < (x->info->nr); h++) {
    for (w = 0; w < (x->info->nc); w++) {
      histogram[x->data[h][w]] ++;
    }
  }

  n_pixels = (x->info->nr) * (x->info->nc);

  for (gv = 0; gv < 256; gv++) {
    histogram[gv] = histogram[gv] / (float)(n_pixels);  /*!> Normalize histogram */
  }
}