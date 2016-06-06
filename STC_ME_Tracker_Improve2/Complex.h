#pragma once

#include <opencv2/opencv.hpp>

using namespace cv;

//复数乘法
void complexMultiply(const Mat& src1, const Mat& src2, Mat& dst);
//复数除法
void complexDivide(const Mat& src1, const Mat& src2, Mat& dst);