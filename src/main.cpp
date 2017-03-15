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

static cv::Mat norm_0_255(cv::InputArray _src) {
  cv::Mat src = _src.getMat();
  // Create and return normalized image
  cv::Mat dst;
  switch (src.channels()) {
    case 1:
      cv::normalize(_src, dst, 0, 255, cv::NORM_MINMAX, CV_8UC1);
      break;
    case 3:
      cv::normalize(_src, dst, 0, 255, cv::NORM_MINMAX, CV_8UC3);
      break;
    default:
      src.copyTo(dst);
      break;
  }
  return dst;
}

static void read_csv(const std::string& filename, std::vector<cv::Mat>& images,
                     std::vector<int>& labels, char token = ';') {
  std::ifstream file(filename.c_str(), std::ifstream::in);
  if (!file) {
    std::string error = "Input file invalid; Check its name.";
    CV_Error(CV_StsBadArg, error);
  }
  std::string line, path, classlabel;
  while (std::getline(file, line)) {
    std::stringstream lines(line);
    std::getline(lines, path, token);
    std::getline(lines, classlabel);
    if (!path.empty() && !classlabel.empty()) {
      images.push_back(cv::imread(path, 0));
      labels.push_back(std::atoi(classlabel.c_str()));
    }
  }
}

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
