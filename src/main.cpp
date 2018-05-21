
#include <stdio.h>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include "SpreadFilter.hpp"
#include "RadialFilter.hpp"

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

    cv::Mat image;
    image = cv::imread(argv[1], cv::IMREAD_COLOR);

    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }

    /* image = spreadFilter(image, xspr, yspr); */
    int intensity = 1;
    bool circflag = true;
    image = radialFilter(image, intensity, circflag);

    cv::imwrite("test.jpg", image);
    cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE );
    cv::imshow("Display Image", image);
    while((cv::waitKey() & 0xEFFFFF) != 27);

    return 0;
}
