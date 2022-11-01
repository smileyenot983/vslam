#include <opencv2/opencv.hpp>

#include <opencv2/features2d.hpp>
// #include <opencv2/xfeatures2d.hpp>
// #include "opencv2/nonfree/nonfree.hpp"

#include <string>

#include <Eigen/Dense>

// processes pair of stereo images

class visualOdometry
{

    private: 
        // feature extractor(SIFT):
        // cv::Ptr<cv::Feature2D> sift_ptr = cv::SIFT::create();

        // cv::Ptr<cv::Feature2D> surf_ptr = cv::SURF::create(800.0);

        cv::Ptr<cv::Feature2D> detector;
    
        // flann - suitable for all types of features
        cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create(cv::DescriptorMatcher::FLANNBASED);

    // void addFrame

    public:

        visualOdometry(std::string detector_type="sift")
        {   
            if(detector_type=="sift")
            {
                detector = cv::SIFT::create();
            }
            // else if(detector_type=="surf")
            // {
            //     detector = cv::SURF::create(800.0);
            // }
            else if(detector_type=="orb")
            {
                detector = cv::ORB::create(10);
            }
            else{
                std::cout << "[ERROR] WRONG DETECTOR" << std::endl;
            }
        }


        void find_features(cv::Mat &img, std::vector<cv::KeyPoint> &kpts, cv::Mat &desc, bool visualize = false)
        {
            detector->detectAndCompute(img, cv::Mat(), kpts, desc);

            if(visualize)
            {
                draw_features(img, kpts);
            }

        }

        void draw_features(cv::Mat &img, const std::vector<cv::KeyPoint> &kpts)
        {
            for(const auto &kp : kpts)
            {
                cv::circle(img, cv::Point(kp.pt.x,kp.pt.y),1,cv::Scalar(0,0,255),2);
            }
        }

        // does simplest feature matching between left and right image
        void match_features(const cv::Mat &desc_l, const cv::Mat &desc_r, std::vector<std::vector<cv::DMatch>> &knn_matches, std::vector<cv::DMatch> &good_matches)
        {


            // std::vector<std::vector<cv::DMatch>> knn_matches;
            matcher->knnMatch(desc_l, desc_r, knn_matches, 2);

            // we keep 2 closest neighbors for all keypoints for lowe's test
            const float RATIO_THRESH = 0.7f;
            // go through all features
            for(size_t i=0; i < knn_matches.size();i++)
            {
                // compare 2 distances: dist(feature, closest) , dist(feature, second closest)
                auto dist1 = knn_matches[i][0].distance;
                auto dist2 = knn_matches[i][1].distance;

                // std::cout << "dist1: " << dist1 << "| dist2: " << dist2 << std::endl;
                if(dist1 < RATIO_THRESH * dist2)
                {
                    good_matches.push_back(knn_matches[i][0]);
                }
            }


            std::cout << "match_features.size(): " << knn_matches.size() << std::endl;
            std::cout << "good_features.size(): " << good_matches.size() << std::endl;
        }

        void triangulate_features(const std::vector<cv::KeyPoint> &kpts_l,
                                  const std::vector<cv::KeyPoint> &kpts_r,
                                  const std::vector<cv::DMatch> &knn_matches, const Eigen::Matrix<double,3,4> &pose)
        {
            // 
        }


        void visualize_matches( const cv::Mat &img_l, const std::vector<cv::KeyPoint> &kpts_l, 
                                const cv::Mat &img_r, const std::vector<cv::KeyPoint> &kpts_r,
                                const std::vector<cv::DMatch> &knn_matches, cv::Mat &img_merged)
        {

            
            // merge 2 images first(diagonally)
            img_merged = cv::Mat::zeros(img_l.rows, img_l.cols + img_r.cols, CV_8UC3);

            int HOR_OFFSET = img_l.cols;
            for(size_t i = 0; i < img_l.rows; ++i)
            {
                for(size_t j = 0; j < img_l.cols; ++j)
                {
                    img_merged.at<cv::Vec3b>(i,j) = img_l.at<cv::Vec3b>(i,j);
                    img_merged.at<cv::Vec3b>(i,j+HOR_OFFSET) = img_r.at<cv::Vec3b>(i,j);
                }
            }

            // visualizing matches:
            for(size_t i = 0; i < knn_matches.size(); i++)
            {
                int idx1 = knn_matches[i].queryIdx;
                int idx2 = knn_matches[i].trainIdx;

                cv::circle(img_merged, cv::Point(kpts_l[idx1].pt.x, kpts_l[idx1].pt.y),3, cv::Scalar(255,0,0), 2);
                cv::circle(img_merged, cv::Point(kpts_r[idx2].pt.x + HOR_OFFSET, kpts_r[idx2].pt.y), 3, cv::Scalar(0,255,0), 2);
            
                cv::line(img_merged, cv::Point(kpts_l[idx1].pt.x, kpts_l[idx1].pt.y), cv::Point(kpts_r[idx2].pt.x + HOR_OFFSET, kpts_r[idx2].pt.y), cv::Scalar(0,255,0),
                            1);
            }

            // std::cout << "kpts_l.size(): " << kpts_l.size() << std::endl;
            // std::cout << "kpts_r.size(): " << kpts_r.size() << std::endl;

            // cv::imshow("disp",img_merged);
            // cv::waitKey(0);
            // cv::destroyWindow("disp");


        }
};