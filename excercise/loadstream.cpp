#include <opencv2/opencv.hpp>
#include <iostream>  // Include for std::cout and std::endl

using namespace cv;
using namespace std;  // Now no need to prefix 'std::' with cout, endl, etc.

int main()
{
    // Create a VideoCapture object
    VideoCapture cap(0); // Open the second camera device (default is 0, 1 is usually an external camera)

    if(!cap.isOpened())
    {
        cout << "Some error occurred with the stream" << endl;
        return -1;  // Exit if the camera couldn't be opened
    }

    Mat frame;

    while(true)
    {
        cap >> frame;  // Capture the frame from the webcam
        if (frame.empty()) {
            cout << "Failed to capture image" << endl;
            break;
        }
        
        imshow("Frame", frame);  // Display the frame in a window named "Frame"

        // Exit the loop if the 'Esc' key (27) is pressed
        if(waitKey(1) == 27)
        {
            break;
        }
    }

    cap.release();  // Release the camera
    destroyAllWindows();  // Destroy all OpenCV windows

    return 0;
}
