#if (1)
#include "app_main_aipcv_thrglh.h"

void main(void) {

  IplImage *img  = 0;
  IplImage *img2 = 0;

  aipcv_image_t_ptr x;

  uint32_t height, width, step, channels;
  uint8_t *data;

  uint32_t mean   = 0;
  uint32_t count  = 0;

  img = cvLoadImage("C:\\Users\\Wancheng\\Pictures\\BlackSwanJpg.jpg", CV_LOAD_IMAGE_COLOR);

  height    = img->height;
  width     = img->width;
  step      = img->widthStep;
  channels  = img->nChannels;
  data      = (uint8_t*)img->imageData;

  printf("Now processing an image with %dx%d resolution, and %d channels, %d depth\n", height, width, channels, img->depth);

  cvNamedWindow("Source Image", CV_WINDOW_AUTOSIZE);
  cvMoveWindow("Source Image", 100, 100);
  cvShowImage("Source Image", img);

  x = aipcv_from_OpenCV(img);
  printf("Convert image from OpenCV to AIPCV: %dx%d\n", x->info->nc, x->info->nr);

  if (x) {
    aipcv_thr_glh(x);                                               /*!> Comment this to check normal AIPCV convert functionality */
    img2 = aipcv_to_OpenCV(x);

    printf("converted image: %dx%d\n", img2->width, img2->height);

    cvNamedWindow("Threshold", CV_WINDOW_AUTOSIZE);
    cvMoveWindow("Threshold", 100 + width, 100);
    cvShowImage("Threshold", img2); 
  }

  cvWaitKey(0);

  cvReleaseImage(&img);

  return;
}
#endif
