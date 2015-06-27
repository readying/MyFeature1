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

/*
openCV_LBPHistogram
Various helper functions for computing the LBP representation and histogram of an image.
Author: Florian Gross

*/

#ifndef OPENCV_LBPHISTOGRAM_HPP_
#define OPENCV_LBPHISTOGRAM_HPP_

#ifdef HAVE_OPENCV

#include <core/smileCommon.hpp>
#include <core/dataSource.hpp>

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>

namespace LBPHistogram {

/**
 * @function euclideanDistance
 * @brief Computes the euclidean distance of two points ( \f$ d = \sqrt{(x_2-x_1)^2+(y_2-y_1)^2}\f$ ).
 */
inline float euclideanDistance(cv::Point& p1, cv::Point& p2)
{
  float dx = p1.x - p2.x;
  float dy = p1.y - p2.y;
  return std::sqrt(dx * dx + dy * dy);
};

/**
 * @function computeLBPImage_
 * @brief Creates an image representation of the LBP value for each pixel. (by binarizing...)
 * @param src Source Image (@link cv::Mat @endlink)
 */
template<class Elem, class Word>
inline cv::Mat computeLBPImage_(cv::Mat& src)
{
	cv::Mat dst = cv::Mat::zeros(src.size(), CV_8UC1);
	for (int i = 1; i < src.rows - 1; ++i)
    {
        for (int j = 1; j < src.cols - 1; ++j)
        {
            std::vector<Elem> neighbors(8);
            neighbors[0] = src.at<Elem>(i,     j + 1);
            neighbors[1] = src.at<Elem>(i + 1, j + 1);
            neighbors[2] = src.at<Elem>(i + 1, j);
            neighbors[3] = src.at<Elem>(i + 1, j - 1);
            neighbors[4] = src.at<Elem>(i,     j - 1);
            neighbors[5] = src.at<Elem>(i - 1, j - 1);
            neighbors[6] = src.at<Elem>(i - 1, j);
            neighbors[7] = src.at<Elem>(i - 1, j + 1);
            Elem c = src.at<Elem>(i, j);
            Word word = 0;
            for (size_t k = 0; k < neighbors.size(); ++k)
            {
                if (k > 0)
                    word = word << 1;
                unsigned char bit = neighbors[k] > c;
                word |= bit;
            }
            dst.at<Word>(i, j) = word;
        }
    }
    return dst;
};

/**
 * @function computeLBPImage
 * @brief Wrapper for computeLBPImage_ with standard types <unsigned char, unsigned char>
 */
inline cv::Mat computeLBPImage(cv::Mat& src)
{
	return computeLBPImage_<unsigned char, unsigned char>(src);
};

/**
 * @function lbp_hist
 * @brief Creates an LBP Histogram.
 * @param src LBP image representation (see computeLBPImage)
 * @param dst Destination histogram
 * @param wordMap Mapping to uniform or identity patterns
 */
template<class Word, class Cnt>
inline void lbp_hist_(cv::Mat& src, std::map<Word, Cnt>& dst, const std::map<Word, Word> &wordMap)
{
    dst.clear();
    for (typename std::map<Word, Word>::const_iterator itr = wordMap.begin();
         itr != wordMap.end(); ++itr)
    {
        dst[itr->second] = 0;
    }
    for (int i = 1; i < src.rows - 1; ++i)
    {
        for (int j = 1; j < src.cols - 1; ++j)
        {
            Word word = src.at<Word>(i, j);
            typename std::map<Word, Word>::const_iterator itr = wordMap.find(word);
            if (itr != wordMap.end())
            {
                ++dst[itr->second];
            }
            else
            {
                ++dst[word];
            }
        }
    }
};

/**
 * @function lbp_hist_add
 * @brief Add values of histogram bins of op1 in op2 to op1
 * @param op1 First histogram
 * @param op2 Second histogram
 */
// add values of histogram bins of op1 in op2 to op1
template<class Word, class Cnt>
inline void lbp_hist_add_(std::map<Word, Cnt>& op1, const std::map<Word, Cnt>& op2)
{
    for (typename std::map<Word, Cnt>::const_iterator itr = op2.begin();
         itr != op2.end(); ++itr)
    {
        op1[itr->first] += itr->second;
    }
};

/**
 * @function lbp_hist_normalize
 * @brief Normalizes the LBP Histogram.
 * @param op LBP histogram
 */
template<class Word, class Cnt>
inline void lbp_hist_normalize_(std::map<Word, Cnt>& op)
{
    Cnt total = 0;
    for (typename std::map<Word, Cnt>::const_iterator itr = op.begin();
         itr != op.end(); ++itr)
    {
        total += itr->second;
    }
    for (typename std::map<Word, Cnt>::iterator itr = op.begin();
         itr != op.end(); ++itr)
    {
        itr->second /= total;
    }
};

/**
 * @function computeLBPHistogram_
 * @brief Template function for complete computation of LBP histograms, either with uniform patterns mapping or without
 * @param src LBP image representation (see computeLBPImage)
 * @param wordMap Mapping to uniform or identity patterns
 */
template<class Word, class Cnt>
inline std::map<Word, Cnt> computeLBPHistogram_(cv::Mat& src, std::map<unsigned char, unsigned char>& wordMap, int normalize)
{
	std::map<Word, Cnt> lbpHist;
	
	lbp_hist_<Word, Cnt>(src, lbpHist, wordMap); // Create the histogram
	
	if(normalize == 1)
	{
		lbp_hist_normalize_<Word, Cnt>(lbpHist); // Normalize the histogram
	}
	
	return lbpHist;
};

/**
 * @function computeLBPHistogram
 * @brief Wrapper for computeLBPHistogram_ with standard types <unsigned char,float>
 */
inline std::map<unsigned char, float> computeLBPHistogram(cv::Mat& src, std::map<unsigned char, unsigned char>& wordMap, int normalize)
{
	return computeLBPHistogram_<unsigned char, float>(src, wordMap, normalize);
};

/**
 * @function compute_uniform_map_
 * @brief ???
 * @return Map with uniform pattern words
 */
template<class Word>
inline std::map<Word, Word> compute_uniform_map_()
{
	std::map<Word, Word> m;
    m.clear();
    int maxWord = (1 << (sizeof(Word) * 8)) - 1;
    int nextIdx = 0;
    std::vector<Word> nonunif;
    for (int word = 0; word <= maxWord; ++word)
    {
        int wordTmp = word;
        int ntrans = 0;
        unsigned char lastBit;
        for (int bit = 0; bit < sizeof(Word) * 8; ++bit)
        {
            // get LSB
            unsigned char bitValue = wordTmp & 1;
            // shift right
            wordTmp = wordTmp >> 1;
            if (bit > 0 && bitValue != lastBit)
            {
                ++ntrans;
            }
            lastBit = bitValue;
        }
        if (ntrans > 2)
        {
            nonunif.push_back(word);
        }
        else if (m.find(word) == m.end())
        {
            m[word] = nextIdx;
            ++nextIdx;
        }
    }
    for (size_t k = 0; k < nonunif.size(); ++k)
    {
        m[nonunif[k]] = nextIdx;
    }
    return m;
};

/**
 * @function compute_identity_map_
 * @brief ???
 * @return Map with identity pattern words
 */
template<class Word>
inline std::map<Word, Word> compute_identity_map_()
{
	std::map<Word, Word> m;
    m.clear();
    int maxWord = (1 << (sizeof(Word) * 8)) - 1;
    for (int word = 0; word <= maxWord; ++word)
    {
        m[word] = word;
    }
    return m;
};

/**
 * @function cropFace
 * @brief Scales and rotates image, so that a image of the upright head of the size dstSize is returned.
 */
inline void cropFace(const cv::Mat& src, cv::Mat& dst, cv::Point& leftEye, cv::Point& rightEye, float offsetXPct, float offsetYPct, cv::Size& dstSize)
{
  float offsetX = std::floor(offsetXPct * dstSize.width);
  float offsetY = std::floor(offsetYPct * dstSize.height);
  float rotation = std::atan2(float(rightEye.y - leftEye.y), float(rightEye.x - leftEye.x));
  float rotationDeg = rotation * 180.0 / M_PI;
  float dist = euclideanDistance(leftEye, rightEye);
  float refDist = float(dstSize.width) - 2.0 * offsetX;
  float scale = dist / refDist;
  cv::Mat rotMat = cv::getRotationMatrix2D(leftEye, rotationDeg, 1.0);
  cv::Mat dst2 = cv::Mat::zeros(src.rows, src.cols, src.type());
  cv::warpAffine(src, dst2, rotMat, dst2.size());
  float cropX = leftEye.x - scale * offsetX;
  float cropY = leftEye.y - scale * offsetY;
  float cropSizeWidth = float(dstSize.width) * scale;
  float cropSizeHeight = float(dstSize.height) * scale;
  
  cv::Rect roi(cropX, cropY, cropSizeWidth, cropSizeHeight);
  if (roi.x + roi.width >= dst2.cols)
    roi.width = dst2.cols - roi.x - 1;
  if (roi.y + roi.height >= dst2.rows)
    roi.height = dst2.rows - roi.y - 1;
  if (roi.x < 0)
    roi.x = 0;
  if (roi.y < 0)
    roi.y = 0;

  cv::Rect insideROI = roi & cv::Rect(0, 0, dst2.cols, dst2.rows);
  if(insideROI.area() > 0)
  {
  	cv::Mat cropped = dst2(insideROI);
  	cv::resize(cropped, dst, dstSize);
  }
  else
  {
  	cv::resize(dst2, dst, dstSize);
  }
};

template<class Key, class Value>
inline void print_map(const std::map<Key, Value>& m, std::ostream& os)
{
    os << "[" << std::endl;
    for (typename std::map<Key, Value>::const_iterator itr = m.begin();
         itr != m.end(); ++itr)
    {
        os << itr->first - 0 << " => " << itr->second - 0 << "," << std::endl;
    }
    os << "]" << std::endl;
};


template<class Key, class Value>
inline void print_map_values(const std::map<Key, Value>& m, std::ostream& os, char sep, char eol)
{
    for (typename std::map<Key, Value>::const_iterator itr = m.begin();
         itr != m.end(); )
    {
        os << itr->second - 0;
        if (++itr == m.end())
            break;
        os << sep;
    }
    os << eol;
};

} // End namespace LBPHistogram

#endif // HAVE_OPENCV

#endif /* OPENCV_LBPHISTOGRAM_HPP_ */
