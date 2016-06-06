#pragma once

#include <stdlib.h>
#include <cmath>
#include <fstream>
#include <iostream>

using namespace std;

//置信度队列
//只保存足够数量的置信度用于计算尺度变化
class ConfidenceList
{

private:
	int intervalNumber;//间隔阈值
	int capacity;//容量=间隔+1
	int startIndex;//start指向起始的第一个数据的索引
	int endIndex;//end指向即将写入数据所在的索引
	int size;
	double *dataPtr;

public:
	ConfidenceList(int intervalNumber);
	ConfidenceList(const ConfidenceList& r);
	~ConfidenceList();

public:
	ConfidenceList& operator=(const ConfidenceList& r);

public:
	//添加一个新的置信度
	void addConfidence(double confi);
	//计算当次的估计尺度
	double calculateEstimateScale();

};