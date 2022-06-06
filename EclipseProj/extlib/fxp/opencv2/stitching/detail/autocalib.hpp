/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                          License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009, Willow Garage Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#ifndef __OPENCV_STITCHING_AUTOCALIB_HPP__
#define __OPENCV_STITCHING_AUTOCALIB_HPP__

#include "../../../../fxp/opencv2/core/core.hpp"
#include "../../../../fxp/opencv2/stitching/detail/matchers.hpp"

namespace cv {
namespace detail {

// See "Construction of Panoramic Image Mosaics with Global and Local Alignment"
// by Heung-Yeung Shum and Richard Szeliski.
void CV_EXPORTS focalsFromHomography(const Mat &H, double &f0, double &f1, bool &f0_ok, bool &f1_ok);

void CV_EXPORTS estimateFocal(const std::vector<ImageFeatures> &features,
                              const std::vector<MatchesInfo> &pairwise_matches,
                              std::vector<double> &focals);

bool CV_EXPORTS calibrateRotatingCamera(const std::vector<Mat> &Hs, Mat &K);

} // namespace detail
} // namespace cv

#endif // __OPENCV_STITCHING_AUTOCALIB_HPP__
