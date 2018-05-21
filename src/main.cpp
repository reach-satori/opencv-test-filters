
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <opencv2/opencv.hpp>
#include "SpreadFilter.hpp"
#include "RadialFilter.hpp"

/* struct s_spreadFilter { */
/*     int xspr; */
/*     int yspr; */
/*     cv::Mat *img; */
/* }; */

void printHelp() {
        printf("usage: DisplayImage.out [-cam | -img] [0 | 1 | 2] [path/to/file]\n");
        printf("-img tells the program you'll provide a path to an image as a 4th argument.\n");
        printf("-cam will bring up video feed from a camera to capture the image on the spot.\n");
        printf("the third argument, 0, 1 or 2, defines which filter you will use.\n");
        printf("0 for radial filter, 1 for spread filter. (2 not implemented yet)\n");
}

/* static void radFilterCallback(int value, void* imgPtr) { */
/*     cv::Mat *origImg = static_cast<cv::Mat*>(imgPtr); */
/*     imgPtr = NULL; // dunno if i need this but just to be sure */
/*     cv::Mat filteredImg; */
/*     filteredImg = radialFilter(*origImg, value, true); */
/*     cv::imshow("Image", filteredImg); */
/* } */

/* static void sprFilterCallback(int value, void* vparamsPtr) { */
/*     s_spreadFilter *paramsPtr = static_cast<s_spreadFilter*>(vparamsPtr); */
/*     vparamsPtr = NULL; */
/*     cv::Mat filteredImg; */
/*     filteredImg = spreadFilter(*(paramsPtr->img), paramsPtr->xspr, paramsPtr->yspr); */
/*     cv::imshow("Image", filteredImg); */
/* } */

void runFilter(int filterFlag, cv::Mat *ptrToImg) {
    /* cv::Mat origImg = *static_cast<cv::Mat*>(vptrToImg); */
    cv::namedWindow("Image", cv::WINDOW_AUTOSIZE | cv::WINDOW_GUI_EXPANDED);
    switch(filterFlag) {
        case 0:
            {
            int value = 5;
            void *vptrToImg = static_cast<void*>(ptrToImg);
            cv::createTrackbar("intensity", "Image", &value, 255, radFilterCallback, vptrToImg);
            radFilterCallback(value, vptrToImg);
            while((cv::waitKey(100) & 0xEFFFFF) != 27);
            }
            break;
        case 1:
            {
            s_spreadFilter params;
            params.xspr = 5;
            params.yspr = 5;
            params.img = ptrToImg;
            void *ptrToParams = &params;
            cv::createTrackbar("X spread", "Image", &(params.xspr), 255, sprFilterCallback, ptrToParams);
            cv::createTrackbar("Y spread", "Image", &(params.yspr), 255, sprFilterCallback, ptrToParams);
            sprFilterCallback(5, ptrToParams);
            while((cv::waitKey(100) & 0xEFFFFF) != 27);
            }
            break;
    }
}


int main(int argc, char* argv[])
{

    int filterFlag;
    if( argc != 3 && argc != 4) {
        printHelp();
        return -1;
    }
    std::string args[4];
    for(int i = 0; i<argc; i++) {
        args[i] = argv[i];
    }
    if((argc == 4 && args[1] == "-cam") || (argc == 3 && args[1] == "-img")) {
        printHelp();
        return -1;
    }
    if(sscanf(argv[2], "%d", &filterFlag)<=0) {
        printHelp();
        return -1;
    }
    if(filterFlag != 0 && filterFlag != 1 && filterFlag != 2) {
        printHelp();
        return -1;
    }
    if (args[1] == "-cam") {
        printHelp();
        printf("unimplemented");
        return -1;
    }



    cv::Mat origImg;
    origImg = cv::imread(argv[3], cv::IMREAD_COLOR);

    if ( !origImg.data )
    {
        printf("No image data \n");
        return -1;
    }

    /* image = spreadFilter(image, xspr, yspr); */
    /* image = radialFilter(image, intensity, circflag); */
    /* cv::imwrite("test.jpg", image); */
    cv::Mat *ptrToImg = &origImg;
    runFilter(filterFlag, ptrToImg);

    return 0;
}
