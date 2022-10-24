#include <string>
#include <iostream>

#include <opencv2/opencv.hpp>

#include "visual_odometry.cpp"

#include <chrono>
using namespace std::chrono;

int main()
{


    std::string left_folder = "../dataset/image_l";
    std::string right_folder = "../dataset/image_r";

    cv::Mat img_left, img_right;


    int N_IMAGES = 10;
    std::string pth_left, pth_right;
    
    visualOdometry vo;


    std::vector<cv::KeyPoint> cur_kpts_l;
    cv::Mat cur_desc_l;
    std::string write_name_l = "image_l";

    std::vector<cv::KeyPoint> cur_kpts_r;
    cv::Mat cur_desc_r;
    std::string write_name_r = "image_r";

    auto start = high_resolution_clock::now();

    for(int i=1; i<=N_IMAGES; i++)
    {
        pth_left = left_folder + "/" + std::to_string(i) + ".png";
        pth_right = right_folder + "/" + std::to_string(i) + ".png";

        std::cout << pth_left << std::endl;

        img_left = cv::imread(pth_left);
        img_right = cv::imread(pth_right);

        // cv::imwrite("left_image.png",img_left);
        // cv::imwrite("right_image.png",img_right);


        vo.find_features(img_left, cur_kpts_l, cur_desc_r, true);
        vo.find_features(img_right, cur_kpts_r, cur_desc_r, true);

        std::cout << "n features(left): " << cur_kpts_l.size() << std::endl;
        std::cout << "n features(right): " << cur_kpts_r.size() << std::endl;
        
        cv::imwrite(write_name_l + std::to_string(i) + ".png",img_left);
        cv::imwrite(write_name_r + std::to_string(i) + ".png",img_right);

    }

    auto end = high_resolution_clock::now();

    auto duration = duration_cast<seconds>(end-start);

    std::cout << "duration: " << duration.count() << std::endl;





    return 0;
}

