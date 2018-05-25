#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include "EdgeFilter.hpp"

void edgeFilterInit(cv::Mat *ptrToImg, s_edgeFilter& params) {
    void *ptrToParams = &params;
    params.ratio = 2000;
    params.thickness = 1;
    params.img = ptrToImg;
    params.ncolors = 2;

    cv::createTrackbar("Edge Sensitivity", "Image", &(params.ratio), 7000, edgeFilterCallback, ptrToParams);
    cv::createTrackbar("Thickness", "Image", &(params.thickness), 20, edgeFilterCallback, ptrToParams);
    cv::createTrackbar("Number of colors", "Image", &(params.ncolors), 5, edgeFilterCallback, ptrToParams);

    edgeFilterCallback(5, ptrToParams);
}

void edgeFilterCallback(int value, void* vparamsPtr){
    s_edgeFilter *paramsPtr = static_cast<s_edgeFilter*>(vparamsPtr);
    vparamsPtr = NULL;
    cv::Mat filteredImg;
    filteredImg = edgeFilter(*(paramsPtr->img), paramsPtr->ratio, paramsPtr->thickness, paramsPtr->ncolors );
    cv::imshow("Image", filteredImg);
}

//stole this function
cv::Mat quantizeColors(cv::Mat& img, int ncolors) {
    cv::Mat opImage, output;
    output = img.clone();

    output.convertTo(opImage, CV_32F);
    opImage = opImage.reshape(3, opImage.total());

    cv::Mat labels, centers;
    cv::kmeans(opImage, ncolors, labels, cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 10, 1.0), 3,
           cv::KMEANS_PP_CENTERS, centers);

    // reshape both to a single row of Vec3f pixels:
    centers = centers.reshape(3,centers.rows);
    opImage = opImage.reshape(3,opImage.rows);

    // replace pixel values with their center value:
    cv::Vec3f *p = opImage.ptr<cv::Vec3f>();
    for (size_t i=0; i<opImage.rows; i++) {
       int center_id = labels.at<int>(i);
       p[i] = centers.at<cv::Vec3f>(center_id);
    }

    // back to 2d, and uchar:
    output = opImage.reshape(3, output.rows);
    output.convertTo(output, CV_8U);
    return output;
}


cv::Mat edgeFilter(cv::Mat& origImg, int ratio, int thickness, int ncolors) {

    if(ratio > 9000) {
        ratio = 9000;
    }
    if(ratio == 0) {
        ratio = 1;
    }
    if(thickness == 0) {
        thickness = 1;
    }
    ncolors += 3;

    cv::Mat origCopy, colorInfo;
    colorInfo.create(origImg.size(), origImg.type());
    origCopy = origImg.clone();

    colorInfo = quantizeColors(origImg, ncolors);

    cv::cvtColor(origCopy, origCopy, cv::COLOR_BGR2GRAY );
    //origCopy should be CV_8U 1channel at this point

    //threshold can be changed but for the purposes of this simple filter, it's not necessary
    //run edge detection with fixed initial threshold since finer control isn't required
    cv::blur(origCopy, origCopy, cv::Size(3, 3));
    cv::Canny(origCopy, origCopy,(double) 2, 2*ratio, 5);

    //thicken lines
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(thickness,thickness));
    cv::dilate(origCopy, origCopy, kernel);

    //take negative
    cv::cvtColor(origCopy, origCopy, cv::COLOR_GRAY2BGR);
    //subtract from quantized color
    cv::subtract(colorInfo, origCopy, origCopy);

    return origCopy;
}
