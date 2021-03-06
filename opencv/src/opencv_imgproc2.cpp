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
        cvtColor(frame, dst, COLOR_BGR2GRAY);

        /* Gaussian Blur */
        GaussianBlur(dst, dst, Size(3,3), 0);

        /* Canny Edge Detection */
        Canny(dst, dst, 50, 150);

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
