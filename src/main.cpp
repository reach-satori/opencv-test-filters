
#include <stdio.h>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include "SpreadFilter.hpp"
#include "RadialFilter.hpp"


static void on_trackbar(int value, void* imgptr) {
    /* if (value=0) {value = 1;} */
    cv::Mat origImg = *static_cast<cv::Mat*>(imgptr);
    cv::Mat filteredImg;
    filteredImg = radialFilter(origImg, value, true);
    cv::imshow("Display Image", filteredImg);
}

int main(int argc, char* argv[])
{
    int xspr, yspr;
    if ( argc > 4 || argc < 4 )
    {
        printf("usage: DisplayImage.out <Image_Path> <X spread> <Y spread>\n");
        return -1;
    } else if (argc = 4) {
        if (sscanf(argv[2], "%i", &xspr) != 1) {
            printf("Argument error.");
            return -1;
        }
        if (sscanf(argv[3], "%i", &yspr) != 1) {
            printf("Argument error.");
            return -1;
        }
    }

    cv::Mat origImg, filteredImg;
    origImg = cv::imread(argv[1], cv::IMREAD_COLOR);

    if ( !origImg.data )
    {
        printf("No image data \n");
        return -1;
    }

    /* image = spreadFilter(image, xspr, yspr); */
    /* image = radialFilter(image, intensity, circflag); */
    /* cv::imwrite("test.jpg", image); */
    int intensity = 1;
    bool circflag = true;
    cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE | cv::WINDOW_GUI_EXPANDED);

    int value = 5;
    bool changed = true;
    void *ptrToImg = &origImg;
    cv::createTrackbar("track1", "Display Image", &value, 255, on_trackbar, ptrToImg);
    filteredImg = radialFilter(origImg, value, true);
    cv::imshow("Display Image", filteredImg);

    while((cv::waitKey(100) & 0xEFFFFF) != 27);


    return 0;
}
