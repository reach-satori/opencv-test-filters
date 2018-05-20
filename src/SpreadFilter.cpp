#include <stdio.h>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include "SpreadFilter.hpp"

int getRandint (int rangea, int rangeb) {
    int out = rand()%((rangeb - rangea) + 1) + rangea;
    return out;
}

void swapPix(cv::Mat& img, cv::Mat& checkGrid,  int h,  int w,  int xspread,  int yspread) {

    cv::Vec3b& fixedPix = img.at<cv::Vec3b>(h, w);
    const cv::Vec3b tempPix = cv::Vec3b(fixedPix[0], fixedPix[1], fixedPix[2]);

    //clamping random bound values
    int xminbound = ((w - xspread) > 0) ? w - xspread : 0;
    int xmaxbound = ((w + xspread) < img.cols) ? w + xspread : img.cols-1;
    int yminbound = ((h - yspread) > 0) ? h - yspread : 0;
    int ymaxbound = ((h + yspread) < img.rows) ? h + yspread : img.rows-1;

    /* //getting random pixel in given range */
    int yrand = getRandint(yminbound, ymaxbound);
    int xrand = getRandint(xminbound, xmaxbound);
    cv::Vec3b& randomizedPix = img.at<cv::Vec3b>(yrand, xrand);

    //flag as already swapped
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
    //checkgrid has same size as origImage, initially all values 0. When swapped on the original image, the swapped pixels become 1 in the checkGrid
    int width = origImage.cols;
    int height = origImage.rows;
    printf("OPENCV:%d\n", CV_MAJOR_VERSION);
    printf("size: w:%d h:%d\n", width, height);//RIGHT
    printf("channels: %d depth: %d\n", origImage.channels(), origImage.depth());
    srand(time(NULL));

    cv::Mat checkGrid = cv::Mat(height, width, CV_8UC1, cv::Scalar(0));
    for(int w = 0; w < width; w++){
        for(int h = 0; h < height; h++) {
            /* printf("%d\n", w); */
            if(checkGrid.at<unsigned char>(h, w)){
                continue;
            } else {
                swapPix(origImage, checkGrid, h, w, xspread, yspread);
            };
        }
    }
    return origImage;
};

