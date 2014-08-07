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
// filter HSV based on research
/*
 * H >  0    and H < 63,75
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
			  if ( (H >  0 && H <  64) && (S > 38 && S < 230) && (V > 51 && V < 242) )
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
// calculate percentage
float calcPercentage( const Mat& inputImage)
{
	int count = 0, pixels;

	pixels = inputImage.cols * inputImage.rows;

	for(int i=0; i<inputImage.rows; i++){
		for(int j=0; j<inputImage.cols; j++){
			Vec3b pixel = inputImage.at<Vec3b>(i,j);

		}
	}
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
