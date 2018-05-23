#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <opencv2/opencv.hpp>

#include "SpreadFilter.hpp"
#include "RadialFilter.hpp"
#include "EdgeFilter.hpp"


//TODO: put argument processing in a function
//TODO: add camera capture support
//TODO: last filter

void printHelp() {
        printf("usage: DisplayImage.out [-cam | -img] [0 | 1 | 2] [path/to/file]\n");
        printf("-img tells the program you'll provide a path to an image as a 4th argument.\n");
        printf("-cam will bring up video feed from a camera to capture the image on the spot.\n");
        printf("the third argument, 0, 1 or 2, defines which filter you will use.\n");
        printf("0 for radial filter, 1 for spread filter. (2 not implemented yet)\n");
}

void runFilter(int filterFlag, cv::Mat *ptrToImg) {
    cv::namedWindow("Image", cv::WINDOW_AUTOSIZE | cv::WINDOW_GUI_EXPANDED);
    switch(filterFlag) {
        case 0: {
            s_radialFilter params;
            radFilterInit(ptrToImg, params);
        }
            break;

        case 1: {
            s_spreadFilter params;
            sprFilterInit(ptrToImg, params);
        }
            break;
        case 2: {
            s_edgeFilter params;
            edgeFilterInit(ptrToImg, params);
        }
            break;
    }
        while((cv::waitKey(100) & 0xEFFFFF) != 27);
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

    cv::Mat *ptrToImg = &origImg;
    runFilter(filterFlag, ptrToImg);

    return 0;
}
