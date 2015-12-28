#include <iostream>
#include <cv.h>
#include <highgui.h>

using namespace std;
using namespace cv;



IplImage* DrawHistogram(CvHistogram *hist, float scaleX=5, float scaleY=5)
{
    float histMax = 0;
    cvGetMinMaxHistValue(hist, 0, &histMax, 0, 0);

    IplImage *imgHist = cvCreateImage(cvSize(256*scaleX, 64*scaleY), 8, 1);
    cvZero(imgHist);

    for(int i=0; i<255; i++)
    {
        float histValue = cvQueryHistValue_1D(hist, i);
        float nextValue = cvQueryHistValue_1D(hist, i+1);

        CvPoint pt1 = cvPoint(i*scaleX, 64*scaleY);
        CvPoint pt2 = cvPoint(i*scaleX+scaleX, 64*scaleY);
        CvPoint pt3 = cvPoint(i*scaleX+scaleX, (64-nextValue*64/histMax)*scaleY);
        CvPoint pt4 = cvPoint(i*scaleX, (64-histValue*64/histMax)*scaleY);

        int numPts = 5;
        CvPoint pts[] = {pt1,pt2,pt3,pt4,pt1};

        cvFillConvexPoly(imgHist, pts, numPts, cvScalar(255));
    }

    return imgHist;
}

int main()
{
    IplImage *image1 = cvLoadImage("hepa10.jpg");
    IplImage *image2 = cvCloneImage(image1);
    IplImage *image3 = cvCloneImage(image1);
    cvShowImage("Original", image1);

    IplImage *imgR = cvCreateImage(cvGetSize(image1), IPL_DEPTH_8U, 1);
    IplImage *imgG = cvCreateImage(cvGetSize(image1), IPL_DEPTH_8U, 1);
    IplImage *imgB = cvCreateImage(cvGetSize(image1), IPL_DEPTH_8U, 1);

    IplImage *imgR1 = cvCreateImage(cvGetSize(image2), IPL_DEPTH_8U, 1);
    IplImage *imgG1 = cvCreateImage(cvGetSize(image2), IPL_DEPTH_8U, 1);
    IplImage *imgB1 = cvCreateImage(cvGetSize(image2), IPL_DEPTH_8U, 1);

    IplImage *imgR2 = cvCreateImage(cvGetSize(image2), IPL_DEPTH_8U, 1);
    IplImage *imgG2 = cvCreateImage(cvGetSize(image2), IPL_DEPTH_8U, 1);
    IplImage *imgB2 = cvCreateImage(cvGetSize(image2), IPL_DEPTH_8U, 1);

    cvCvtColor(image1,image1,CV_RGB2HSV);
    cvCvtColor(image2,image2,CV_RGB2HSV);
    cvCvtColor(image3,image3,CV_RGB2HSV);

    cvSplit(image1, imgB, imgG, imgR, NULL);
    cvSplit(image2, imgB1, imgG1, imgR1, NULL);
    cvSplit(image3, imgB2, imgG2, imgR2, NULL);

    //cvThreshold(imgR,imgR,100,255,CV_THRESH_BINARY);
    //cvThreshold(imgG,imgG,100,255,CV_THRESH_BINARY);
    //cvThreshold(imgB,imgB,100,255,CV_THRESH_BINARY);
    //cvThreshold(imgGray,imgGray,100,255,CV_THRESH_BINARY);

    //cvEqualizeHist(imgB,imgB);
    //cvEqualizeHist(imgG,imgG);
    //cvEqualizeHist(imgR,imgR);

    Mat imgBlue(imgB);
    Mat imgGreen(imgG);
    Mat imgRed(imgR);
    Mat img1(image1);

    // Create Trackbar
    cvNamedWindow("Control",CV_WINDOW_AUTOSIZE);
    int lowB=20, lowG=100, lowR=100;
    int highB=100, highG=255, highR=255;

    cvCreateTrackbar("Low Red","Control",&lowR,255);
    cvCreateTrackbar("High Red","Control",&highR,255);

    cvCreateTrackbar("Low Green","Control",&lowG,255);
    cvCreateTrackbar("High Green","Control",&highG,255);

    cvCreateTrackbar("Low Blue","Control",&lowB,255);
    cvCreateTrackbar("High Blue","Control",&highB,255);

    Mat imgBlue1(imgB1);
    Mat imgGreen1(imgG1);
    Mat imgRed1(imgR1);
    Mat img2(image2);

    Mat imgBlue1F(imgB2);
    Mat imgGreen1F(imgG2);
    Mat imgRed1F(imgR2);
    Mat img3(image3);
    Mat img3Final;

    while(true){
        /* First way */
        inRange(img3,Scalar(lowB,lowG,lowR),Scalar(highB,highG,highR),img3Final);
        imshow("Final Custom",img3Final);

        /* Second way
        inRange(imgBlue1,lowB,highB,imgBlue1F);
        inRange(imgGreen1,lowG,highG,imgGreen1F);
        inRange(imgRed1,lowR,highR,imgRed1F);

        cvMerge(imgB2,imgG2,imgR2,NULL,image3);
        cvShowImage("Final Custom",image3);
        */

        if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
        {
            cout << "esc key is pressed by user" << endl;
            break;
        }
    }

    inRange(imgBlue,lowB,highB,imgBlue);
    inRange(imgGreen,lowG,highG,imgGreen);
    inRange(imgRed,lowR,highR,imgRed);

    cvMerge(imgB,imgG,imgR,NULL,image1);
    cvShowImage("Final 1",image1);

    /*
    cvShowImage("Red1", imgR);
    cvShowImage("Green1", imgG);
    cvShowImage("Blue1", imgB);
    cvShowImage("Grayscale1", imgGray);
    */

    cvWaitKey(0);

    return 0;
}
