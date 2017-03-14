// g++ main.cpp -o test
// -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_videoio
// -lopencv_face
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/face.hpp>

int main (int argc, char** argv) {
  // Any arguments?
  if (argc < 2) {
    std::cout << "usage: " << argv[0] << " <csv.ext> <output_folder>"
      << std::endl;
    exit(1);
  }
  std::string output_folder = ".";
  if (argc == 3) {
    output_folder = std::string(argv[2]);
  }
  // Variable Declaration
  std::string videoName("/dev/video0");
  cv::VideoCapture stream(videoName);
  cv::Mat frame;
  if (!stream.isOpened()) {
    std::cout << "Cannot open stream" << std::endl;
  }
  // Create window
  cv::namedWindow("Displayer", cv::WINDOW_AUTOSIZE);
  // Show the loaded image
  while (1) {
    stream >> frame;
    if (frame.empty()) break;
    cv::imshow("Displayer", frame);
    int stopper = cv::waitKey(25);
    if (stopper != -1) break;
  }
  return 0;
}
