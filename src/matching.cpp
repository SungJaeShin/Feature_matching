#include "extractor.h"
#include "descriptor.h"
#include "plot.h"

int main()
{
    std::string query_path = "/home/sj/workspace/paper_ws/icra2023/src/feature_matching/image/test_pair/" + std::to_string(IMAGE_NUM) + "_query.png";
    std::string cand_path = "/home/sj/workspace/paper_ws/icra2023/src/feature_matching/image/test_pair/" + std::to_string(IMAGE_NUM) + "_cand.png";
    cv::Mat query = cv::imread(query_path);
    cv::Mat cand = cv::imread(cand_path);

    /* ---------------------------------------------------------------------------------------- */ 
    // Initialization Keypoints and Descriptors 
    std::vector<cv::Point2f> query_2d, cand_2d;
    std::vector<cv::KeyPoint> query_kpt, cand_kpt;
    cv::Mat query_des, cand_des;

    /* ---------------------------------------------------------------------------------------- */ 
    // Extract Method
    if(EXTRACT_MODE == 1)
    {
        orb(query, query_kpt);
        orb(cand, cand_kpt);
    }
    else if(EXTRACT_MODE == 2)
    {
        gftt(query, query_kpt);
        gftt(cand, cand_kpt);
    }
    else if(EXTRACT_MODE == 3)
    {
        sift(query, query_kpt);
        sift(cand, cand_kpt);
    }
    else if(EXTRACT_MODE == 4)
    {
        gftt(query, query_kpt);
        orb(cand, cand_kpt);
    }
    else if(EXTRACT_MODE == 5)
    {
        gftt(query, query_kpt);
        sift(cand, cand_kpt);
    }
    
    /* ---------------------------------------------------------------------------------------- */ 
    // Descriptor Method
    if(DESCRIPTOR_MODE == 1)
        akaze(query, cand, query_kpt, cand_kpt, query_des, cand_des);
    else if(DESCRIPTOR_MODE == 2)
        surf(query, cand, query_kpt, cand_kpt, query_des, cand_des);
    else if(DESCRIPTOR_MODE == 3)
        daisy(query, cand, query_kpt, cand_kpt, query_des, cand_des);

    /* ---------------------------------------------------------------------------------------- */ 
    // See Feature Points
    drawimage(query, cand, query_kpt, cand_kpt);

    /* ---------------------------------------------------------------------------------------- */ 
    // Get Good Points    
    cv::FlannBasedMatcher matcher;
    std::vector<std::vector<cv::DMatch>> matches;
    matcher.knnMatch(query_des, cand_des, matches, 2);
    std::cout << "[FIRST] # FLANN Matches: " << matches.size() << std::endl;

    // Convert Point2f and Get good match points
    double threshold = 0.95f;
    std::vector<cv::KeyPoint> good_query_kpt, good_cand_kpt; 
	std::vector<cv::DMatch> good_matches;
    for(int i = 0; i < matches.size(); i ++)
    {
        if(matches[i][0].distance < threshold * matches[i][1].distance)
        {
            int query_idx = matches[i][0].queryIdx;
            int cand_idx = matches[i][0].trainIdx;
            query_2d.push_back(query_kpt[query_idx].pt);
            cand_2d.push_back(cand_kpt[cand_idx].pt);
            good_query_kpt.push_back(query_kpt[query_idx]);
            good_cand_kpt.push_back(cand_kpt[cand_idx]);
            good_matches.push_back(matches[i][0]);
        }
    }
    std::cout << "[SECOND] # Good FLANN Matches: " << good_matches.size() << std::endl;

    /* ----- If you want to first matching results, please erase annotated this code !!! ----- */ 
    // // Draw Good Matches (First Good Points) 
    // std::string save_good = "good";
    // drawmatches(query, cand, query_kpt, cand_kpt, good_matches, save_good);

    /* ---------------------------------------------------------------------------------------- */ 
    // Remove Outlier Points (Get Final Points)
    /* cv::findFundmentalMatrix(InputArray points1, InputArray points2, int method, double ransacReprojThreshold, double confidence, int maxIters, OutputArray mask = noArray())*/
    cv::Mat F;
    std::vector<uchar> status;
    std::vector<cv::KeyPoint> final_query_kpt, final_cand_kpt; 
	std::vector<cv::DMatch> final_matches;
    int count = 0;
    F = cv::findFundamentalMat(query_2d, cand_2d, cv::FM_RANSAC, 3.0, 0.99, status);
    // std::cout << "Fundmental Matrix: " << F << std::endl;
    // std::cout << "status size: " << status.size() << std::endl;

    for(int i = 0; i < query_2d.size(); i++)
    {
        cv::Mat query_2pt = (cv::Mat_<double>(3, 1) << query_2d[i].x, query_2d[i].y, 1);
        cv::Mat cand_2pt = (cv::Mat_<double>(1, 3) << cand_2d[i].x, cand_2d[i].y, 1);
        if(status[i] > 0)
        {
            // std::cout << "[inlier] x'Fx = " << cand_2pt * F * query_2pt << std::endl;
            final_query_kpt.push_back(good_query_kpt[i]);
            final_cand_kpt.push_back(good_cand_kpt[i]);
            final_matches.push_back(good_matches[i]);
            count++;
        }
        // else
        //     std::cout << "[outlier] x'Fx = " << cand_2pt * F * query_2pt << std::endl;
    }
    std::cout << "[FINAL] # Final inlier FLANN Matches: " << count << std::endl;

    /* ---------------------------------------------------------------------------------------- */ 
    // Draw Final Matches (Final Points) 
    std::string save_final = "final";
    drawmatches(query, cand, query_kpt, cand_kpt, final_matches, save_final);
    cv::waitKey(0);
    return 0;
}
