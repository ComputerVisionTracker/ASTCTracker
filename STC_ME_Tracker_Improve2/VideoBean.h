#pragma once

#include "string"
#include "Config.h"

using namespace std;

class VideoBean
{
public:
	string mVideoPath;
	int x;
	int y;
	int width;
	int height;
	string name;
	Config *mConfig;


public:
	VideoBean();
	VideoBean(string mVideoPath, int x, int y, int width, int height, string name);
	VideoBean(const VideoBean& r);
	~VideoBean();

public:
	VideoBean& operator =(const VideoBean &r);

public:
	void set(string mVideoPath, int x, int y, int width, int height, string name, Config* mTConfig);

	Config& getConfig();

	Config* getConfigPtr();
};

