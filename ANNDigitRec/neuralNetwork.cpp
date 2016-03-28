// Example : neural network learning
// usage: prog training_data_file testing_data_file

// For use with test / training datasets : handwritten_ex

// Author : Toby Breckon, toby.breckon@cranfield.ac.uk

// Copyright (c) 2010 School of Engineering, Cranfield University
// License : LGPL - http://www.gnu.org/licenses/lgpl.html

#include <opencv2/opencv.hpp>  //头文件
#include <vector>
#include <iostream> 
#include <fstream>
#include <string>
#include <algorithm>
using namespace cv;  //包含cv命名空间
using namespace std;  

int number_of_training_samples = 50;
int attributes_per_sample = 128;
int number_of_testing_samples = 39;
int number_of_classes = 10;
// N.B. classes are integer handwritten digits in range 0-9

/******************************************************************************/

// loads the sample database from file (which is a txt text file)
int read_data_from_txt(const char* filename, Mat& data, int rows, int cols )
{
	// if we can't read the input file then return 0
	fstream f(filename, ios::in | ios::out | ios::binary);
	if( !f )
	{
		printf("ERROR: cannot read file %s\n",  filename);
		return 0; // all not OK
	}
	// for each sample in the file
	for(int line = 0; line < rows; line++)
	{
		// for each attribute on the line in the file
		for(int attribute = 0; attribute < cols; attribute++)
		{
			string str = "";
			f >> str;
			int n;
			n = atoi(str.c_str());
			data.at<float>(line, attribute) = (float)n/255;
		}
	}
	f.close();
	return 1; // all OK
}

// loads the sample database from file (which is a txt text file)
int read_train_label_from_txt(const char* filename, Mat& label, int rows, int cols )
{
	// if we can't read the input file then return 0
	fstream f(filename, ios::in | ios::out | ios::binary);
	if( !f )
	{
		printf("ERROR: cannot read file %s\n",  filename);
		return 0; // all not OK
	}
	// for each sample in the file
	for(int line = 0; line < rows; line++)
	{
		// for each attribute on the line in the file
		for(int attribute = 0; attribute < cols; attribute++)
		{
			string str = "";
			f >> str;
			int n;
			n = atoi(str.c_str());
			for (int k = 0; k < 10; k++)
			{
				if (k == n)
				{
					label.at<float>(line, k) = 0.9;
				}
				else
				{
					label.at<float>(line, k) = 0.1;
				}
			}
		}
	}
	f.close();
	return 1; // all OK
}

// loads the sample database from file (which is a txt text file)
int read_test_label_from_txt(const char* filename, Mat& label, int rows, int cols )
{
	// if we can't read the input file then return 0
	fstream f(filename, ios::in | ios::out | ios::binary);
	if( !f )
	{
		printf("ERROR: cannot read file %s\n",  filename);
		return 0; // all not OK
	}
	// for each sample in the file
	for(int line = 0; line < rows; line++)
	{
		// for each attribute on the line in the file
		for(int attribute = 0; attribute < cols; attribute++)
		{
			string str = "";
			f >> str;
			int n;
			n = atoi(str.c_str());
			label.at<float>(line, attribute) = n;
		}
	}
	f.close();
	return 1; // all OK
}

int results_process(Mat& original_result, Mat& final_result, Mat& confidence_interval)
{
	float perresult[10];
	for (unsigned int nrow = 0; nrow < original_result.rows; nrow++)
	{
		for (unsigned int ncol = 0; ncol < original_result.cols; ncol++)
		{
			perresult[ncol] = original_result.at<float>(nrow, ncol);
		}
//		sort(perresult.begin(), perresult.end());
		float tempmax = 0;
		int tempposition;
		for (int i = 0; i < 10; i++)
		{
			if (tempmax < perresult[i])
			{
				tempmax = perresult[i];
				tempposition = i;				
			}
		}
		final_result.at<float>(nrow, 0) = tempposition;
	}

	return 0;
};

/******************************************************************************/

int main( int argc, char** argv )
{


	// define training data storage matrices (one for attribute examples, one
	// for classifications)
	Mat training_data(number_of_training_samples, attributes_per_sample, CV_32FC1);//50 X 128
	Mat training_label(number_of_training_samples, number_of_classes, CV_32FC1);

	// define testing data storage matrices
	Mat testing_data(number_of_testing_samples, attributes_per_sample, CV_32FC1);
	Mat testing_label(number_of_testing_samples, 1, CV_32FC1);
	Mat testing_classifications(number_of_testing_samples, number_of_classes, CV_32FC1);
	Mat testing_result(number_of_testing_samples, 1, CV_32FC1);
	Mat testing_confidence_interval(number_of_testing_samples, 1, CV_32FC1);
	// define classification output vector
	Mat classificationResult(1, number_of_classes, CV_32FC1);

	// load training and testing data sets
	read_data_from_txt("TrainData.txt", training_data, number_of_training_samples, attributes_per_sample);


	/*
	fstream testfile("testfile.txt", ios::out | ios::binary);
	testfile << format(training_data, "python");
	testfile.close();
	*/

	if (read_data_from_txt("TrainData.txt", training_data, number_of_training_samples, attributes_per_sample) && read_train_label_from_txt("TrainLabel.txt", training_label, number_of_training_samples, 1) &&
		read_data_from_txt("testData.txt", testing_data, number_of_testing_samples, attributes_per_sample) && read_test_label_from_txt("testLabel.txt", testing_label, number_of_testing_samples, 1))
	{

		
		// define the parameters for the neural network (MLP)

		// set the network to be 3 layer 256->10->10
		// - one input node per attribute in a sample
		// - 10 hidden nodes
		// - one output node per class

		// note that the OpenCV neural network (MLP) implementation does not
		// support categorical variables explicitly.
		// So, instead of the output class label, we will use
		// a binary vector of {0,0 ... 1,0,0} components (one element by class)
		// for training and therefore, MLP will give us a vector of "probabilities"
		// at the prediction stage - the highest probability can be accepted
		// as the "winning" class label output by the network

		// create the network using a sigmoid function with alpha and beta
		// parameters 0.6 and 1 specified respectively (refer to manual)

		CvANN_MLP nnetwork;
		// set the training parameters
		CvANN_MLP_TrainParams params = CvANN_MLP_TrainParams(
			// terminate the training after either 1000
			// iterations or a very small change in the
			// network wieghts below the specified value
			cvTermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 1000, 0.000001),
			// use backpropogation for training
			CvANN_MLP_TrainParams::BACKPROP,
			// co-efficents for backpropogation training
			// (refer to manual)
			0.1,
			0.1);
		Mat layers=(Mat_<int>(1,5) << attributes_per_sample, 10, 10, 10, number_of_classes); 
		nnetwork.create(layers, CvANN_MLP::SIGMOID_SYM, 0.6, 1);
		// train the neural network (using training data)
		nnetwork.train(training_data, training_label, Mat(), Mat(), params);

		nnetwork.predict(testing_data, testing_classifications);

		results_process(testing_classifications, testing_result, testing_confidence_interval);

		fstream outtest("testfile.txt", ios::out | ios::binary);    
		for( unsigned int nrow = 0; nrow < testing_classifications.rows; nrow++)  
		{  
			for(unsigned int ncol = 0; ncol < testing_classifications.cols; ncol++)  
			{  
				outtest << testing_classifications.at<float>(nrow,ncol) << '\t';    
			}   
			outtest << endl ;  
		}  
		outtest.close(); 

		for (int i = 0; i < testing_result.rows; i++)
		{
			cout << testing_result.at<float>(i, 0) << endl;
		}
	}

	return 0;
}
/******************************************************************************/