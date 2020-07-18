#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/imgproc.hpp"
#include <string>
#include <iostream>
#include <iomanip>

using namespace std;
using namespace cv;


void VJdetectAndDraw( Mat& img, CascadeClassifier& cascade, double scale )
{
    double t = 0;
    vector<Rect> faces;
    TickMeter total;
    ostringstream osdtext;
    const static Scalar colors[] =
    {
        Scalar(255,0,0),
        Scalar(255,128,0),
        Scalar(255,255,0),
        Scalar(0,255,0),
        Scalar(0,128,255),
        Scalar(0,255,255),
        Scalar(0,0,255),
        Scalar(255,0,255)
    };
    Mat gray, smallImg;

    cvtColor( img, gray, COLOR_BGR2GRAY );
    double fx = 1 / scale;
    resize( gray, smallImg, Size(), fx, fx, INTER_LINEAR_EXACT );
    equalizeHist( smallImg, smallImg );

    total.start();
    cascade.detectMultiScale( smallImg, faces,
                              1.1, 2, CASCADE_SCALE_IMAGE, Size(30, 30) );

    /* Calculate processing time */
    total.stop();
    double msec = total.getTimeMilli();
    total.reset();

    /* Format text on result window */
    osdtext << "Processing time " << setiosflags(ios::fixed) << setprecision(3) << msec << " ms";
    putText(img, osdtext.str(), Point(25, 25), FONT_HERSHEY_DUPLEX, 1, Scalar(0, 0, 255), 2);

    /* Draw face positions */
    for ( size_t i = 0; i < faces.size(); i++ )
    {
        Rect r = faces[i];
        Scalar color = colors[i%8];
        rectangle( img, Point(cvRound(r.x*scale), cvRound(r.y*scale)),
                   Point(cvRound((r.x + r.width-1)*scale), cvRound((r.y + r.height-1)*scale)),
                   color, 3, 8, 0);
    }

    imshow( "Face Detection", img );
}



int main(int argc, char *argv[])
{
    CascadeClassifier cascade;
    double scale=1;

    /* Set USB camera as the video capture device */
    VideoCapture cap(0);
    if(!cap.isOpened())
    {
        cout << "Cannot open a camera" << endl;
        return -4;
    }

    /* Load model */
    if (!cascade.load("haarcascade_frontalface_alt.xml"))
    {
        cerr << "ERROR: Could not load classifier cascade" << endl;
        return -1;
    }

    /* Frame capture and image processing */
    for(;;)
    {
        Mat frame;
        ostringstream osdtext;

        /* Get frame from capture device */
        cap >> frame;
        if (frame.empty())
        {
            cout << "End of video stream" << endl;
            break;
        }

        Mat image = frame.clone();

        /* Detect and draw face positions  */
        VJdetectAndDraw(image, cascade, scale);

        /* Exit when Esc is detected */
        char c = (char)waitKey(30);
        if (c == 27)
            break;
    }
    return 0;

}
