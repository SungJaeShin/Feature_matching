#include <iostream>
#include <vector>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/xfeatures2d/nonfree.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include <chrono>

// image index range; 1 ~ 23
#define IMAGE_NUM 23

// extract mode; [1] ORB [2] GFTT [3] SIFT [4] GFTT + ORB [5] GFTT + SIFT 
#define EXTRACT_MODE 1

// descriptor mode; [1] AKAZE [2] SURF [3] DAISY
#define DESCRIPTOR_MODE 2

// matcher method; [1] BF matcher [2] FLANN matcher 
#define MATCHER_MODE 1

// matching method; [1] vanilla match [2] knn match [3] radius match
#define MATCHING_MODE 2

// rejection method; [1] fundamental matrix [2] vector field consensus
#define REJECTION_MODE 2

// feature point distance when using extractor
#define MIN_DIS 30
