#include "VideoBean.h"

VideoBean::VideoBean()
{
}

VideoBean::VideoBean(string mVideoPath, int x, int y, int width, int height, string name)
{
	this->mVideoPath = mVideoPath;
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->name = name;

	mConfig = NULL;
}

VideoBean::VideoBean(const VideoBean& r)
{
	mVideoPath = r.mVideoPath;
	x = r.x;
	y = r.y;
	width = r.width;
	height = r.height;
	name = r.name;
	mConfig = r.mConfig;
}


VideoBean::~VideoBean()
{
}

VideoBean& VideoBean::operator=(const VideoBean& r)
{
	if (this == &r)
	{
		return *this;
	}

	mVideoPath = r.mVideoPath;
	x = r.x;
	y = r.y;
	width = r.width;
	height = r.height;
	name = r.name;

	mConfig = r.mConfig;
	return *this;
}

void VideoBean::set(string mVideoPath, int x, int y, int width, int height, string name, Config* mTConfig)
{
	this->mVideoPath = mVideoPath;
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->name = name;
	this->mConfig = mTConfig;
}

Config& VideoBean::getConfig()
{
	return *mConfig;
}

Config* VideoBean::getConfigPtr()
{
	return mConfig;
}
