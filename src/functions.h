/*
 * functions.h
 *
 *  Created on: Aug 6, 2014
 *      Author: ardillo
 */

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

//////////////////////////////////////////////
// int to float value 0...1
Mat matUintToFloat(const cv::Mat &input)
{
	// code below is necessary, converting does not influence pixel values
	Mat output = input.clone();
	output.convertTo(output, CV_32FC3);
	for ( int i = 0; i < (output.rows); i++) {
		for (int j = 0; j < (output.cols); j++) {
			output.at<Vec3b>(i,j)[0] = output.at<Vec3b>(i,j)[0] / 255;
			output.at<Vec3b>(i,j)[1] = output.at<Vec3b>(i,j)[1] / 255;
			output.at<Vec3b>(i,j)[2] = output.at<Vec3b>(i,j)[2] / 255;
	}	}
	return output;
}

//////////////////////////////////////////////
// normalize RGB, todo doesn't work yet
Mat normalized_RGB(const cv::Mat &input)
{
	Mat output = input.clone();
	output.convertTo(output, CV_32F);
	for(int i=0; i<input.rows; i++){
		  for(int j=0; j<input.cols; j++){
			  float B = input.at<Vec3b>(i,j)[0];
			  float G = input.at<Vec3b>(i,j)[1];
			  float R = input.at<Vec3b>(i,j)[2];
			  output.at<Vec3b>(i,j)[0] = round((B / (B+G+R)) * 255);
			  output.at<Vec3b>(i,j)[1] = round((G / (B+G+R)) * 255);
			  output.at<Vec3b>(i,j)[2] = round((R / (B+G+R)) * 255);
		  }
	}
	return output;
}

//////////////////////////////////////////////
/* filter HSV based on paper
 *
 * H > 0    and H < 0.25
 * S > 0.15 and S < 0.9
 * V > 0.2  and V < 0.95
 * Where H,S,V are in the range of 0...1
 *
/*/////
/*/////
 * OpenCV uses the ranges H=0...180 S=0...255 V=0.255 so this means the following settings
 * H >  0    and H < 45
 * S > 38,25 and S < 229,5
 * V > 51    and V < 242.25
 */
Mat filtered_HSV(const cv::Mat &input_HSV)
{
	Mat output = input_HSV.clone();
	for(int i=0; i<input_HSV.rows; i++){
		  for(int j=0; j<input_HSV.cols; j++){
			  Vec3b pixel = input_HSV.at<Vec3b>(i,j);
			  int H = pixel[0];
			  int S = pixel[1];
			  int V = pixel[2];
			  if ( (H >  0 && H <  25) && (S > 5 && S < 155) && (V > 55 && V < 255) )
			  {
				  pixel[0] = H;
				  pixel[1] = S;
				  pixel[2] = V;
			  }
			  else{
				  pixel[0] = 0 ;
				  pixel[1] = 0 ;
				  pixel[2] = 0 ;
			  }
			  output.at<Vec3b>(i,j) = pixel;
		  }
	}
	return output;
}
//////////////////////////////////////////////
/* filter RGB based on own research
 * Met GIMP heb ik de volgende waarde ontdekt
 * R > 150  and R < 240
 * G > 90  and G < 180
 * B > 70  and B < 120
 */
Mat filtered_BGR( const Mat& inputImage)
{
	Mat output = inputImage.clone();
	for(int i=0; i<inputImage.rows; i++){
		for(int j=0; j<inputImage.cols; j++){
			Vec3b pixel = inputImage.at<Vec3b>(i,j);
			int B = pixel[0]; int G = pixel[1]; int R = pixel[2];
			if ((B>60 && B<130) && (G>80 && G<190) && (R>140 && R<250))
				continue;
			else{
			  pixel[0] = 0; pixel[1] = 0; pixel[2] = 0;
			}
			output.at<Vec3b>(i,j) = pixel;
		}
	}
	return output;
}

//////////////////////////////////////////////
// calculate percentage
float calcPercentage( const Mat& inputImage, const int threshold)
{
	int pixels;
	float percentage, count = 0.0;

	pixels = inputImage.cols * inputImage.rows;

	for(int i=0; i<inputImage.rows; i++){
		for(int j=0; j<inputImage.cols; j++){
			Vec3b pixel = inputImage.at<Vec3b>(i,j);
			if ( (pixel[0] > threshold) && (pixel [1] > threshold) && (pixel [2] > threshold))
				//cout << "hit" << endl;
				count++;
		}
	}

	percentage = (count / pixels) * 100.0 ;
	return percentage;
}

//////////////////////////////////////////////
// thresholding BGR image (first calculated toGray)
Mat thresholding( const Mat& inputImage, const int value)
{
	Mat gray(inputImage.rows, inputImage.cols, CV_8UC1);
	cvtColor(inputImage, gray, CV_BGR2GRAY);
	for(int i=0; i<gray.rows; i++){
		for(int j=0; j<gray.cols; j++){
			if ( gray.at<uchar>(i,j) > value)
				gray.at<uchar>(i,j) = 255;
			else
				gray.at<uchar>(i,j) = 0;
		}
	}
	return gray;
}

//////////////////////////////////////////////
// XOR image with binary mask
Mat XOR(const Mat& inputImage, const Mat& mask)
{
	if(inputImage.size() != mask.size() )
		return Mat();

	Mat output = Mat::zeros(inputImage.size(), CV_8UC3);
	for(int i=0; i<inputImage.rows; i++){
		for(int j=0; j<inputImage.cols; j++){
			if(mask.at<uchar>(i,j) == 255)
			{
				Vec3b pixel = inputImage.at<Vec3b>(i,j);
				//pixel[0] = 0;
				//pixel[1] = 0;
				//pixel[2] = 0;
				output.at<Vec3b>(i,j) = pixel;
			}
		}
	}
	return output;
}

//////////////////////////////////////////////
// Gamma Correction
Mat correctGamma( Mat& inputImage, double gamma ) {
 double inverse_gamma = 1.0 / gamma;

 Mat lut_matrix(1, 256, CV_8UC1 );
 uchar * ptr = lut_matrix.ptr();
 for( int i = 0; i < 256; i++ )
   ptr[i] = (int)( pow( (double) i / 255.0, inverse_gamma ) * 255.0 );

 Mat output;
 LUT( inputImage, lut_matrix, output );

 return output;
}

//////////////////////////////////////////////
// Histogram equalization
Mat equalizeIntensity(const Mat& inputImage)
{
    if(inputImage.channels() >= 3)
    {
        Mat ycrcb;
        cvtColor(inputImage,ycrcb,CV_BGR2YCrCb);

        vector<Mat> channels;
        split(ycrcb,channels);

        equalizeHist(channels[0], channels[0]);

        Mat result;
        merge(channels,ycrcb);
        cvtColor(ycrcb,result,CV_YCrCb2BGR);

        return result;
    }
    return Mat();
}

#endif /* FUNCTIONS_H_ */
