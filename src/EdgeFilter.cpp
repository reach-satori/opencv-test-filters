#include <opencv2/opencv.hpp>
#include "EdgeFilter.hpp"

void edgeFilterInit(cv::Mat *ptrToImg, s_edgeFilter& params) {
    void *ptrToParams = &params;
    params.ratio = 2000;
    params.img = ptrToImg;

    cv::createTrackbar("Edge Sensitivity", "Image", &(params.ratio), 7000, edgeFilterCallback, ptrToParams);

    edgeFilterCallback(5, ptrToParams);
}

void edgeFilterCallback(int value, void* vparamsPtr){
    s_edgeFilter *paramsPtr = static_cast<s_edgeFilter*>(vparamsPtr);
    vparamsPtr = NULL;
    cv::Mat filteredImg;
    filteredImg = edgeFilter(*(paramsPtr->img), paramsPtr->ratio );
    cv::imshow("Image", filteredImg);
}


cv::Mat edgeFilter(const cv::Mat& origImg, int ratio) {

    if(ratio > 9000) {
        ratio = 9000;
    }
    if(ratio == 0) {
        ratio = 1;
    }

    cv::Mat origCopy, finalResult;
    origCopy = origImg.clone();
    cv::cvtColor(origCopy, origCopy, cv::COLOR_BGR2GRAY );

    cv::blur(origCopy, origCopy, cv::Size(3, 3));
    cv::Canny(origCopy, origCopy,(double) 2, 2*ratio, 5);

    return origCopy;
}
