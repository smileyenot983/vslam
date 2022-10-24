#include <opencv2/opencv.hpp>

#include <opencv2/features2d.hpp>
// #include <opencv2/xfeatures2d.hpp>
// #include "opencv2/nonfree/nonfree.hpp"

#include <string>
// processes pair of stereo images

class visualOdometry
{

    private: 
        // feature extractor(SIFT):
        // cv::Ptr<cv::Feature2D> sift_ptr = cv::SIFT::create();

        // cv::Ptr<cv::Feature2D> surf_ptr = cv::SURF::create(800.0);

        cv::Ptr<cv::Feature2D> detector;
    

    // void addFrame

    public:

        visualOdometry(std::string detector="sift")
        {   
            if(detector=="sift")
            {
                detector = cv::SIFT::create();
            }
            else if(detector=="surf")
            {
                detector = cv::SURF::create(800.0);
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
                cv::circle(img, cv::Point(kp.pt.x,kp.pt.y),5,cv::Scalar(0,0,255),2);
            }
        }
};