#include <opencv2/opencv.hpp>
#include <math.h>
#include "RadialFilter.hpp"

double getDistance(cv::Point2f p1, cv::Point2f p2) {
    double dist = sqrt(pow(p1.x - p2.x, 2) + pow((p1.y - p2.y), 2));
    return dist;
}

void radFilterTrackCallback(int value, void* paramPtr) {
    s_radialFilter *params = static_cast<s_radialFilter*>(paramPtr);
    paramPtr = NULL; // dunno if i need this but just to be sure
    cv::Mat filteredImg;
    filteredImg = radialFilter(*(params->img), params->intensity, params->cx, params->cy);
    cv::imshow("Image", filteredImg);
}


cv::Mat radialFilter(cv::Mat origImg, int intensity, int cx, int cy) {
    //tranformation to logpolar image: center is center of image, radius is half of the diagonal i.e. catches whole image
    if (intensity == 0) intensity = 1;
    cv::Point2f center( cx, cy) ;

    //everything up to polarOut declaration is just to get the max radius (calculated as farthest diagonal of the image)
    //TODO? put in its own function
    double maxRadius;
    int right = 0;
    int down = 0;
    if (cx > origImg.cols/2) {right = 1;}
    if (cy > origImg.rows/2) {down = 2;}
    int result = right + down;
    //upper left quadrant: 0
    //upper right quadrant: 1
    //lower left quadrant: 2
    //lower right quadrant: 3
    cv::Point2f furthestPoint;

    switch(result) {
        case 0:
            furthestPoint = cv::Point2f(origImg.cols, origImg.rows);
            break;
        case 1:
            furthestPoint = cv::Point2f(0.0, origImg.rows);
            break;
        case 2:
            furthestPoint = cv::Point2f(origImg.cols, 0.0);
            break;
        case 3:
            furthestPoint = cv::Point2f(0.0, 0.0);
            break;
    };
    maxRadius = getDistance(furthestPoint, center);

    cv::Mat polarOut;
    cv::warpPolar(origImg, polarOut, cv::Size(), center, maxRadius, cv::INTER_LINEAR + cv::WARP_POLAR_LINEAR);

    cv::Mat blurKernel(1, intensity, CV_32F, cv::Scalar(1.0/intensity));

    //channels must be split to apply filter2d
    cv::Mat splitChannels[3];
    cv::split(polarOut, splitChannels);
    for (int i = 0; i < 3; i++) {
        cv::filter2D(splitChannels[i], splitChannels[i], -1, blurKernel);
    }

    //debug
    /* cv::imwrite("b.png", splitChannels[0]); */
    /* cv::imwrite("g.png", splitChannels[1]); */
    /* cv::imwrite("r.png", splitChannels[2]); */
    cv::merge(splitChannels, 3, polarOut );

    // transform back to cartesian image reusing the input variable
    cv::warpPolar(polarOut, origImg, origImg.size(), center, maxRadius, cv::WARP_POLAR_LINEAR + cv::WARP_INVERSE_MAP + cv::INTER_LINEAR);

    return origImg;
}


