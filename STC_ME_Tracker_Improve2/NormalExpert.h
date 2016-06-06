#pragma once

#include <opencv2/opencv.hpp>
#include <cmath>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "ExpertResult.h"
#include "Complex.h"
#include "SnapshootExpert.h"

using namespace std;
using namespace cv;

//普通专家类
class NormalExpert
{
public:
	friend class SnapshootExpert;

private:
	Size mCxtSize;	//上下文大小
	Size mFrameSize;	//帧大小

	Point mCenterPoint;	//跟踪对象的中心点
	//分别存储的是对应的局部空间上下文区域子图像的横纵坐标的数组
	int *mCxtX;
	int *mCxtY;

	Mat mCxtMat;	//上下文数据矩阵
	Mat mCxtPriProMat;	//经过处理后得到的上下文先验概率矩阵
	Mat mCxtPriProMatFFT;	//经过FFT变换后的上下文先验概率矩阵

	Mat mSCModelFFT;	//经过FFT变换的空间上下文模型矩阵
	Mat mSTCModelFFT;	//经过FFT变换的时空上下文模型矩阵
	
	ExpertResult mExpertResult;

	Mat mPredictionConfidenceMap;	//用于存储当前帧的预测用的置信图

public:
	NormalExpert(const Size& mTCxtSize, const Size& mTFrameSize);
	NormalExpert(const NormalExpert& r);
	~NormalExpert();

public:
	NormalExpert& operator=(const NormalExpert& r);

public:
	void setCenterPoint(const Point& mTPoint);

	//根据提供的当前帧数据矩阵和滤波窗口矩阵计算当前帧的上下文数据矩阵
	void getContext(const Mat& mFrame, const Mat& mFilterWindowMat);

	//设置经过FFT变换的时空上下文模型
	void setSTCModel(const Mat& mTSTCModel);

	void updateSTCModel(double mTLearnRate);

	//计算当前专家的经过FFT变换的空间上下文模型
	Mat& calculateSCModel(const Mat& mTCxtPostProMatFFT);

	ExpertResult& calculateExpertResult(const Mat& mFrame, const Mat& mFilterWindowMat);

	//使用专家组中得到的最好专家重置普通专家类。形参中专家计算得到的数据为最新最可靠的数据，可直接用来进行覆盖
	void resetBySnapshootExpert(SnapshootExpert& mTSnapExpert);

};
