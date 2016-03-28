//--------------------------------------【程序说明】-------------------------------------------
//		程序说明：基于ANN的数字字符识别系统
//		程序描述：
//		开发测试所用操作系统： Windows 7 64bit
//		开发测试所用IDE版本：Visual Studio 2010
//		开发测试所用OpenCV版本：	2.4.8
//------------------------------------------------------------------------------------------------

#include <opencv2/opencv.hpp>  //头文件
using namespace cv;  //包含cv命名空间
//#include <fstream>
//using namespace std;  


void main( )
{    
	//载入图像	
	Mat srcImage = imread("scanDigit.bmp");
	imshow("【原始图】",srcImage);//显示载入的图片
	//灰度化
	Mat grayImage;
	cvtColor(srcImage, grayImage, CV_RGB2GRAY);//RGB图像的灰度化
	imshow("【灰度图】",grayImage);
//	imwrite("grayImage.jpg",grayImage);//保存图片
	//二值化
	Mat biImage;
	threshold(grayImage, biImage, 200, 255, CV_THRESH_BINARY_INV ); //对灰度图进行二值化处理,前景变为白色后findContours()寻找外轮廓才好用
	imshow("【二值化图】",biImage);
//	imwrite("biImage.jpg",biImage);
/*
	//查看图片数据
	ofstream outImage("imagedata.txt", ios::out | ios::binary);    
	for( unsigned int nrow = 0; nrow < srcImage.rows; nrow++)  
	{  
		for(unsigned int ncol = 0; ncol < srcImage.cols; ncol++)  
		{  
			uchar val = srcImage.at<unsigned char>(nrow,ncol);    
			outImage << (int(val) > 200 ? 1 :0) ; //File3<<int(val)<< endl ;
		}   
		outImage << endl ;  
	}  
	outImage.close(); 
*/
	//去除噪声
	Mat contourImage(biImage.rows, biImage.cols, CV_8UC1, Scalar(0, 0, 0));	
	Mat noiseRemoveImage = biImage.clone();	
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours( biImage, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );
	for(int i = 0; i< contours.size(); i++ )
	{
		Scalar color( 255, 255, 255 );
		drawContours( contourImage, contours, i, color, 1, 8, hierarchy, 1);//绘制轮廓
//		printf("轮廓[%d]的面积: %.2f\n", i, contourArea(contours[i]));
		if (contourArea(contours[i]) < 20)			//此处面积20为经验值
		{
			drawContours( noiseRemoveImage, contours, i, Scalar(0, 0, 0), -1, 8, hierarchy, 0);
		}
	}
	imshow( "外轮廓图", contourImage );
	imshow( "去除噪声图", noiseRemoveImage );
/*
	//先做反色处理
	Mat dstImage;
	bitwise_not(srcImage, dstImage);
	imshow( "反色图", dstImage );
*/
/*
	//整体倾斜度调整
	Mat rotMat(2, 3, CV_32FC1);
	Mat rotateImage(noiseRemoveImage.rows, noiseRemoveImage.cols, noiseRemoveImage.type(), Scalar(0, 0, 0));	
	Point center = Point(noiseRemoveImage.cols/2, noiseRemoveImage.rows/2);
	double angle = 2.1;//此处指定旋转角度
	//	double angle = slopeImage(noiseImage);
	double scale = 1;
	rotMat = getRotationMatrix2D(center, angle, scale);
	warpAffine(noiseRemoveImage, rotateImage, rotMat, rotateImage.size(),INTER_LINEAR, BORDER_CONSTANT, Scalar(0, 0, 0));
	imshow( "旋转图", rotateImage );
*/
/*
	//对图像进行膨胀操作
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));//定义核
	Mat dilateImage = srcImage.clone();
	morphologyEx(srcImage, dilateImage, MORPH_DILATE, element);
	imshow( "膨胀图", dilateImage);
*/
	//字符分割并作尺寸归一化处理
	Mat SegmentationImage = noiseRemoveImage.clone();
	vector<vector<Point> > segcontours;
	vector<Vec4i> seghierarchy;
	findContours( noiseRemoveImage, segcontours, seghierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	Mat characterImage;
	Mat dstImage;
	char name[50];
	for (int i = 0; i < segcontours.size(); i++)
	{
		Rect rect = boundingRect(Mat(segcontours[i]));
//		rectangle(SegmentationImage, rect.tl(), rect.br(), Scalar(255, 0, 0));
		//将每个字符的尺寸归一化并保存为图片
		Mat roi = SegmentationImage(rect);
		resize(roi,characterImage,Size(32, 64 ),(0,0),(0,0),INTER_NEAREST);
		//再反色处理
		bitwise_not(characterImage, dstImage);
		sprintf(name, "TestData\\%d.jpg", i);
		imwrite(name,dstImage);	
	}
	imshow( "分割图", SegmentationImage);
	//等待任意按键按下
	waitKey(0);
}  

