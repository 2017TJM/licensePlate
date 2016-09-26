#include "position.h"


position::position()
{
	shapeKernal = getStructuringElement(MORPH_RECT, Size(5, 5));
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
	 
	//�ҶȻ�ͼ��
	cv::cvtColor(sourceFrame, grayFrame, CV_RGB2GRAY);
	//��˹�˲�
	cv::GaussianBlur(grayFrame,grayFrame, Size(3,3),0);
	//�����Ե��⣨X�᷽�����΢�֣��������Ϊ��ֵͼ
	cv::Sobel(grayFrame,grayFrame,grayFrame.depth(),2,0);
	//��ֵ��
	cv::threshold(grayFrame,grayFrame,80,255,CV_THRESH_BINARY);
	//��̬ѧ����(2�����ͣ�2�θ�ʴ)
	cv::dilate(grayFrame, grayFrame,shapeKernal);
	cv::dilate(grayFrame, grayFrame,shapeKernal);
	cv::erode(grayFrame, grayFrame,shapeKernal);
	cv::erode(grayFrame, grayFrame,shapeKernal);
	imshow("aaa", grayFrame);
	moveWindow("aaa",0,0);
	//�ҵ�����
	cv::findContours(grayFrame,contourAll,hierarchy,CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	//��䳵��	
	Rect plateRect;
	int platItemCount = contourAll.size();
	for(int i=0;i<platItemCount;i++)
    {
		plateRect = boundingRect(contourAll[i]);
		if(plateRect.area() < 50 || plateRect.width < 5 || plateRect.height < 5) 
			continue; //���С��50�������С��5������Ϊ���Ǿ���
		if(plateRect.width < plateRect.height || plateRect.width > plateRect.height * 4)
			continue; //���ڳ��ƿ�߶�Ϊ440*140����3.14����СΪ220*140��1.57�������Ⱥ͸߶ȵı�ֵΪ 1 �� 5 ֮����Ϊ����Ϊ����	
		rectangle(grayFrame,Point(plateRect.x,plateRect.y), Point(plateRect.x + plateRect.width, plateRect.y + plateRect.height), Scalar(255));
		drawContours(grayFrame,contourAll,i,Scalar(255),CV_FILLED);  
    }
	//������ͨ==========================����С��10������Ϊ����ͬһ������

	imshow("ccc", grayFrame);
	moveWindow("ccc",1000,0);

	return platePosition;
}

//�����Ƕ�
vector<Mat> position::TurnPosition(vector<Rect>)
{
	vector<Mat> plates;

	return plates;
}