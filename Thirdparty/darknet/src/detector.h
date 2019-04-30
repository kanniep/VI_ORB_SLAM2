//
// Created by kannie on 30/04/19.
//

#ifndef ORB_SLAM2_DETECTOR_H
#define ORB_SLAM2_DETECTOR_H
void setup_detector();
detection* detect(char *filename, float thresh, float hier_thresh);
#endif //ORB_SLAM2_DETECTOR_H
