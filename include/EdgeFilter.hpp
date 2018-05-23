
struct s_edgeFilter {
    int threshold;
    int ratio;
    //they all have to be ints to work with the opencv trackbar
    //can be changed into proper doubles inside the filter function itself
    //ratio will be between 1 and 10: (double) (ratio + 1000) / 1000; ratio limited to 0-9000
    cv::Mat* img;
};
cv::Mat edgeFilter(const cv::Mat& origImg, int ratio);
void edgeFilterInit(cv::Mat *ptrToImg, s_edgeFilter& params);
void edgeFilterCallback(int value, void* vparamsPtr);
