//--------------------------------------������˵����-------------------------------------------
//		����˵��������ANN�������ַ�ʶ��ϵͳ
//		����������
//		�����������ò���ϵͳ�� Windows 7 64bit
//		������������IDE�汾��Visual Studio 2010
//		������������OpenCV�汾��	2.4.8
//------------------------------------------------------------------------------------------------

#include <opencv2/opencv.hpp>  //ͷ�ļ�
using namespace cv;  //����cv�����ռ�
//#include <fstream>
//using namespace std;  


void main( )
{    
	//����ͼ��	
	Mat srcImage = imread("scanDigit.bmp");
	imshow("��ԭʼͼ��",srcImage);//��ʾ�����ͼƬ
	//�ҶȻ�
	Mat grayImage;
	cvtColor(srcImage, grayImage, CV_RGB2GRAY);//RGBͼ��ĻҶȻ�
	imshow("���Ҷ�ͼ��",grayImage);
//	imwrite("grayImage.jpg",grayImage);//����ͼƬ
	//��ֵ��
	Mat biImage;
	threshold(grayImage, biImage, 200, 255, CV_THRESH_BINARY_INV ); //�ԻҶ�ͼ���ж�ֵ������,ǰ����Ϊ��ɫ��findContours()Ѱ���������ź���
	imshow("����ֵ��ͼ��",biImage);
//	imwrite("biImage.jpg",biImage);
/*
	//�鿴ͼƬ����
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
	//ȥ������
	Mat contourImage(biImage.rows, biImage.cols, CV_8UC1, Scalar(0, 0, 0));	
	Mat noiseRemoveImage = biImage.clone();	
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours( biImage, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );
	for(int i = 0; i< contours.size(); i++ )
	{
		Scalar color( 255, 255, 255 );
		drawContours( contourImage, contours, i, color, 1, 8, hierarchy, 1);//��������
//		printf("����[%d]�����: %.2f\n", i, contourArea(contours[i]));
		if (contourArea(contours[i]) < 20)			//�˴����20Ϊ����ֵ
		{
			drawContours( noiseRemoveImage, contours, i, Scalar(0, 0, 0), -1, 8, hierarchy, 0);
		}
	}
	imshow( "������ͼ", contourImage );
	imshow( "ȥ������ͼ", noiseRemoveImage );
/*
	//������ɫ����
	Mat dstImage;
	bitwise_not(srcImage, dstImage);
	imshow( "��ɫͼ", dstImage );
*/
/*
	//������б�ȵ���
	Mat rotMat(2, 3, CV_32FC1);
	Mat rotateImage(noiseRemoveImage.rows, noiseRemoveImage.cols, noiseRemoveImage.type(), Scalar(0, 0, 0));	
	Point center = Point(noiseRemoveImage.cols/2, noiseRemoveImage.rows/2);
	double angle = 2.1;//�˴�ָ����ת�Ƕ�
	//	double angle = slopeImage(noiseImage);
	double scale = 1;
	rotMat = getRotationMatrix2D(center, angle, scale);
	warpAffine(noiseRemoveImage, rotateImage, rotMat, rotateImage.size(),INTER_LINEAR, BORDER_CONSTANT, Scalar(0, 0, 0));
	imshow( "��תͼ", rotateImage );
*/
/*
	//��ͼ��������Ͳ���
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));//�����
	Mat dilateImage = srcImage.clone();
	morphologyEx(srcImage, dilateImage, MORPH_DILATE, element);
	imshow( "����ͼ", dilateImage);
*/
	//�ַ��ָ���ߴ��һ������
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
		//��ÿ���ַ��ĳߴ��һ��������ΪͼƬ
		Mat roi = SegmentationImage(rect);
		resize(roi,characterImage,Size(32, 64 ),(0,0),(0,0),INTER_NEAREST);
		//�ٷ�ɫ����
		bitwise_not(characterImage, dstImage);
		sprintf(name, "TestData\\%d.jpg", i);
		imwrite(name,dstImage);	
	}
	imshow( "�ָ�ͼ", SegmentationImage);
	//�ȴ����ⰴ������
	waitKey(0);
}  

