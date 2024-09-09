#include "extractor.h"
#include "descriptor.h"
#include "plot.h"
#include "rejection.h"

int main()
{
    std::string query_path = "../image/test_pair/" + std::to_string(IMAGE_NUM) + "_query.png";
    std::string cand_path = "../image/test_pair/" + std::to_string(IMAGE_NUM) + "_cand.png";
    cv::Mat query = cv::imread(query_path);
    cv::Mat cand = cv::imread(cand_path);

    /* ---------------------------------------------------------------------------------------- */ 
    // Initialization Keypoints and Descriptors 
    std::vector<cv::Point2f> query_2d, cand_2d;
    std::vector<cv::KeyPoint> query_kpt, cand_kpt;
    cv::Mat query_des, cand_des;

    /* ---------------------------------------------------------------------------------------- */ 
    // Extract Method
    auto extract_start = std::chrono::high_resolution_clock::now();
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
    
    auto extract_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> extract_duration = extract_end - extract_start;

    /* ---------------------------------------------------------------------------------------- */ 
    // Descriptor Method
    auto descriptor_start = std::chrono::high_resolution_clock::now();
    if(EXTRACT_MODE != 6 && DESCRIPTOR_MODE == 1)
        akaze(query, cand, query_kpt, cand_kpt, query_des, cand_des);
    else if(EXTRACT_MODE != 6 && DESCRIPTOR_MODE == 2)
        surf(query, cand, query_kpt, cand_kpt, query_des, cand_des);
    else if(EXTRACT_MODE != 6 && DESCRIPTOR_MODE == 3)
        daisy(query, cand, query_kpt, cand_kpt, query_des, cand_des);

    auto descriptor_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> descriptor_duration = descriptor_end - descriptor_start;

    /* ---------------------------------------------------------------------------------------- */ 
    // See Feature Points
    drawimage(query, cand, query_kpt, cand_kpt);
    std::cout << "query keypoint size: " << query_kpt.size() << std::endl;
    std::cout << "cand keypoint size: " << cand_kpt.size() << std::endl;
    std::cout << "----------------------------------------------" << std::endl;

    /* ---------------------------------------------------------------------------------------- */ 
    // Matcher Method      
    cv::Ptr<cv::DescriptorMatcher> matcher;
    if(MATCHER_MODE == 1)
    {
        // ***[IMPORTANT]*** When you use BFMatcher, please change in create arguments
        // [CASE 1] SIFT, SURF => cv::NORM_L1 & cv::NORM_L2  
        // [CASE 2] ORB => cv::NORM_HAMMING
        // The last argument of create function is boolean that meaning of crosscheck (True value means double check that add opposite direction) 
        cv::Ptr<cv::BFMatcher> bf_matcher = cv::BFMatcher::create(cv::NORM_L2, false);
        matcher = bf_matcher;
    }
    else if(MATCHER_MODE == 2)
    {
        cv::Ptr<cv::FlannBasedMatcher> flann_matcher = cv::FlannBasedMatcher::create();
        matcher = flann_matcher;
    }

    /* ---------------------------------------------------------------------------------------- */ 
    // Matching Method
    // [-] Reference Site: https://bkshin.tistory.com/entry/OpenCV-28-%ED%8A%B9%EC%A7%95-%EB%A7%A4%EC%B9%ADFeature-Matching
    auto matching_start = std::chrono::high_resolution_clock::now();
    double threshold = 0.95f;
    std::vector<cv::KeyPoint> good_query_kpt, good_cand_kpt; 
	std::vector<cv::DMatch> good_matches;
 
    if(MATCHING_MODE == 1)
    {
        // match(InputArray queryDescriptors, InputArray trainDescriptors, std::vector<cv::DMatch> &matches, InputArray mask=noArray())
        std::vector<cv::DMatch> matches;
        matcher -> match(query_des, cand_des, matches);
        std::cout << "[FIRST] # of Matches: " << matches.size() << std::endl;

        // Convert Point2f 
        for(int i = 0; i < matches.size(); i ++)
        {
            int query_idx = matches[i].queryIdx;
            int cand_idx = matches[i].trainIdx;
            query_2d.push_back(query_kpt[query_idx].pt);
            cand_2d.push_back(cand_kpt[cand_idx].pt);
            good_query_kpt.push_back(query_kpt[query_idx]);
            good_cand_kpt.push_back(cand_kpt[cand_idx]);
            good_matches.push_back(matches[i]);
        }
    }
    else if(MATCHING_MODE == 2)
    {
        // knnMatch(InputArray queryDescriptors, InputArray trainDescriptors, std::vector<std::vector<cv::DMatch>> &matches, int k, InputArray mask=noArray(), bool compactResult=false)
        std::vector<std::vector<cv::DMatch>> matches;
        matcher -> knnMatch(query_des, cand_des, matches, 2);
        std::cout << "[FIRST] # of Matches: " << matches.size() << std::endl;

        // Convert Point2f and Get good match points using ratio test
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
    }
    else if(MATCHING_MODE == 3)
    {
        // radiusMatch (InputArray queryDescriptors, InputArray trainDescriptors, std::vector<std::vector<cv::DMatch>> &matches, float maxDistance, InputArray mask=noArray(), bool compactResult=false) 
        // [Preference radius] 0.4 => 405 matches / 0.3 => 183 matches / 0.2 => 117 matches
        std::vector<std::vector<cv::DMatch>> matches;
        matcher -> radiusMatch(query_des, cand_des, matches, 0.3);
        std::cout << "[FIRST] # of Matches: " << matches.size() << std::endl;
        
        // Convert Point2f 
        for(int i = 0; i < matches.size(); i ++)
        {            
            int min_idx = 0;
            int min_dist = matches[i][0].distance;
            
            for(int j = 0; j < matches[i].size(); j++)
            {
                if(min_dist > matches[i][j].distance)
                {
                    min_dist = matches[i][j].distance;
                    min_idx = j;
                }
            }

            int query_idx = matches[i][min_idx].queryIdx;
            int cand_idx = matches[i][min_idx].trainIdx;
            query_2d.push_back(query_kpt[query_idx].pt);
            cand_2d.push_back(cand_kpt[cand_idx].pt);
            good_query_kpt.push_back(query_kpt[query_idx]);
            good_cand_kpt.push_back(cand_kpt[cand_idx]);
            good_matches.push_back(matches[i][min_idx]);
        }
    }
    auto matching_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> matching_duration = matching_end - matching_start;

    std::cout << "[SECOND] # of Matches after filtering: " << good_matches.size() << std::endl;

    /* ----- If you want to first matching results, please erase annotated this code !!! ----- */ 
    // // Draw Good Matches (First Good Points) 
    // std::string save_good = "good";
    // drawmatches(query, cand, query_kpt, cand_kpt, good_matches, save_good);

    /* ---------------------------------------------------------------------------------------- */ 
    // Outlier Rejection (Get Final Points)
    auto outlier_rejection_start = std::chrono::high_resolution_clock::now();
    std::vector<cv::KeyPoint> final_query_kpt, final_cand_kpt; 
	std::vector<cv::DMatch> final_matches;
    int count = 0;

    if(REJECTION_MODE == 1)
        rejectionUsingFundamentalMat(count, query_2d, cand_2d, good_query_kpt, good_cand_kpt, good_matches, final_query_kpt, final_cand_kpt, final_matches);
    else if(REJECTION_MODE == 2)
        rejectionUsingVectorFieldConsensus(count, query_2d, cand_2d, good_query_kpt, good_cand_kpt, good_matches, final_query_kpt, final_cand_kpt, final_matches);

    auto outlier_rejection_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> outlier_rejection_duration = outlier_rejection_end - outlier_rejection_start;

    std::cout << "[FINAL] # of Final inlier Matches: " << count << std::endl;
    std::cout << "----------------------------------------------" << std::endl;

    /* ---------------------------------------------------------------------------------------- */ 
    // Time Consumption Results 
    std::cout << "Extraction Time: " << extract_duration.count() << " ms" << std::endl;
    std::cout << "Descriptor Time: " << descriptor_duration.count() << " ms" << std::endl;
    std::cout << "Matching Time: " << matching_duration.count() << " ms" << std::endl;
    std::cout << "Outlier Rejection Time: " << outlier_rejection_duration.count() << " ms" << std::endl;
    std::cout << "----------------------------------------------" << std::endl;

    /* ---------------------------------------------------------------------------------------- */ 
    // Draw Final Matches (Final Points) 
    std::string save_final = "final";
    drawmatches(query, cand, query_kpt, cand_kpt, final_matches, save_final);
    cv::waitKey(0);
    return 0;
}
