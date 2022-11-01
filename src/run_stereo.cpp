#include <string>
#include <iostream>

#include <opencv2/opencv.hpp>

#include "visual_odometry.cpp"


#include <Eigen/Dense>
#include <chrono>
using namespace std::chrono;

int main()
{

    Eigen::Matrix3d m;
    m << 1,2,3,
         6,5,6,
         7,8,9;

    std::cout << m << std::endl;

    Eigen::Matrix3d m2;
    m2 << 3,2,7,
          1,4,2,
          5,6,7;

    std::cout << m2 << std::endl;


    Eigen::Matrix3d m_to_m2;
    m_to_m2 = m * m2.inverse();

    // vector in world coords:
    Eigen::Vector3d v_world;
    v_world << 1,2,3;

    // writing vector coordinates in odom's frame:
    Eigen::Vector3d v_odom;
    v_odom = m2 * m.inverse() * v_world;


    std::cout << "v_world: " << v_world.transpose() << std::endl;
    std::cout << "v_odom: " << v_odom.transpose() << std::endl;

    Eigen::Vector3d v_returned;
    v_returned = m * m2.inverse() * v_odom;

    std::cout << "v returned: " << v_returned.transpose() << std::endl;


    std::cout << "m.inverse(): " << m.inverse() << std::endl;
    std::cout << "m2.inverse(): " << m2.inverse() << std::endl;
    std::cout << "m_to_m2: " << m_to_m2 << std::endl;


    // std::cout << "m(seq(0,1),seq(0,1)): " << m.block(seq(0,1),seq(0,1)) << std::endl; 

    std::cout << "m.block(): " << m.block(0,0,2,2) << std::endl;


    std::string left_folder = "../dataset/image_l";
    std::string right_folder = "../dataset/image_r";

    cv::Mat img_left, img_right;
    cv::Mat img_merged;


    int N_IMAGES = 5;
    std::string pth_left, pth_right;
    
    visualOdometry vo;


    std::vector<cv::KeyPoint> cur_kpts_l;
    cv::Mat cur_desc_l;
    std::string write_name_l = "image_l";

    std::vector<cv::KeyPoint> cur_kpts_r;
    cv::Mat cur_desc_r;
    std::string write_name_r = "image_r";

    std::vector<std::vector<cv::DMatch>> matches;
    std::vector<cv::DMatch> good_matches;

    auto start = high_resolution_clock::now();

    for(int i=1; i<=N_IMAGES; i++)
    {
        pth_left = left_folder + "/" + std::to_string(i) + ".png";
        pth_right = right_folder + "/" + std::to_string(i) + ".png";

        std::cout << pth_left << std::endl;

        img_left = cv::imread(pth_left);
        img_right = cv::imread(pth_right);




        cv::cvtColor(img_left, img_left, cv::COLOR_BGR2RGB);
        cv::cvtColor(img_right, img_right, cv::COLOR_BGR2RGB);



        // cv::imwrite("left_image.png",img_left);
        // cv::imwrite("right_image.png",img_right);


        vo.find_features(img_left, cur_kpts_l, cur_desc_l, true);
        vo.find_features(img_right, cur_kpts_r, cur_desc_r, true);

        std::cout << "n features(left): " << cur_kpts_l.size() << std::endl;
        std::cout << "n features(right): " << cur_kpts_r.size() << std::endl;
        
        cv::imwrite(write_name_l + std::to_string(i) + ".png",img_left);
        cv::imwrite(write_name_r + std::to_string(i) + ".png",img_right);


        vo.match_features(cur_desc_l, cur_desc_r, matches, good_matches);

        vo.visualize_matches(img_left, cur_kpts_l, img_right, cur_kpts_r, good_matches, img_merged);
        cv::imwrite("img_merged" + std::to_string(i) + ".png", img_merged);

    }

    auto end = high_resolution_clock::now();

    auto duration = duration_cast<seconds>(end-start);

    std::cout << "duration: " << duration.count() << std::endl;





    return 0;
}

