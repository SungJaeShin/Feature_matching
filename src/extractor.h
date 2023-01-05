#include "include.h"

// For SuperPoint Method
#include "SuperPoint/SPDetector.cpp"
#include "SuperPoint/SPDetector.hpp"
#include "SuperPoint/SuperPoint.cpp"
#include "SuperPoint/SuperPoint.hpp"

void gftt(cv::Mat image, std::vector<cv::KeyPoint> &img_kpt)
{
    // Feature Extractor
    cv::Mat img_gray;
    std::vector<cv::Point2f> img_2d;

    // If you use goodFeaturesToTrack, then you must convert gray scale
    // Opencv Error: src.type() == CV_8UC1 || src.type() == CV_32FC1 in function cornerEigenValsVecs
    cv::cvtColor(image, img_gray, cv::COLOR_BGR2GRAY);

    /* cv::goodFeaturesToTrack(InputArray image, OutputArray corners, int maxCorners, double qualityLevel, double minDistance, InputArray mask=noArray(), int blockSize=3, bool useHarrisDetector=false, double k=0.04)
       [-] Reference Site: https://m.blog.naver.com/PostView.naver?isHttpsRedirect=true&blogId=pckbj123&logNo=100203116086 
       1) image: input image 
       2) corners: output corner point2f 
       3) maxcorners: return maximum corner points
       4) qualitylevel: minimum value for determination the corner (product maximum value of eigenvalue)
       5) mindistance: euclidean distance between two corners   
       6) mask: interest ROI to find corners 
       7) blocksize: mean size of block 
       8) useHarrisDetector: the flag to use harris corner detector 
       9) k: parameter of harris detector */
    cv::goodFeaturesToTrack(img_gray, img_2d, 300, 0.01, MIN_DIS);

    // Convert cv::Point2f to cv::KeyPoint
    // [-] Reference Site: https://docs.opencv.org/4.x/d2/d29/classcv_1_1KeyPoint.html#:~:text=%E2%97%86-,convert(),-%5B2/2%5D
    cv::KeyPoint::convert(img_2d, img_kpt);
}

void orb(cv::Mat image, std::vector<cv::KeyPoint> &img_kpt)
{
    // Feature Extractor
    /* cv::ORB(int nfeatures, float scaleFactor, int nlevels, int edgeThreshold, int firstLevel, int WTA_K, int scoreType, int patchSize, int fastThreshold)
       [-] Reference Site: https://076923.github.io/posts/Python-opencv-38/
                           https://docs.opencv.org/3.4/db/d95/classcv_1_1ORB.html#adc371099dc902a9674bd98936e79739c 
       1) nfeatures: maximum feature number (default = 500) 
       2) scaleFactor: pyramid decimation ratio (default = 1.2f)
       3) nlevels: number of pyramid level (default = 8)
       4) edgeThreshold: size of border where features are not detected (default = 31)
       5) firstLevel: level of pyramid to put source image matched patchSize param (default = 0)
       6) WTA_K: The number of points that produce each element of the oriented BRIEF descriptor (default = 2)
       7) scoreType: The default HARRIS_SCORE means that Harris algorithm is used to rank features (default = ORB::HARRIS_SCORE)
       8) patchSize: size of the patch used by the oriented BRIEF descriptor (default = 31)
       9) fastThreshold: the fast threshold (default = 20) */
    cv::Ptr<cv::Feature2D> orb = cv::ORB::create(500, 1.2, 8, 31, 0, 2, cv::ORB::FAST_SCORE, 31, 20);
    orb -> detect(image, img_kpt);
}

void sift(cv::Mat image, std::vector<cv::KeyPoint> &img_kpt)
{
    // Feature Extractor
    /* cv:SIFT(int nfeatures, int nOctaveLayers, double contrastThreshold, double edgeThreshold, double sigma, [option] int descriptorType)
       [-] Reference Site: https://bkshin.tistory.com/entry/OpenCV-27-%ED%8A%B9%EC%A7%95-%EB%94%94%EC%8A%A4%ED%81%AC%EB%A6%BD%ED%84%B0-%EA%B2%80%EC%B6%9C%EA%B8%B0-SIFT-SURF-ORB
                           https://docs.opencv.org/3.4/d7/d60/classcv_1_1SIFT.html
       1) nfeatures: The number of best features to retain (default = 0)
       2) nOctaveLayers: The number of layers in each octave (default = 3)
       3) contrastThreshold: The contrast threshold used to filter out weak features in semi-uniform (low-contrast) regions. 
                             The larger the threshold, the less features are produced by the detector. (default = 0.04)
       4) edgeThreshold: The threshold used to filter out edge-like features (default = 10)
       5) sigma: The sigma of the Gaussian applied to the input image at the octave #0 (default = 1.6)
       6) descriptorType: The type of descriptors. Only CV_32F and CV_8U are supported
    */
    cv::Ptr<cv::xfeatures2d::SIFT> sift = cv::xfeatures2d::SIFT::create(500);
    sift -> detect(image, img_kpt);
}

void superpoint(SuperPointSLAM::SPDetector model, cv::Mat image, std::vector<cv::KeyPoint> &img_kpt, cv::Mat &img_des)
{
    // SuperPoint can compute image keypoints and descriptor simultaneously !!    
    /* Change Image Channel to Gray !
       [-] Reference Site: https://answers.opencv.org/question/172680/conversion-of-channel-3-to-channel-1/ */
    if(image.channels() != 1)
        cv::cvtColor(image, image, cv::COLOR_BGRA2GRAY);

    /* void SPDetector::detect(cv::InputArray _image, std::vector<cv::KeyPoint>& _keypoints, cv::Mat &_descriptors)
       [-] Reference Site: https://github.com/ChanWoo25/SuperPoint2CPP/blob/master/src/SPDetector.cpp */
    model.detect(image, img_kpt, img_des);
}