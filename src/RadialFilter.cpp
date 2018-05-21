#include <opencv2/opencv.hpp>
#include <math.h>
#include "RadialFilter.hpp"


cv::Mat radialFilter(cv::Mat origImg, int intensity, bool circflag) {
    //tranformation to logpolar image: center is center of image, radius is half of the diagonal i.e. catches whole image
    cv::Point2f center( (float)origImg.cols / 2,  (float)origImg.rows / 2);
    double maxRadius = sqrt(pow(origImg.cols, 2)+pow(origImg.rows, 2))/2;
    cv::Mat polarOut;
    cv::warpPolar(origImg, polarOut, cv::Size(), center, maxRadius, cv::INTER_LINEAR + cv::WARP_POLAR_LOG);

    cv::Mat splitChannels[3];
    cv::split(polarOut, splitChannels);

    cv::Mat horKernel(1, 20, CV_32F, cv::Scalar(1.0/20));
    cv::Mat vertKernel(20, 1, CV_32F, cv::Scalar(1.0/20));

    for (int i = 0; i < 3; i++) {
        cv::filter2D(splitChannels[i], splitChannels[i], -1, horKernel);
    }
    //debug
    /* cv::imwrite("b.png", splitChannels[0]); */
    /* cv::imwrite("g.png", splitChannels[1]); */
    /* cv::imwrite("r.png", splitChannels[2]); */
    cv::merge(splitChannels, 3, polarOut );

    cv::warpPolar(polarOut, origImg, origImg.size(), center, maxRadius, cv::WARP_POLAR_LOG + cv::WARP_INVERSE_MAP + cv::INTER_LINEAR);

    return origImg;
}


