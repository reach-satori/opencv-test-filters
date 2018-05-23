
struct s_radialFilter {
    int cx;
    int cy;
    int intensity;
    cv::Mat *img;
};


void radFilterInit(cv::Mat *ptrToImg, s_radialFilter& params);
double getDistance(cv::Point2f p1, cv::Point2f p2);
cv::Mat radialFilter(cv::Mat origImg, int intensity, int cx, int fy);
void radFilterTrackCallback(int value, void* paramPtr);
