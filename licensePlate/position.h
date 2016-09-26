#pragma once

#include <iostream>
#include <core/core.hpp>
#include <highgui/highgui.hpp>
#include <imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

//��ȡ����
class position
{
public:
	position(void);
	~position(void);

	vector<Mat> getPlates(Mat);


private:
	//��λ����
	vector<Rect> LocatePosition();
	//�����Ƕ�
	vector<Mat> TurnPosition(vector<Rect>);

	Mat sourceFrame;
};

