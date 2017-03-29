#include <iostream>
#include <fstream>
#include <string>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/objdetect.hpp>

cv::CascadeClassifier face_cascade;
const std::string windowname("disp");

cv::Mat Detect (cv::Mat, cv::Rect2d*);

int main (int argc, char** argv) {
  cv::Mat frame; // Will hold every frame for processing
  cv::Mat dispf; // Product of process
  cv::Ptr<cv::Tracker> tracker = cv::Tracker::create("KCF"); // Will hold tracker
  cv::Rect2d roi; // Region of Interest
  std::string device; // Will hold device name
  int cmd = 0; // Will hold cmd input by waitKey
  int iter = 0; // Will hold iteration no. for tracker reset
  // First argument will be the device name
  // If there is no argument, device will be set to DEFAULT IN POSIX
  if (argc == 1) {
    device = "/dev/video0";
  } else {
    device = argv[1];
  }
  // Capture video from device
  // No stream = no run
  cv::VideoCapture videostream(device);
  if (!videostream.isOpened()) {
    std::cout << "Cannot open video stream from " << device << std::endl;
    return 1;
  }
  // Face cascade setup
  if (!face_cascade.load("../data/haarcascades/haarcascade_frontalface_alt.xml")) {
    std::cout << "Error loading face cascade." << std::endl;
    return 2;
  }
  // Create display windowname
  cv::namedWindow(windowname, cv::WINDOW_KEEPRATIO);
  // Stream image
  // Q to quit
  // R to reset roi
  videostream >> frame;
  dispf = Detect(frame, &roi);
  cv::imshow(windowname, dispf);
  cmd = cv::waitKey(25);
  do {
    tracker->init(dispf, roi);
    if (cmd == 'r') {
      videostream >> frame;
      dispf = Detect(frame, &roi);
    } else {
      videostream >> frame;
      dispf = frame;
      tracker->update(dispf, roi);
      cv::rectangle(frame, roi, cv::Scalar(255, 0, 0), 2, 1);
    }
    cv::imshow(windowname, dispf);
    cmd = cv::waitKey(25);
  } while (cmd != 'q');
  return 0;
}

cv::Mat Detect (cv::Mat frame, cv::Rect2d* proi) {
  std::vector<cv::Rect> faces;
  cv::Mat grayframe, dispframe;
  dispframe = frame;
  cv::cvtColor(frame, grayframe, CV_BGR2GRAY);
  cv::equalizeHist(grayframe, grayframe);
  face_cascade.detectMultiScale(grayframe, faces, 1.1, 2,
                                0|CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));
  for (size_t i = 0; i < faces.size(); ++i) {
    cv::Point center(faces[i].x+faces[i].width*0.5,
                     faces[i].y+faces[i].height*0.5);
    cv::Rect2d roi(center.x-0.5*faces[i].width, center.y-0.5*faces[i].height,
                 faces[i].width, faces[i].height);
    cv::rectangle (dispframe, roi, cv::Scalar(0, 255, 0), 2, 1);
    *proi = roi;
  }
  return dispframe;
}
