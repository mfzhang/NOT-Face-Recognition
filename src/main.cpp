/*
 * FaceRecog.cpp
 *
 *  Created on: Aug 6, 2014
 *      Author: ardillo
 */

#include <opencv2/opencv.hpp>
#include "color_constancy.hpp"
#include "functions.h"

using namespace cv;
using namespace std;

//#define DEBUG


int main( int argc, char** argv )
{

  //////////////////////////////////////////////
  // Lees image in
  Mat input;
  input = imread( argv[1], 1);

  if( argc != 2 || !input.data ){
	  cout << "No image data \n" << endl;
	  return 0;
  }

  //////////////////////////////////////////////
  // resized original
  Mat original = input.clone();
  //original.create(240,320, CV_8UC(3));
  //resize(input, original, original.size(), 0, 0, INTER_NEAREST);


  color_correction::contrast_stretching a;
  color_correction::gray_world b1;
  //color_correction::gray_edge b2;
  //color_correction::maxRGB b3;
  //color_correction::max_edge b4;

  /*
  imshow("original",original);
  imshow("contrast stretching",a.run(original));
  imshow("gray world RGB",b1.run2(original,1,2));
  imshow("gray world Lab",b1.run1(original,1));
  imshow("Shades of gray",b1.run2(original,6,2));
  imshow("maxRGB",b3.run(original,6,0));
  imshow("gray edge",b2.run(original,1,0));
  imshow("max edge",b4.run(original,1,0));
  */


  //////////////////////////////////////////////
  // 3
  // Gray world normalization ->
  // HSV filtering
  Mat image_HSV3;
  Mat GW_RGB3 = b1.run2(original,1,2);
  cvtColor(GW_RGB3, image_HSV3, COLOR_BGR2HSV);            // Convert GW_RGB3 to HSV color space
  Mat HSV_filter3 = filtered_HSV(image_HSV3);          // Filter HSV based on paper
  cvtColor(HSV_filter3, HSV_filter3, COLOR_HSV2BGR);   // Convert back to BGR in order to print on screen


  //////////////////////////////////////////////
  // display original
  namedWindow( "Original image", CV_WINDOW_NORMAL ); //CV_WINDOW_NORMAL -- WINDOW_AUTOSIZE
  imshow( "Original image", input );

  // 3
  namedWindow( "3: Image gray world norm", CV_WINDOW_NORMAL );
  imshow( "3: Image gray world norm", GW_RGB3 );

  namedWindow( "3: HSV filtered", CV_WINDOW_NORMAL );
  imshow( "3: HSV filtered", HSV_filter3 );

  waitKey(0);

  return 0;
}

/*
 * Some garbage testing
 */
/*

  //////////////////////////////////////////////
  // 0
  // HSV filtering
  Mat image_HSV;
  cvtColor(original, image_HSV, COLOR_BGR2HSV);
  Mat HSV_filter = filtered_HSV(image_HSV);
  cvtColor(HSV_filter, HSV_filter, COLOR_HSV2BGR);

  //////////////////////////////////////////////
  // 1
  // Gamma correctie ->
  // HSV filtering
  Mat image_HSV1;
  Mat gammaCorr1 = correctGamma(original, 2.2);
  cvtColor(gammaCorr1, image_HSV1, COLOR_BGR2HSV);
  Mat HSV_filter1 = filtered_HSV(image_HSV1);
  cvtColor(HSV_filter1, HSV_filter1, COLOR_HSV2BGR);

  //////////////////////////////////////////////
  // 2
  // Gamma correctie ->
  // Histogram equalization ->
  // HSV filtering
  Mat image_HSV2;
  Mat gammaCorr2 = correctGamma(original, 2.2);
  Mat eq2 = equalizeIntensity(gammaCorr2);
  cvtColor(eq2, image_HSV2, COLOR_BGR2HSV);
  Mat HSV_filter2 = filtered_HSV(image_HSV2);
  cvtColor(HSV_filter2, HSV_filter2, COLOR_HSV2BGR);

  //////////////////////////////////////////////
  // 4
  // Histogram stretch (auto contrast) ->
  // HSV filtering
  Mat image_HSV4;
  Mat contrStr = a.run(original);
  cvtColor(contrStr, image_HSV4, COLOR_BGR2HSV);
  Mat HSV_filter4 = filtered_HSV(image_HSV4);
  cvtColor(HSV_filter4, HSV_filter4, COLOR_HSV2BGR);

  // 0
  namedWindow( "0: HSV filtered", CV_WINDOW_NORMAL );
  imshow( "0: HSV filtered", HSV_filter );

  // 1
  namedWindow( "1: Image gam corr", CV_WINDOW_NORMAL );
  imshow( "1: Image gam corr", gammaCorr1 );

  namedWindow( "1: HSV filtered", CV_WINDOW_NORMAL );
  imshow( "1: HSV filtered", HSV_filter1 );

  // 2
  namedWindow( "2: Image hist eq", CV_WINDOW_NORMAL );
  imshow( "2: Image hist eq", eq2 );

  namedWindow( "2: HSV filtered", CV_WINDOW_NORMAL );
  imshow( "2: HSV filtered", HSV_filter2 );

  // 4
  namedWindow( "4: Image contrast stretched", CV_WINDOW_NORMAL );
  imshow( "4: Image contrast stretched", contrStr );

  namedWindow( "4: HSV filtered", CV_WINDOW_NORMAL );
  imshow( "4: HSV filtered", HSV_filter4 );

*/


