#include <opencv2\opencv.hpp>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

int main()
{
  Mat img = imread("C:\\Users\\Wancheng\\Pictures\\microMsg.1428333509220.jpg");   //×¢ÒâÍ¼Æ¬µØÖ·Â·¾¶
  if (img.empty()) {
    cout << "error";	return -1;
  }
  imshow("mypic", img);
  waitKey();
  return 0;
}
 