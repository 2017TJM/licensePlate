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
	//����������
	vector<int> HorizonJumpCheck(Mat,vector<Rect>);
	//�����Ƕ�
	vector<Mat> TurnPosition(vector<Rect>);
	//ͼ�����
	Mat sourceFrame, shapeKernal;
	//ͼ���е�����
	vector<vector<Point> > contourAll;
	vector<Vec4i> hierarchy;
};

