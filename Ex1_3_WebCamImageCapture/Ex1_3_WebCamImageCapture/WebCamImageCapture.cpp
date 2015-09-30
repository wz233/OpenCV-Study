#include <stdint.h>
#include <stdio.h>

#include <cv.h>
#include <highgui.h>

#include "WebCamImageCapture.h"

#if (CAM_CAP_MODE_SEL == OPENCV_C_PLUS_MODE)
  #include <opencv2\opencv.hpp>
  using namespace cv;
#endif

void main(void) {
  
  CvCapture *webcam = 0;
  IplImage  *camimg = 0;

  uint16_t  i = 0;
  uint16_t  n = 0;

#if (CAM_CAP_MODE_SEL == OPENCV_C_PLUS_MODE)
  VideoCapture cap(0);
#endif

  char      filename[256] = { "" };
  char      c;

#if (CAM_CAP_MODE_SEL == OPENCV_C_MODE)
  // webcam = cvCreateCameraCapture(0);
  webcam = cvCaptureFromCAM(CV_CAP_ANY);
  // webcam = cvCreateCameraCapture(200);
  if (!webcam) {
    printf("There's no camera in this PC!\n");
  }
  else {
    printf("Camera exists!\n");
  }

  cvNamedWindow("video", CV_WINDOW_AUTOSIZE);
  cvMoveWindow("video", 150, 200);

  for (i = 0; i < 600; i++) {
    camimg = cvQueryFrame(webcam);
    if (!camimg) {
      printf("Capture failed!\n");
    }
    else {
      c = cvWaitKey(100);
      cvShowImage("video", camimg);  /*!> Display current frame */

      if (c > 0) {
        sprintf_s(filename, "C:/AIPCV/VideoFrame%d.jpg", n++);
        if (!cvSaveImage(filename, camimg)) {
          printf("Failed to save frame!\n");
          return;
        }
        else {
          printf("Successfully save the frame!\n");
        }
      }
    }
  }

  cvReleaseCapture(&webcam);

  cvWaitKey(0); 

#elif (CAM_CAP_MODE_SEL == OPENCV_C_PLUS_MODE)

  if (!cap.isOpened()) {
    printf("Failed to open camera!\n");
    return;
  }

  Mat edges;
  namedWindow("edges", 1);
  for (;;)
  {
    Mat frame;
    cap >> frame; // get a new frame from camera
    cvtColor(frame, edges, CV_BGR2GRAY);
    GaussianBlur(edges, edges, Size(7, 7), 1.5, 1.5);
    Canny(edges, edges, 0, 30, 3);
    imshow("edges", edges);
    if (waitKey(30) >= 0) break;
  }
  // the camera will be deinitialized automatically in VideoCapture destructor

#endif

  return;
}