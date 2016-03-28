#include <opencv2/opencv.hpp>  //头文件
#include <iostream> 
#include <fstream>
using namespace cv;  //包含cv命名空间
using namespace std;  


#define RESAMPLE_LEN 4 //每隔4个像素采一个数据

int main()
{
	cout << "To be begin." << endl;

	ofstream trainData("TrainData.txt", ios::out | ios::binary);
	ofstream trainLabel("TrainLabel.txt", ios::out | ios::binary);
	for (int i = 0; i < 10; i++)
	{
		vector<string> files;//看至此
		char a[50];
		sprintf(a,"TrainData/%d/%d_Arial.bmp", i, i);
		files.push_back(a);
		sprintf(a,"TrainData/%d/%d_Courier.bmp", i, i);
		files.push_back(a);
		sprintf(a,"TrainData/%d/%d_SongTi.bmp", i, i);
		files.push_back(a);
		sprintf(a,"TrainData/%d/%d_Times.bmp", i, i);
		files.push_back(a);
		sprintf(a,"TrainData/%d/%d_Verdana.bmp", i, i);
		files.push_back(a);


		for (int j = 0; j < files.size(); j++)
		{
//			cout << files[j].c_str() << endl;
			Mat img = imread(files[j].c_str(), 0);//读入灰度图
//			Mat f5 = features(img, 5);	//看到此

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
					trainData << nGray << "\t";
				}   
			}  
			trainData << "\n";

/*
			for (int k = 0; k < 10; k++)
			{
				if (k == i)
				{
					trainLabel << 9 << "\t";
				}
				else
				{
					trainLabel << 1 << "\t";
				}
			}
*/
			trainLabel << i << "\n";
			
		}
	}
	trainData.close();
	trainLabel.close();
	return 0;
}