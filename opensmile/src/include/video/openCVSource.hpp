/*F***************************************************************************
 * openSMILE - the Munich open source Multimedia Interpretation by Large-scale
 * Extraction toolkit
 * 
 * (c) 2008-2013, Florian Eyben, Felix Weninger, Florian Gross, Martin
 * Woellmer, Bjoern Schuller
 * 
 * Technische Universität München (TUM), 80333 Munich, Germany
 * 
 * openSMILE is distributed under a research only license. 
 * 
 * You may use, copy, reproduce, and distribute this Software for any
 * non-commercial purpose. Some purposes which can be non-commercial are
 * teaching, academic research, public demonstrations and personal
 * experimentation. You may not use or distribute this Software or any
 * derivative works in any form for commercial purposes. 
 * 
 * See LICENSE.txt in the top level source directory for details.
 * 
 * THIS SOFTWARE COMES "AS IS", WITH NO WARRANTIES. THIS MEANS NO EXPRESS,
 * IMPLIED OR STATUTORY WARRANTY, INCLUDING WITHOUT LIMITATION, WARRANTIES OF
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, ANY WARRANTY AGAINST
 * INTERFERENCE WITH YOUR ENJOYMENT OF THE SOFTWARE OR ANY WARRANTY OF TITLE
 * OR NON-INFRINGEMENT. THERE IS NO WARRANTY THAT THIS SOFTWARE WILL FULFILL
 * ANY OF YOUR PARTICULAR PURPOSES OR NEEDS. ALSO, YOU MUST PASS THIS
 * DISCLAIMER ON WHENEVER YOU DISTRIBUTE THE SOFTWARE OR DERIVATIVE WORKS.
 * NEITHER TUM NOR ANY CONTRIBUTOR TO THE SOFTWARE WILL BE LIABLE FOR ANY
 * DAMAGES RELATED TO THE SOFTWARE OR THIS LICENSE AGREEMENT, INCLUDING
 * DIRECT, INDIRECT, SPECIAL, CONSEQUENTIAL OR INCIDENTAL DAMAGES, TO THE
 * MAXIMUM EXTENT THE LAW PERMITS, NO MATTER WHAT LEGAL THEORY IT IS BASED ON.
 * ALSO, YOU MUST PASS THIS LIMITATION OF LIABILITY ON WHENEVER YOU DISTRIBUTE
 * THE SOFTWARE OR DERIVATIVE WORKS.
 ***************************************************************************E*/


/*  openSMILE component:

OpenCVSource
Captures frames from either webcam or file stream and extracts a square region containing the face(s).
Author: Florian Gross

*/


#ifndef __OPENCV_SOURCE_HPP
#define __OPENCV_SOURCE_HPP


#include <core/smileCommon.hpp>
#include <core/dataSource.hpp>

#ifdef HAVE_OPENCV

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/video.hpp"

#include <video/openCV_LBPHistogram.hpp>
#include <video/openCV_HSVHistogram.hpp>
#include <video/openCV_OpticalFlow.hpp>

#include <iostream>
#include <stdio.h>

using namespace cv;

class ExtractedFace {
public:
	bool faceFound, eyesFound;
	cv::Rect faceRect;
	cv::Point leftEye, rightEye;
	cv::Mat faceGray, faceColor;
};

#define COMPONENT_DESCRIPTION_COPENCVSOURCE "Captures frames from either webcam or file stream, extracts a square region containing the face and creates a LBP, HSV and optical flow histogram."
#define COMPONENT_NAME_COPENCVSOURCE "cOpenCVSource"

#undef class
class DLLEXPORT cOpenCVSource : public cDataSource {
  private:
    int cfgDisplay; // Display video output
	String cfgVideoSource; // WEBCAM or FILE
	String cfgFilename;
	String cfgFace_cascade_name;
	String cfgEyes_cascade_name;

	int cfgExtractFace;
	int cfgExtractHSVHist;
	int cfgExtractLBPHist;
	int cfgExtractOpticalFlow;
	
	int cfgIncludeFaceFeatures;
	
	int cfgIgnoreInvalid;
	int cfgFaceWidth;
	int cfgLBPUniformPatterns;
	
	int cfgHueBins, cfgSatBins, cfgValBins;
	int cfgFlowBins;
	int cfgNormalizeHistograms;
	FLOAT_DMEM cfgMaxFlow;
	FLOAT_DMEM cfgFlowDownsample;

	double cfgFps;

	cv::Mat mCurrentFrame_bgr, mCurrentFrame_gray, mPrevFrame_gray_resized, mCurrentFrameDisplay_bgr;
	VideoCapture mVideoCapture;
	CascadeClassifier mFace_cascade;
	CascadeClassifier mEyes_cascade;

	int mVectorSize;

	std::map<unsigned char, unsigned char> mIdentityMap;
	std::map<unsigned char, unsigned char> mUniformMap;
	int mLBPSize;

	double avgVal[7], avgNum[7];
    
  protected:
    SMILECOMPONENT_STATIC_DECL_PR
    
    virtual void fetchConfig();
    virtual int myConfigureInstance();
    virtual int myFinaliseInstance();
    virtual int myTick(long long t);


    virtual int configureWriter(sDmLevelConfig &c);
    virtual int setupNewNames(long nEl);
    
	virtual ExtractedFace extractFace(cv::Mat& frame, cv::Mat& frame_gray);

  public:
    SMILECOMPONENT_STATIC_DECL
    
    cOpenCVSource(const char *_name);

    virtual ~cOpenCVSource();
};


#endif // HAVE_OPENCV

#endif // __LBPHIST_SOURCE_HPP
