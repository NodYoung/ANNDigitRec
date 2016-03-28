#include <opencv2/opencv.hpp>  //头文件
#include <iostream> 
#include <fstream>
using namespace cv;  //包含cv命名空间
using namespace std;  


#define RESAMPLE_LEN 4 //每隔4个像素采一个数据

int main()
{
	cout << "To be begin." << endl;

	ofstream testData("testData.txt", ios::out | ios::binary);
	ofstream testLabel("testLabel.txt", ios::out | ios::binary);
	for (int i = 0; i < 39; i++)
	{
		vector<string> files;//看至此
		char filename[50];
		sprintf(filename,"TestData/%d.jpg", i);
		Mat img = imread(filename, 0);//读入灰度图
		for( unsigned int nrow = 0; nrow < img.rows; nrow += RESAMPLE_LEN)  
		{  
			for(unsigned int ncol = 0; ncol < img.cols; ncol += RESAMPLE_LEN)  
			{  
				int nGray = 0;
				for (int m = nrow; m < nrow+RESAMPLE_LEN; m++)
				{
					for (int n = ncol; n < ncol+RESAMPLE_LEN; n++)
					{
						nGray += img.at<unsigned char>(m,n); 
					}	
				}
				nGray /= RESAMPLE_LEN*RESAMPLE_LEN;
				//nGray /= 255;//变为0至1区间
				testData << nGray << "\t";
			}   
		}  
		testData << "\n";
	
	}
	testData.close();
	testLabel.close();
	return 0;
}