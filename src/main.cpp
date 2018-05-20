
#include <stdio.h>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include "SpreadFilter.hpp"

int main(int argc, char** argv )
{
    if ( argc != 2 )
    {
        printf("usage: DisplayImage.out <Image_Path>\n");
        return -1;
    }

    cv::Mat image;
    image = cv::imread( argv[1], cv::IMREAD_COLOR );

    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }

    image = spreadFilter(image, 5, 0);
    cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE );
    cv::imshow("Display Image", image);
    cv::waitKey(0);

    return 0;
}
