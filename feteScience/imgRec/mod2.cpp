#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "variables.h"
#include "recContours.h"
#include "pathfinding.h"
#include <string>

using namespace cv;
using namespace std;

string window_name2 = "Params";
bool isRunning = true;
Mat imgs[4];

//hide the local functions in an anon namespace
namespace {
    void help(char** av) {
        cout << "The program captures frames from a video file, image sequence (01.jpg, 02.jpg ... 10.jpg) or camera connected to your computer." << endl
             << "Usage:\n" << av[0] << " <video file, image sequence or device number>" << endl
             << "q,Q,esc -- quit" << endl
             << "space   -- save frame" << endl << endl
             << "\tTo capture from a camera pass the device number. To find the device number, try ls /dev/video*" << endl
             << "\texample: " << av[0] << " 0" << endl
             << "\tYou may also pass a video file instead of a device number" << endl
             << "\texample: " << av[0] << " video.avi" << endl
             << "\tYou can also pass the path to an image sequence and OpenCV will treat the sequence just like a video." << endl
             << "\texample: " << av[0] << " right%%02d.jpg" << endl;
    }

void onMouse(int event, int x, int y, int, void*)
{
	if(event == EVENT_LBUTTONDOWN)
	{
		clickPoint = Point(x,y);
		selectedI = selectedContour(clickPoint);
		drawStuff(0,0);
	}
	else if(event == EVENT_RBUTTONDOWN)
	{
		selectedI = -1;
		drawStuff(0,0);
	}
	else if(event == EVENT_LBUTTONDBLCLK)
	{
		pathfinding(offsetLenght);
	}
	else return;
}

void ini(){
	namedWindow(window_name2, WINDOW_AUTOSIZE); // nouvelle fenêtre
	cv::setWindowProperty(window_name2, WINDOW_AUTOSIZE, WINDOW_NORMAL);	
	resizeWindow(window_name2, 400,200);

	namedWindow(window_name, WINDOW_NORMAL); // nouvelle fenêtre
	cv::setWindowProperty(window_name, WINDOW_FREERATIO, WINDOW_NORMAL);	
		
	cv::createTrackbar("Minimum de Canny :", window_name2, &lowThreshold, max_lowThreshold, drawStuff ); // barre du pas pour Canny
	cv::createTrackbar("Taille de la bande de Canny :", window_name2, &hystSize, max_hystSize, drawStuff );
	cv::createTrackbar("Reduction du bruit :", window_name2, &lowNegligeable, max_Negligeable, drawStuff ); // barre du pourcentage d'aire négligeable pour nettoyage
	cv::createTrackbar("Approximation :", window_name2, &approxEpsilonInt, 50, drawStuff );
	cv::createTrackbar("Filtre - 0=Image de depart\n\t, 1=Canny, \n\t2=Contours :", window_name2, &displayedPicture, 2, displayPicture); // image affichée
	
	cv::moveWindow(window_name2,0,0);
	
	cv::setMouseCallback(window_name, onMouse, 0);	
	
//	cv::waitKey(0);

}

Mat moyImgs(){
	uchar R,G,B;
	Mat s = img_rgb.clone();
	for(int y; y<frame.row;y++){
		for( int x ; x<frame.cols ; x++ )
		{
			Vec3b intensity0 = imgs[0].at<Vec3b>(y,x);
			Vec3b intensity1 = imgs[1].at<Vec3b>(y,x);
			Vec3b intensity2 = imgs[2].at<Vec3b>(y,x);
			Vec3b intensity3 = imgs[3].at<Vec3b>(y,x);
			B += ((int)intensity0.val[0]+(int)intensity1.val[0]+(int)intensity2.val[0]+(int)intensity3.val[0])/4;
			G += ((int)intensity0.val[1]+(int)intensity1.val[1]+(int)intensity2.val[1]+(int)intensity3.val[1])/4;
			R += ((int)intensity0.val[2]+(int)intensity1.val[2]+(int)intensity2.val[2]+(int)intensity3.val[1])/4;
			s.at<uchar>(y,x).val[0] = B;
			s.at<uchar>(y,x).val[1] = G;
			s.at<uchar>(y,x).val[2] = R;
		}
		
	}
	return s;
}

    int process(VideoCapture& capture) {
        int n = 0,m=0;
        char filename[200];
        string window_name = "video | q or esc to quit";
        cout << "press space to save a picture. q or esc to quit" << endl;
        namedWindow(window_name, WINDOW_KEEPRATIO); //resizable window;
        Mat frame;
	
        for (;;) {
            capture >> frame;
            if (frame.empty())
                break;

	    imgs[m] = frame;
	    if(isRunning==true && m == 3){
		img_rgb = moyImgs();//frame;
		drawStuff(0,0);	
	    }

            imshow(window_name, frame);
	    
	    char key = (char)waitKey(30); //delay N millis, usually long enough to display and capture input
		    switch (key) {
			    case 'q':
			    case 'Q':
			    case 27: //escape key
				return 0;
			    case ' ': //Save an image
				isRunning = (isRunning==true) ? false : true;
				//sprintf(filename,"filename%.3d.jpg",n++);
				//imwrite(filename,frame);
				//cout << "Saved " << filename << endl;
				cout<<"PAUSED"<<endl;
				break;
			    default:
				break;
		    }
		m = (++m)%4;
        }
        return 0;
    }
}

int main(int ac, char** av) {
	cv::CommandLineParser parser(ac, av, "{help h||}{@input||}");
	if (parser.has("help"))
	{
	help(av);
	return 0;
	}
	std::string arg = parser.get<std::string>("@input");
	if (arg.empty()) {
	help(av);
	return 1;
	}
	VideoCapture capture(arg); //try to open string, this will attempt to open it as a video file or image sequence
	if (!capture.isOpened()) //if this fails, try to open as a video camera, through the use of an integer param
	capture.open(atoi(arg.c_str()));
	if (!capture.isOpened()) {
	cerr << "Failed to open the video device, video file or image sequence!\n" << endl;
	help(av);
	return 1;
	}
	ini();
	return process(capture);
}
