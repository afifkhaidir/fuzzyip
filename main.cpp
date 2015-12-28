#include <iostream>
#include <cv.h>
#include <highgui.h>

using namespace std;
using namespace cv;

int findCategory(int maxValue)
{
    int rendahLow = 100, rendahPeek = 130, rendahHigh = 160;
    int sedangLow = 130, sedangPeek = 160, sedangHigh = 190;
    int tinggiLow = 160, tinggiPeek = 190;

    int category=0;
    float miu=0,maxMiu=0;

    // Kategori 1: Rendah
    if(maxValue>=rendahLow && maxValue<rendahHigh)
    {
        if(maxValue<=rendahPeek)
            miu = (float)(maxValue - rendahLow)/(rendahPeek - rendahLow);
        else
            miu = (float)(maxValue - rendahHigh)/(rendahPeek - rendahHigh);
        if(miu>maxMiu)
        {
            maxMiu = miu;
            category = 1;
        }
    }

    // Kategori 2: Sedang
    if(maxValue>=sedangLow && maxValue<sedangHigh)
    {
        if(maxValue<=sedangPeek)
            miu = (float)(maxValue - sedangLow)/(sedangPeek - sedangLow);
        else
            miu = (float)(maxValue - sedangHigh)/(sedangPeek - sedangHigh);
        if(miu>maxMiu)
        {
            maxMiu = miu;
            category = 2;
        }
    }

    // Kategori 3: Tinggi
    if(maxValue>=tinggiLow && maxValue<=255)
    {
        if(maxValue<=tinggiPeek)
            miu = (float)(maxValue - tinggiLow)/(tinggiPeek - tinggiLow);
        else
            miu = 1;
        if(miu>maxMiu)
        {
            maxMiu = miu;
            category = 3;
        }
    }

    return category;
}

int findMaxValue(CvHistogram *hist)
{
    float maxValue = 0;
    int lightVal;
    for(int i=1; i<255; i++)
    {
        float histValue = cvQueryHistValue_1D(hist, i);
        if(histValue > maxValue){
            maxValue = histValue;
            lightVal = i;
        }
        cout << "value " << i << ": " << histValue << endl;
    }
    cout << endl << "Maximum Value at " << lightVal << "= " << maxValue << endl;

    return lightVal;
}

int main()
{
    IplImage *image1 = cvLoadImage("images/hepa6.jpg");
    IplImage *image2 = cvCloneImage(image1);
    cvShowImage("Original", image1);

    /* Convert original image into HSV color */
    cvCvtColor(image2,image2,CV_BGR2HSV);

    /* Create Trackbar */
    cvNamedWindow("Control",CV_WINDOW_AUTOSIZE);

    int lowH=20, lowS=100, lowV=100;
    int highH=100, highS=255, highV=255;

    cvCreateTrackbar("Low Hue","Control",&lowH,179);
    cvCreateTrackbar("High Hue","Control",&highH,179);

    cvCreateTrackbar("Low Saturation","Control",&lowS,255);
    cvCreateTrackbar("High Saturation","Control",&highS,255);

    cvCreateTrackbar("Low Value","Control",&lowV,255);
    cvCreateTrackbar("High Value","Control",&highV,255);


    /* Create matrix for image thresholding */
    Mat img2(image2);
    Mat mask;

    while(true)
    {
        /* Thresholding */
        inRange(img2,Scalar(lowH,lowS,lowV),Scalar(highH,highS,highV),mask);
        imshow("Mask",mask);

        if (waitKey(30) == 27) // Press escape if thresholding process done
        {
            cout << "esc key is pressed by user" << endl;
            break;
        }
    }

    /* Bitwise AND between original image and mask */
    IplImage *result = cvCreateImage(cvGetSize(image1), IPL_DEPTH_8U, 3);
    Mat img1(image1);
    Mat res(result);
    bitwise_and(img1,img1,res,mask= mask);

    cvShowImage("Result",result);

    /* Split image into HSV */
    IplImage *imgH = cvCreateImage(cvGetSize(image1), IPL_DEPTH_8U, 1);
    IplImage *imgS = cvCreateImage(cvGetSize(image1), IPL_DEPTH_8U, 1);
    IplImage *imgV = cvCreateImage(cvGetSize(image1), IPL_DEPTH_8U, 1);

    cvSplit(result, imgH, imgS, imgV, NULL);

    /* Calculate maximum value */
    int numBins = 256;
    float range[] = {0,255};
    float *ranges[] = {range};

    CvHistogram *hist = cvCreateHist(1, &numBins, CV_HIST_ARRAY, ranges, 1);
    cvClearHist(hist);

    cvCalcHist(&imgV, hist, 0, 0);
    int maxImgValue = findMaxValue(hist);

    int category = findCategory(maxImgValue);
    cout << endl << "Category: " << category << endl;

    cvWaitKey(0);

    return 0;
}
