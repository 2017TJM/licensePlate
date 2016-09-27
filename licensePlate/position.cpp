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
	Mat grayFrame, edgeFrame;
	//�ҶȻ�ͼ��
	cv::cvtColor(sourceFrame, grayFrame, CV_RGB2GRAY);
	//��˹�˲�
	cv::GaussianBlur(grayFrame,grayFrame, Size(3,3),0);	
	//�����Ե��⣨X�᷽�����΢�֣��������Ϊ��ֵͼ
	cv::Sobel(grayFrame,edgeFrame,grayFrame.depth(),2,0);
	////��ֵ��
	cv::threshold(edgeFrame,edgeFrame,80,255,CV_THRESH_BINARY);
	//��̬ѧ����(2�����ͣ�2�θ�ʴ)
	cv::dilate(edgeFrame, edgeFrame,shapeKernal);
	cv::dilate(edgeFrame, edgeFrame,shapeKernal);
	cv::erode(edgeFrame, edgeFrame,shapeKernal);
	cv::erode(edgeFrame, edgeFrame,shapeKernal);
	//ͨ������������Ϣ�������ҵ�Ǳ�ڳ���
	cv::findContours(edgeFrame,contourAll,hierarchy,CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	Rect plateRect;
	vector<Rect> edgeRect;
	int platItemCount = contourAll.size();
	for(int i=0;i<platItemCount;i++)
    {
		plateRect = boundingRect(contourAll[i]);
		if(plateRect.area() < 50 || plateRect.width < 5 || plateRect.height < 5) 
			continue; //���С��50�������С��5������Ϊ���Ǿ���
		if(plateRect.width < plateRect.height || plateRect.width > plateRect.height * 4)
			continue; //���ڳ��ƿ�߶�Ϊ440*140����3.14����СΪ220*140��1.57�������Ⱥ͸߶ȵı�ֵΪ 1 �� 5 ֮����Ϊ����Ϊ����			
		edgeRect.push_back(plateRect);
		//drawContours(edgeFrame,contourAll,i,Scalar(255),CV_FILLED);  
    }
	//������ͨ==========================����С��10������Ϊ����ͬһ������

	//��ɫ���� ============================���װ��֣��Ƶ׺��֣��ڵװ��֣��׵׺���

	//�����������
	vector<int> jumpRect = HorizonJumpCheck(grayFrame,edgeRect);
	for(int i=0;i<jumpRect.size();i++){
		platePosition.push_back(edgeRect[jumpRect[i]]);
		rectangle(sourceFrame,Point(platePosition[i].x,platePosition[i].y), Point(platePosition[i].x + platePosition[i].width, platePosition[i].y + platePosition[i].height), Scalar(0,0,255), 2);
	}

	imshow("ddd", sourceFrame);
	moveWindow("ddd",1000,0);

	return platePosition;
}


//����������
vector<int> position::HorizonJumpCheck(Mat grayFrame,vector<Rect> edgeRect){
		//�����������
	int platItemCount = edgeRect.size();
	Mat roiImg;
	uchar lastPixelValue, * tmpRow;
	vector<int> jumpRect, jumpRowCount;
	map<int,int> jumpCount;
	int itemCount, itemRowCount, roiWidth, roiHeight;
	bool thisBigFlag, changeFlag;
	for(int i=0;i<platItemCount;i++)
	{//ͳ�����о���
		jumpRowCount.clear();
		jumpCount.clear ();
		roiImg = grayFrame(edgeRect[i]);
		roiWidth = roiImg.cols;
		roiHeight = roiImg.rows;
		for(int k=0;k<roiHeight;k++)
		{//ÿ��ͳ��
			itemCount = 0;
			itemRowCount = 0;
			tmpRow = roiImg.ptr<uchar>(i);
			lastPixelValue = tmpRow[0];
			for(int j=1;j<roiWidth;j++)
			{//ͳ�Ƶ�ǰ�仯
				if(abs(lastPixelValue - tmpRow[j])<5){ //���ر仯С��5������Ϊ��û��״̬
					changeFlag = false;
				}else{//��С�仯
					changeFlag = true;
					itemRowCount ++;
				}
				lastPixelValue = tmpRow[j];
				if(changeFlag && itemRowCount > 3){//3�������ڵ�ͻ����Ϊ������
					changeFlag = false;
					itemRowCount = 0;
					itemCount++;
				}
			}
			jumpRowCount.push_back(itemCount);
		}
		for(int j=0;j<jumpRowCount.size();j++)
		{//ͳ��ÿ���������
			if(jumpCount.find(jumpRowCount[j]) == jumpCount.end())
				jumpCount.insert(pair<int,int>(jumpRowCount[j],1));
			else jumpCount[jumpRowCount[j]]++;
		}
		itemRowCount = 0;
		for(map<int,int>::iterator itemStep = jumpCount.begin();itemStep != jumpCount.end(); itemStep++){
			//(,itemStep->second)
			if(itemStep->first > 12 && itemStep->first < 20) //����������������12 - 20��
				itemRowCount += itemStep->second;
		}
		if(itemRowCount >= roiHeight / 4.0) //������Ч���򳬹��߶ȵ�1/4����Ϊ�ǳ���
			jumpRect.push_back(i);
	}

	return jumpRect;	
}

//�����Ƕ�
vector<Mat> position::TurnPosition(vector<Rect>)
{
	vector<Mat> plates;

	return plates;
}