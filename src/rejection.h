#include "include.h"

void rejectionUsingFundamentalMat(int &inliers, std::vector<cv::Point2f> query_2d, std::vector<cv::Point2f> cand_2d, 
                                  std::vector<cv::KeyPoint> good_query_kpt, std::vector<cv::KeyPoint> good_cand_kpt, std::vector<cv::DMatch> good_matches,
                                  std::vector<cv::KeyPoint> &final_query_kpt, std::vector<cv::KeyPoint> &final_cand_kpt, std::vector<cv::DMatch> &final_matches)
{
    /* cv::findFundmentalMatrix(InputArray points1, InputArray points2, int method, double ransacReprojThreshold, double confidence, int maxIters, OutputArray mask = noArray())*/
    cv::Mat F;
    std::vector<uchar> status;
    F = cv::findFundamentalMat(query_2d, cand_2d, cv::FM_RANSAC, 1, 0.999, status);
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
            inliers++;
        }
        // else
        //     std::cout << "[outlier] x'Fx = " << cand_2pt * F * query_2pt << std::endl;
    }
}