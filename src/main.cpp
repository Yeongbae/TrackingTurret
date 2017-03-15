#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/objdetect.hpp>

cv::CascadeClassifier face_cascade;

void DetectAndDisplay (cv::Mat);
const std::string windowname("DISPLAYER");

int main () {
  cv::Mat frame;
  // Cascade setup
  if (!face_cascade.load("../data/haarcascades/haarcascade_frontalface_default.xml")) {
    std::cout << "Error loading face cascade." << std::endl;
    return -1;
  }
  // Tracker setup
  cv::Ptr<cv::Tracker> tracker = cv::Tracker::create("KCF");
  // Device name; default in POSIX
  std::string devName("/dev/video0");
  // Capture stream from device
  cv::VideoCapture devCapture(devName);
  // No stream = no run
  if (!devCapture.isOpened()) {
    std::cout << "Cannot open device." << std::endl;
    return -1;
  }
  // Create window
  cv::namedWindow(windowname, cv::WINDOW_KEEPRATIO);
  // Stream image
  std::cout << "Press 'q' to quit" << std::endl;
  while (1) {
    devCapture >> frame;
    if (frame.empty()) {
      std::cout << "Frame is empty." << std::endl;
    }
    DetectAndDisplay(frame);
    int stop = cv::waitKey(40);
    if (stop == 113) break;
  }
  return 0;
}

void DetectAndDisplay (cv::Mat frame) {
  std::vector<cv::Rect> faces;
  cv::Mat frame_gray;
  cv::cvtColor(frame, frame_gray, CV_BGR2GRAY);
  cv::equalizeHist(frame_gray, frame_gray);
  // Detection
  face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2,
                                0|CV_HAAR_SCALE_IMAGE, cv::Size(30,30));
  for (size_t i = 0; i < faces.size(); ++i) {
    cv::Point center(faces[i].x + faces[i].width*0.5,
                     faces[i].y + faces[i].height*0.5);
    cv::ellipse(frame, center,
                cv::Size(faces[i].width*0.5, faces[i].height*0.5),
                0, 0, 360, cv::Scalar(0, 255, 0), 3, 8, 0);
    cv::Mat faceROI = frame_gray(faces[i]);
  }
  cv::imshow(windowname, frame);
}
