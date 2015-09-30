/**
  * @filename: image_threshold.cpp
  * @author  : Wancheng Zhou
  * @version : 1.0
  * @brief   : This is a simple program tageted to threshold a color image.
  *
  */

#include <stdio.h>
#include <stdint.h>

#include <stdlib.h>
#include <math.h>
#include <cv.h>
#include <highgui.h>

void main(void) {

  IplImage *color_img = 0;
  uint32_t mean       = 0;
  uint32_t count      = 0;

  uint16_t i, j, k;
  
  color_img = cvLoadImage("C:/Users/Wancheng/Pictures/BlackSwanJpg.jpg");
  
  if (color_img) {
    printf("Input Image Size: %d x %d.\n", color_img->height, color_img->width);

    cvNamedWindow("Input Image", CV_WINDOW_AUTOSIZE);
    cvShowImage("Input Image", color_img);
    
    printf("Display of this image is done.\n");
    cvWaitKey(0);

    /*!> 1. Convert the input RGB image to Gray */
    for (i = 0; i < color_img->height; i++) {
      for (j = 0; j < color_img->width; j++) {

        /*!> NOTE: color_img->widthStep = color_img->width * color_img->nChannels */
        /*!> Calculate average RGB value of a pixel (R, G, B) */                
        k = ((color_img->imageData + i * color_img->widthStep)[j * color_img->nChannels + 0]  
           + (color_img->imageData + i * color_img->widthStep)[j * color_img->nChannels + 1]
           + (color_img->imageData + i * color_img->widthStep)[j * color_img->nChannels + 2]) / 3;

        /*!> Directly update corresponding pixel value */
        (color_img->imageData + i * color_img->widthStep)[j * color_img->nChannels + 0] = (uint8_t)k;
        (color_img->imageData + i * color_img->widthStep)[j * color_img->nChannels + 1] = (uint8_t)k;
        (color_img->imageData + i * color_img->widthStep)[j * color_img->nChannels + 2] = (uint8_t)k;

        mean += k;
        /*!> Let this value overflow to generate a random value for segmentation */
        // if (mean >= UINT16_MAX) printf("Mean value overflow!\n");  

        count++;
      }
    }

    mean = mean / (color_img->height * color_img->width); /*!> Should be divided by the number of pixels */

    cvNamedWindow("Gray", CV_WINDOW_AUTOSIZE);
    cvShowImage("Gray", color_img);
    cvWaitKey(0);

    /*!> Compute mean level for use as a threshold and pass through the image again */
    for (i = 0; i < color_img->height; i++) {
      for (j = 0; j < color_img->width; j++) {
        /*!> Get the value converted in step 1. */
        k = (color_img->imageData + i * color_img->widthStep)[j * color_img->nChannels + 0];
        
        if (k < mean) k = 0;     /*!> Define the value of segmentation */
        else          k = 255;
        /*!> Directly segment the whole image */
        (color_img->imageData + i * color_img->widthStep)[j * color_img->nChannels + 0] = (uint8_t) k;
        (color_img->imageData + i * color_img->widthStep)[j * color_img->nChannels + 1] = (uint8_t) k;
        (color_img->imageData + i * color_img->widthStep)[j * color_img->nChannels + 2] = (uint8_t) k;
      }
    }

    cvNamedWindow("Threshold", CV_WINDOW_AUTOSIZE);
    cvShowImage("Threshold", color_img);
    cvWaitKey(0);

    cvDestroyWindow("Input Image");
    cvDestroyWindow("Gray");
    cvDestroyWindow("Threshold");
  }
  else
  {
    printf("Invalid Image.\n");
  }
  
  
  return;
}
