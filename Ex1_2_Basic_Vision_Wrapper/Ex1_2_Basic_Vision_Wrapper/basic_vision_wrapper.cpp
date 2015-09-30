// basic_vision_wrapper.c: A "wrapper" for basic vision algorithms.

#include <opencv2\opencv.hpp>
#include <cv.h>
#include <highgui.h>
#include <stdint.h>

int main(void) {
  IplImage *image = 0;

  CvScalar pixel_val;
  
  uint8_t r_val;
  uint8_t g_val;
  uint8_t b_val;

  uint16_t row, col;
  
  image = cvLoadImage("C:\\Users\\Wancheng\\Pictures\\microMsg.1428333509220.jpg", 1);

  if (image) {

    printf("Image size: %dx%d:\n",              image->width, image->height);
    printf("Colors per pixel (channels): %d\n", image->nChannels);
    printf("Bits per pixel: %d\n",              image->depth);
    printf("Image origin: %d\n",                image->origin);
    printf("Size of image row: %d\n",           image->widthStep);
    printf("Image data location: %x\n",         image->imageDataOrigin);

    // b_val = (uint8_t)*(image->imageDataOrigin);
    b_val = (uint8_t)*((image->imageDataOrigin) + ((uint32_t)(image->widthStep)) + ((uint32_t)(image->nChannels)) + 0);
    g_val = (uint8_t)*((image->imageDataOrigin) + ((uint32_t)(image->widthStep)) + ((uint32_t)(image->nChannels)) + 1);
    r_val = (uint8_t)*((image->imageDataOrigin) + ((uint32_t)(image->widthStep)) + ((uint32_t)(image->nChannels)) + 2);

    printf("The pixel value at (0, 0): R-%d, G-%d, B-%d\n", r_val, g_val, b_val);

    printf("Validate the pixel value using cvGet2D function...\n");
    pixel_val = cvGet2D(image, 0, 0);
    printf("The pixel value at (0, 0): R-%f, G-%f, B-%f\n", pixel_val.val[2], pixel_val.val[1], pixel_val.val[0]);
    
    printf("Set a 10x10 box start at (0, 0) to yellow!\n");
    pixel_val.val[0] = 0;
    pixel_val.val[1] = 255;
    pixel_val.val[2] = 255;

    for (row = 0; row < (0 + 20); row++) {
      for (col = 0; col < (0 + 20); col++) {
        cvSet2D(image, row, col, pixel_val);
      }
    }

    cvNamedWindow("Input Image", CV_WINDOW_NORMAL);   /*!> The user can change the size of the window */
    cvShowImage("Input Image", image); 
    
    printf("Press a key to exit\n");
    cvWaitKey(0);
    cvDestroyWindow("String");
  }
  else
  {
    fprintf(stderr, "Error reading Image\n");
  }

  // Read Image in grey level format
  image = cvLoadImage("C:\\Users\\Wancheng\\Pictures\\microMsg.1428333509220.jpg", 0);

  if (image) {
    cvNamedWindow("Gray Image", CV_WINDOW_NORMAL);               /*!> Create a window named as "Input Image" with CV_WINDOW_AUTOSIZE */
    cvShowImage("Gray Image", image);                            /*!> Display the image specified by an IplImage pointer */    

    // TODO: Try cvSaveImage(); cvMoveWindow();
    
    printf("Press a key to exit\n");
    cvWaitKey(0);
    cvDestroyWindow("String");
  }
  else
  {
    fprintf(stderr, "Error reading Image\n");
  }

  return 0;
}