#include "position.h"


position::position()
{
}


position::~position(void)
{
}


vector<Mat> position::getPlates(Mat videoFrame){	
	position::sourceFrame = videoFrame;

	vector<Rect> locPosition = LocatePosition();
	return TurnPosition(locPosition);
}

//��λ����
vector<Rect> position::LocatePosition()
{
	vector<Rect> platePosition;


	return platePosition;
}

//�����Ƕ�
vector<Mat> position::TurnPosition(vector<Rect>)
{
	vector<Mat> plates;

	return plates;
}