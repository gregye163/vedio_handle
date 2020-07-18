#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/imgproc.hpp"
#include <string>
#include <iostream>
#include <iomanip>

using namespace std;
using namespace cv;


int main(int argc, char *argv[])
{
    /* Set USB camera as the video capture device */
    VideoCapture cap(0);
    if(!cap.isOpened())
    {
        cout << "Cannot open a camera" << endl;
        return -4;
    }

    /* Frame capture and image processing */
    for(;;)
    {
        Mat frame, dst;
        TickMeter total;
        ostringstream osdtext;

        /* Get frame from capture device */
        cap >> frame;
        if (frame.empty())
        {
            cout << "End of video stream" << endl;
            break;
        }


        total.start();

        /* Image Processing */
        // 3x3 convolution kernel
        // 1, 0, 0
        // 0, 0, 0
        // 0, 0, -1
        Mat kernel = Mat::zeros(3, 3, CV_8SC1);
        kernel.at<signed char>(0,0) = 1;
        kernel.at<signed char>(2,2) = -1;
        filter2D(frame, dst, -1, kernel, Point(-1,-1), 128);

        /* Calculate processing time */
        total.stop();
        double msec = total.getTimeMilli();
        total.reset();

        /* Formatting text on result window */
        osdtext << "Processing time " << setiosflags(ios::fixed) << setprecision(3) << msec << " ms";
        putText(dst, osdtext.str(), Point(25, 25), FONT_HERSHEY_DUPLEX, 1, Scalar(0, 0, 255), 2);

        /* Show images */
        imshow("Camera Input", frame);
        imshow("Processed Frames", dst);

        /* Exit when Esc is detected */
        char c = (char)waitKey(30);
        if (c == 27)
            break;
    }
    return 0;

}
