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
        Mat temp0, temp1;
        // 1. Convert color to gray
        cvtColor(frame, temp0, COLOR_BGR2GRAY);
        // 2. Reverse gray image
        temp1 = 255 - temp0;
        // 3. Apply gaussian blur to reversed image
        GaussianBlur(temp1, temp1, Size(13, 13), 0);
        // 4. Color Dodge
        dst.create(frame.size(), CV_8UC1);
        int width = frame.cols;
        int height = frame.rows;
        for(int j = 0; j < height; j++)
        {
            uchar* ptr0 = temp0.ptr<uchar>(j);
            uchar* ptr1 = temp1.ptr<uchar>(j);
            uchar* ptr = dst.ptr<uchar>(j);
            for(int i = 0; i < width; i++)
            {
                int val0 = ptr0[i];
                int val1 = ptr1[i];
                ptr[i] = min((val0 << 8) / (255 - val1 + 1e-5), 255.);
            }

        }

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
