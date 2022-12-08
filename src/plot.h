#include "include.h"

void drawimage(cv::Mat image1, cv::Mat image2, std::vector<cv::KeyPoint> img1_kpt, std::vector<cv::KeyPoint> img2_kpt)
{
    // Draw image keypoints 
    cv::Mat outqueryImg, outcandImg;
    cv::drawKeypoints(image1, img1_kpt, outqueryImg);
    cv::drawKeypoints(image2, img2_kpt, outcandImg);
    cv::imshow( "Query", outqueryImg);
    cv::imshow( "Cand", outcandImg);
}

void drawmatches(cv::Mat image1, cv::Mat image2, std::vector<cv::KeyPoint> img1_kpt, std::vector<cv::KeyPoint> img2_kpt, std::vector<cv::DMatch> match, std::string method)
{
    // Draw matching pair and save the result
    cv::Mat outImg;
    cv::drawMatches(image1, img1_kpt, image2, img2_kpt, match, outImg);
    cv::imshow(method, outImg);

    if(EXTRACT_MODE == 1)
        method = method + "_orb";
    else if(EXTRACT_MODE == 2)
        method = method + "_gftt";
    else if(EXTRACT_MODE == 3)
        method = method + "_sift";
    else if(EXTRACT_MODE == 4)
        method = method + "_gftt_orb";
    else if(EXTRACT_MODE == 5)
        method = method + "_gftt_sift";

    if(DESCRIPTOR_MODE == 1)
        method = method + "_akaze";
    else if(DESCRIPTOR_MODE == 2)
        method = method + "_surf";
    else if(DESCRIPTOR_MODE == 3)
        method = method + "_daisy";

    if(MATCHER_MODE == 1)
        method = method + "_bf";
    else if(MATCHER_MODE == 2)
        method = method + "_flann";

    if(MATCHING_MODE == 1)
        method = method + "_vanilla";
    else if(MATCHING_MODE == 2)
        method = method + "_knn";
    else if(MATCHING_MODE == 3)
        method = method + "_radius";

    std::string save_path = "/home/sj/workspace/paper_ws/icra2023/src/feature_matching/image/" + std::to_string(IMAGE_NUM) + "_" + method + "_result.png";
    cv::imwrite(save_path, outImg);
}
