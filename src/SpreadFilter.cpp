#include <stdio.h>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include "SpreadFilter.hpp"

void sprFilterInit(cv::Mat *ptrToImg, s_spreadFilter& params) {
            params.xspr = 5;
            params.yspr = 5;
            params.img = ptrToImg;
            void *ptrToParams = &params;

            cv::createTrackbar("X spread", "Image", &(params.xspr), 100, sprFilterCallback, ptrToParams);
            cv::createTrackbar("Y spread", "Image", &(params.yspr), 100, sprFilterCallback, ptrToParams);
            sprFilterCallback(5, ptrToParams);
}

int getRandint(int rangea, int rangeb) {
    int out = rand()%((rangeb - rangea) + 1) + rangea;
    return out;
}

void sprFilterCallback(int value, void* vparamsPtr) {
    s_spreadFilter *paramsPtr = static_cast<s_spreadFilter*>(vparamsPtr);
    vparamsPtr = NULL;
    cv::Mat filteredImg;
    filteredImg = spreadFilter(*(paramsPtr->img), paramsPtr->xspr, paramsPtr->yspr);
    cv::imshow("Image", filteredImg);
}

void swapPix(cv::Mat& img, cv::Mat& checkGrid,  int h,  int w,  int xspread,  int yspread) {
    //clamping and getting random bound values
    int xminbound = ((w - xspread) > 0) ? w - xspread : 0;
    int xmaxbound = ((w + xspread) < img.cols) ? w + xspread : img.cols-1;
    int yminbound = ((h - yspread) > 0) ? h - yspread : 0;
    int ymaxbound = ((h + yspread) < img.rows) ? h + yspread : img.rows-1;

    //get initial pixel to be switched and store its position in a temp var
    cv::Vec3b& fixedPix = img.at<cv::Vec3b>(h, w);
    const cv::Vec3b tempPix = cv::Vec3b(fixedPix[0], fixedPix[1], fixedPix[2]);
    //getting random pixel in given range
    int yrand = getRandint(yminbound, ymaxbound);
    int xrand = getRandint(xminbound, xmaxbound);
    cv::Vec3b& randomizedPix = img.at<cv::Vec3b>(yrand, xrand);

    //flag chosen pixels as already swapped
    checkGrid.at<unsigned char>(yrand, xrand) = 255;
    checkGrid.at<unsigned char>(h, w) = 255;

    //swap
    for(int i = 0; i < 3; i++) {
        fixedPix[i] = randomizedPix[i];
        randomizedPix[i] = tempPix[i];
    }
};

cv::Mat spreadFilter(cv::Mat origImage, int xspread, int yspread) {
    //spreadFilter swaps pixels at a max distance of xspread horizontally, yspread vertically.
    //checkgrid has same size as origImage, initially all values 0. When swapped on the original image,
    //the swapped pixels become 255 as a flag not to be swapped again
    int width = origImage.cols;
    int height = origImage.rows;

    //strange, if i don't do this the image gets changed in-place
    //i thought arguments were passed by value: origImage isn't passed by ref or pointer so it should
    //be a copy already. Might be related to the fact that assigning a mat with = just does a shallow copy in ocv
    cv::Mat copyImage = origImage.clone();

    //debug
    /* printf("OPENCV:%d\n", CV_MAJOR_VERSION); */
    /* printf("size: w:%d h:%d\n", width, height);//RIGHT */
    /* printf("channels: %d depth: %d\n", origImage.channels(), origImage.depth()); */
    srand(time(NULL));

    cv::Mat checkGrid = cv::Mat(height, width, CV_8UC1, cv::Scalar(0));
    for(int w = 0; w < width; w++){
        for(int h = 0; h < height; h++) {
            if(checkGrid.at<unsigned char>(h, w)){
                continue;
            } else {
                swapPix(copyImage, checkGrid, h, w, xspread, yspread);
            };
        }
    }
    return copyImage;
};

