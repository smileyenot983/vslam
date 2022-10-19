#include <string>
#include <iostream>

#include <opencv2/opencv.hpp>

#include "visual_odometry.cpp"



int main()
{


    std::string left_folder = "../dataset/image_l";
    std::string right_folder = "../dataset/image_r";

    cv::Mat img_left, img_right;


    int N_IMAGES = 20;
    std::string pth_left, pth_right;
    
    for(int i=1; i<=N_IMAGES; i++)
    {
        pth_left = left_folder + "/" + std::to_string(i) + ".png";
        pth_right = right_folder + "/" + std::to_string(i) + ".png";

        std::cout << pth_left << std::endl;

        img_left = cv::imread(pth_left);
        img_right = cv::imread(pth_right);

        cv::imwrite("left_image.png",img_left);
        cv::imwrite("right_image.png",img_right);


    }





    return 0;
}

