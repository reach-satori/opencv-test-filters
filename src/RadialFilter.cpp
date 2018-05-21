#include <opencv2/opencv.hpp>
#include <math.h>
#include "RadialFilter.hpp"


cv::Mat radialFilter(cv::Mat origImg, int intensity, bool circflag) {
    //tranformation to logpolar image: center is center of image, radius is half of the diagonal i.e. catches whole image
    if (intensity == 0) intensity = 1;
    cv::Point2f center( (float)origImg.cols / 2,  (float)origImg.rows / 2);
    double maxRadius = sqrt(pow(origImg.cols, 2)+pow(origImg.rows, 2))/2;
    cv::Mat polarOut;
    cv::warpPolar(origImg, polarOut, cv::Size(), center, maxRadius, cv::INTER_LINEAR + cv::WARP_POLAR_LOG);

    cv::Mat horKernel(1, intensity, CV_32F, cv::Scalar(1.0/intensity));
    cv::Mat vertKernel(intensity, 1, CV_32F, cv::Scalar(1.0/intensity));

    //channels must be split to apply filter2d
    cv::Mat splitChannels[3];
    cv::split(polarOut, splitChannels);
    for (int i = 0; i < 3; i++) {
        cv::filter2D(splitChannels[i], splitChannels[i], -1, horKernel);
    }

    //debug
    /* cv::imwrite("b.png", splitChannels[0]); */
    /* cv::imwrite("g.png", splitChannels[1]); */
    /* cv::imwrite("r.png", splitChannels[2]); */
    cv::merge(splitChannels, 3, polarOut );


    // transform back to cartesian image
    cv::warpPolar(polarOut, origImg, origImg.size(), center, maxRadius, cv::WARP_POLAR_LOG + cv::WARP_INVERSE_MAP + cv::INTER_LINEAR);

    return origImg;
}


