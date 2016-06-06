#include "ConfidenceList.h"

ConfidenceList::ConfidenceList(int intervalNumber) :intervalNumber(intervalNumber)
{
	capacity = this->intervalNumber + 1;
	startIndex = endIndex = 0;
	size = 0;
	dataPtr = new double[capacity];
}

ConfidenceList::ConfidenceList(const ConfidenceList& r)
{
	intervalNumber = r.intervalNumber;
	capacity = r.capacity;
	startIndex = r.startIndex;
	endIndex = r.endIndex;
	size = r.size;
	dataPtr = new double[capacity];
	for (int i = startIndex; i != endIndex;)
	{
		dataPtr[i] = r.dataPtr[i];
		i++;
		if (i == capacity)
		{
			i = 0;
		}
	}
}

ConfidenceList::~ConfidenceList()
{
	if (dataPtr != NULL)
	{
		delete[] dataPtr;
	}
}

ConfidenceList& ConfidenceList::operator=(const ConfidenceList& r)
{
	if (this == &r)
	{
		return *this;
	}
	if (capacity != r.capacity)
	{
		intervalNumber = r.intervalNumber;
		capacity = r.capacity;
		if (dataPtr != NULL)
		{
			delete[] dataPtr;
		}
		dataPtr = new double[capacity];
	}
	
	startIndex = r.startIndex;
	endIndex = r.endIndex;
	size = r.size;
	
	for (int i = startIndex; i != endIndex;)
	{
		dataPtr[i] = r.dataPtr[i];
		i++;
		if (i == capacity)
		{
			i = 0;
		}
	}
	return *this;
}

void ConfidenceList::addConfidence(double confi)
{
	if (size == capacity)
	{
		startIndex++;
		if (startIndex == capacity)
		{
			startIndex = 0;
		}
	}
	else
	{
		size++;
	}
	dataPtr[endIndex++] = confi;
	if (endIndex == capacity)
	{
		endIndex = 0;
	}
}


double ConfidenceList::calculateEstimateScale()
{
	double sum = 0;
	int frontIndex = startIndex;
	int backIndex = frontIndex + 1;
	for (int i = 0; i < intervalNumber; i++)
	{
		if (frontIndex == capacity)
		{
			frontIndex = 0;
		}
		if (backIndex == capacity)
		{
			backIndex = 0;
		}
		sum = sum + sqrt(dataPtr[backIndex] / dataPtr[frontIndex]);
		frontIndex++;
		backIndex++;
	}
	return sum / intervalNumber;
}
