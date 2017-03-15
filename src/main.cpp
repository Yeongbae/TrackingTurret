#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/face.hpp>

int main () {
  std::cout << "Ctrl + C to quit" << std::endl;
  // Device name; default in POSIX
  std::string devName("/dev/video0");
  // Capture stream from device
  cv::VideoCapture devCapture(devName);
  cv::Mat frame;
  // No stream = no run
  if (!devCapture.isOpened()) {
    std::cout << "Cannot open device." << std::endl;
    return -1;
  }
  // Create window
  cv::namedWindow("Disp", cv::WINDOW_AUTOSIZE);
  // Show frame
  while (1) {
    devCapture >> frame;
    if (frame.empty()) break;
    cv::imshow("Disp", frame);
    int stop = cv::waitKey(40);
    if (stop != -1) break;
  }
  return 0;
}
