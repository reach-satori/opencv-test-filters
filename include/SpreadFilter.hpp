

struct s_spreadFilter {
    int xspr;
    int yspr;
    cv::Mat *img;
};

void sprFilterInit(cv::Mat *ptrToImg, s_spreadFilter& params);
int getRandint (int rangea, int rangeb);
void swapPix(cv::Mat& img, cv::Mat& checkGrid,  int h,  int w,  int xspread,  int yspread);
cv::Mat spreadFilter(cv::Mat origImage, int xspread, int yspread);
void sprFilterCallback(int value, void* vparamsPtr);
