#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;
int main()
{
VideoCapture
cap("/home/ayush/moveit_ws/src/lbfgs/images/expt_video4.mp4");
// if not success, exit program
if (cap.isOpened() == false)
{
cout << "Cannot open the video camera" << endl;
cin.get(); //wait for any key press
return -1;
}
double dWidth = cap.get(CAP_PROP_FRAME_WIDTH);
double dHeight = cap.get(CAP_PROP_FRAME_HEIGHT);
Size frame_size(dWidth, dHeight);
double fps = cap.get(CAP_PROP_FPS);
cout << "Frames per seconds : " << fps << endl;
cout << "Resolution of the video : " << dWidth << " x " << dHeight <<
endl;
Mat past_frame;
int count = 0;
int frame_count = 0;
vector<pair<Rect, int>> rectContours;
vector<pair<vector<Point>, int>> contours_list;
double x_prop_factor = 162.0/787.0;
double y_prop_factor = 96.5/468.0
cout << "x_prop = " << x_prop_factor << " y_prop = " <<
y_prop_factor << endl;
while (true)
{
Mat frame, diffimage0;
Mat output;
Mat diffimage, thresholdimage;
bool bSuccess = cap.read(frame); // read a new frame from video
resize(frame, frame, Size(1156, 651));
//Breaking the while loop if the frames cannot be captured
if (bSuccess == false)
{
cout << "Video camera is disconnected" << endl;
cin.get(); //Wait for any key press
break;
}
if(count == 0)
{
imwrite("/home/ayush/moveit_ws/src/lbfgs/images/blackboard.jpg",
frame);
cvtColor(frame, past_frame, COLOR_RGB2GRAY);
count++;
continue;
}
if(frame_count == 10)
{
cvtColor(frame, output, COLOR_RGB2GRAY);
//cvtColor(frame, frame, COLOR_RGB2GRAY);
absdiff(output, past_frame, diffimage);
//cvtColor(diffimage, diffimage, COLOR_RGB2GRAY);
threshold(diffimage, thresholdimage, 40, 255, THRESH_BINARY);
//bilateralFilter(diffimage, diffimage0, 20, 120, 120);
medianBlur(thresholdimage, diffimage0, 5);
vector<vector<Point>> contours;
findContours(diffimage0, contours, RETR_EXTERNAL,
CHAIN_APPROX_SIMPLE);
vector<vector<Point>> contours_poly(contours.size());
vector<Rect> boundRect(contours.size());
int rectArea = 0;
Rect maxRect;
bool change_flag = false;
for( size_t i = 0; i < contours.size(); i++ )
{
//cout << "no. of points in original contour = " <<
contours[i].size() << " ";
approxPolyDP(contours[i], contours_poly[i], 5, true);
boundRect[i] = boundingRect(contours_poly[i]);
if(boundRect[i].height*boundRect[i].width > rectArea &&
boundRect[i].height*boundRect[i].width > 100)
{
maxRect = boundRect[i];
rectArea = boundRect[i].height*boundRect[i].width;
change_flag = true;
}
//cout << contours_poly[i].size() << endl;
contours_list.push_back(make_pair(contours_poly[i], 20));
//rectContours.push_back(make_pair(boundRect[i], 20));
}
Mat duplicate_frame = frame.clone();
if(change_flag)
{
Point tl = maxRect.tl();
Point br = maxRect.br();
Scalar color = Scalar(255, 255, 255);
rectangle(frame, tl, br, color, 3);
float x_coord_led = (tl.x + br.x)/2;
float y_coord_led = (tl.y + br.y)/2;
cout << "Pixel Coordinates of LED = " << x_coord_led << " , " <<
y_coord_led << endl;
cout << "World Coordinates of LED = " << (x_coord_led -
131.0)*x_prop_factor << " , " << (630.0 - y_coord_led)*y_prop_factor
<< endl;
rectContours.push_back(make_pair(maxRect, 20));
}
past_frame = output;
frame_count = 0;
}
for( size_t i = 0; i< rectContours.size(); i++ )
{
Scalar color = Scalar(255, 255, 255);
//drawContours(output, contours_poly, (int)i, color, 3);
if(rectContours[i].second > 0)
{
Point tl = rectContours[i].first.tl();
Point br = rectContours[i].first.br();
rectContours[i].second = rectContours[i].second - 1;
rectangle(frame, tl, br, color, 2);
}
}
frame_count++;
//imshow(window1, frame);
//imshow(window2, diffimage);
//oVideoWriter.write(frame);
imshow("Display", frame);
if (waitKey(10) == 27)
{
cout << "Esc key is pressed by user. Stoppig the video" << endl;
break;
}
}
//oVideoWriter.release();
return 0;
}