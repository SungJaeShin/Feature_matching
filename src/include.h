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

// image index range: 1 ~ 21
#define IMAGE_NUM 3

// extract mode; [1] ORB [2] GFTT [3] SIFT [4] GFTT + ORB [5] GFTT + SIFT 
#define EXTRACT_MODE 5

// descriptor mode; [1] AKAZE [2] SURF [3] DAISY
#define DESCRIPTOR_MODE 3

// feature point distance when using extractor
#define MIN_DIS 30