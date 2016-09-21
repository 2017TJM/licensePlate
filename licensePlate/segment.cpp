#include "segment.h"
#include <numeric>
#include <fstream>

segment::segment(void)
{
}


segment::~segment(void)
{
}

vector<Rect> segment::SeperateWords(Mat sourceFrame){
	//����ͼ��
	imagePrepare(sourceFrame);
	//������С�߽�
	int upLoc,downLoc,leftLoc,rightLoc, speratValue;
	getPlateBoundaryUpDown(upLoc, downLoc);
	getPlateBoundaryLeftRight(leftLoc,rightLoc);
	vector<Rect> rectList;
	for(int i=0;i< wordsBoundList.size();i+=2)
	{
		Rect rectItem(wordsBoundList[i],upLoc,wordsBoundList[i+1] - wordsBoundList[i], downLoc - upLoc);
		rectList.push_back(rectItem);
		rectangle(sourceFrame,rectItem, Scalar(255),2);
	}
	imshow("imag6",sourceFrame);	

	return rectList;
}

//����Ԥ����
void segment::imagePrepare(Mat sourceFrame){
	//�ҶȻ�ͼ��
	cv::cvtColor(sourceFrame, plateInfo, CV_RGB2GRAY);
	//��˹�˲�
	cv::GaussianBlur(plateInfo,plateInfo, Size(3,3),0);
	//��Ե��⣬�����Ϊ��ֵͼ
	cv::Canny(plateInfo,plateInfo,100,250);
	//�ҵ�����
	cv::findContours(plateInfo,contourAll,hierarchy,CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	//��䳵��
	int platItemCount = contourAll.size();
	Rect wordArea;
	vector<Point> contoursAppr;

	for(int i=0;i<platItemCount;i++)  
    {  
		approxPolyDP(Mat(contourAll[i]),contoursAppr,5,true);
		wordArea = boundingRect(contoursAppr);
		if(wordArea.height <= 10) continue;		
//		cv::polylines(m4, contourAll[i], false,Scalar(255), 1);
//		rectangle(m4,Point(wordArea.x,wordArea.y), Point(wordArea.x + wordArea.width, wordArea.y + wordArea.height), Scalar(255));
		drawContours(plateInfo,contourAll,i,Scalar(255),CV_FILLED);  
    }
	
	//��ʾͼ��
	imshow("imag4",plateInfo);	
	
	platWidth = plateInfo.cols;
	platHeight = plateInfo.rows;
}

//Ѱ������ı߽�ֵ������ֵ��ƫ����
int getUpDownBoundIdx(const ushort *moveCount,const bool moveDown, const int len,const int checkValue)
{//�ҵ�����N��5����С�ھ�ֵ1/3��Ϊ����߽�
	int resultIdx = 0, flagCount = 0,boundCount=5;
	for(int i=1;i<len;i++)
	{
		//DownΪY������
		if(moveDown) moveCount++;
		else moveCount--;
		//ͳ�Ʊ߽�ֵ
		if(*(moveCount) < checkValue)
			flagCount++;
		else
			flagCount=0;
		if(flagCount >= boundCount)
		{//�߽�
			resultIdx = i;
			break;
		}
	}
	return resultIdx - (flagCount - 1);
}

//Ѱ������ı߽�ֵ������ֵ��ƫ����
int getLeftRightBoundIdx(const ushort *moveCount,const bool moveRight, const int len,const int checkValue)
{//�ҵ�����N��10����ֵ���ڵ�����Ϊ������Ч����
	int resultIdx = 0, flagCount = 0,boundCount=10;
	for(int i=1;i<len;i++)
	{
		//RightΪX������
		if(moveRight) moveCount++;
		else moveCount--;
		//ͳ�Ʊ߽�ֵ
		if(*(moveCount) > checkValue)
			flagCount++;
		else
			flagCount=0;
		if(flagCount >= boundCount)
		{//�߽�
			resultIdx = i;
			break;
		}
	}
	return resultIdx - (flagCount - 1);
}

//��ȡ�������±߽�
void segment::getPlateBoundaryUpDown(int &upLoc, int &downLoc){
	uchar * pMatInfo;
	ushort *moveCount = new ushort[platHeight];
	for(int i=0;i<platHeight;i++) 
		moveCount[i] = 0;
	for (int i = 0; i < platHeight; i++)
	{//ͼ��ѹ����һά��Y�᷽��
		pMatInfo = plateInfo.ptr<uchar>(i);
		for(int j=0;j<platWidth;j++){
			moveCount[i] += ((int)pMatInfo[j] == 255?1:0);
		}
	}
	int idx = platHeight/2, midAreaSum = moveCount[idx], midAreaMean, meanCount = 5;
	for(int i= 1 ;i<= meanCount;i++)
	{//����λ������N��5*2+1���������ֵ
		midAreaSum += moveCount[idx+i] + moveCount[idx-i];
	}
	midAreaMean = midAreaSum / (2 * meanCount + 1);
	//ԭ���ҵ�����N��5����С�ھ�ֵ1/3��Ϊ����߽�
	midAreaMean /= 3.0;
	upLoc = getUpDownBoundIdx(moveCount+idx-meanCount,false,idx-meanCount,midAreaMean);
	upLoc = idx-meanCount - upLoc;
	downLoc = getUpDownBoundIdx(moveCount+idx+meanCount,true,platHeight-idx-meanCount, midAreaMean);
	downLoc = idx+meanCount + downLoc;
	//�ͷſռ�
	delete(moveCount);
}

//��ȡ�������ұ߽�
void segment::getPlateBoundaryLeftRight(int &left, int &right){
	uchar * pMatInfo;
	ushort *moveCount = new ushort[platWidth];
	for(int i=0;i<platWidth;i++) 
		moveCount[i] = 0;
	for (int i = 0; i < platHeight; i++)
	{//ͼ��ѹ����һά��X�᷽��
		pMatInfo = plateInfo.ptr<uchar>(i);
		for(int j=0;j<platWidth;j++){
			moveCount[j] += ((int)pMatInfo[j] == 255?1:0);
		}
	}
	map<ushort,ushort> verticalCount;
	for( int i=0;i<platWidth;i++)
	{//ͳ�ƽ����ѹ������ֱ����Y�᷽��
		if(verticalCount.find(moveCount[i]) == verticalCount.end())
			verticalCount.insert(pair<int,int>(moveCount[i],1));
		else
			verticalCount[moveCount[i]]++;
	}
	//��N(20)�����ڵĴ���M��5������Сֵ�㣨ԭ���ָ���ĸ�Ŀո�ռ�õ�����ռ���࣬����Ч�������٣�
	//N��ͼƬ��N(20)�����صĸ߶���Ϊ�ָ�㣬M����ȥ�ո������Զ����أ�í���������ڿո�ͨ������������ص㣩
	//�����ֵ�����ǿո񣩻ᵼ�²���λ�ã�í����Ӱ�죬ʹ�������޷�ͨ���ո����غ��٣���������Ҫͨ��ĳһ��Сֵ��í�������зָ�
	//��ʵҲ����ֱ����һ������ֵ���зָ�
	int mostPixel = platWidth, speratValue = 0;
	for(map<ushort,ushort>::iterator itemStep = verticalCount.begin();itemStep != verticalCount.end(); itemStep++)
	{//mapĬ�ϰ�Keyֵ��������
		if(itemStep->first >= 20) break;
		if(itemStep -> second < mostPixel && itemStep -> second > 5){
			mostPixel = itemStep->second;
			speratValue = itemStep->first;
		}
	}
	//���������Сֵ����һ�����Բ�������Ϊ�����ֱ߽�ʱͨ��ȥ�������Թ��˵��߽磩
	left = getLeftRightBoundIdx(moveCount,true, platWidth/5.0, speratValue);
	right = getLeftRightBoundIdx(moveCount + platWidth,false, platWidth/5.0, speratValue);
	right = platWidth - right;
	//�ֽ��ַ�
	getWordsBoundary(moveCount,speratValue, left - 1, right + 1);
	delete(moveCount);
}


void segment::getWordsBoundary(const ushort * moveCount,int minValue, int start,int end)
{
	bool findStartFlag = false;//�������㣬�ұ����յ�
	const ushort * tmpCount = moveCount + start;
	for(int i=start;i<=end;i++,tmpCount++)
	{//����������
		if(!findStartFlag && *tmpCount > minValue 
		 || findStartFlag && *tmpCount <= minValue)
		{
			wordsBoundList.push_back(i);
			findStartFlag = !findStartFlag;
		}
	}
	if(wordsBoundList.size() %2 > 0) //ĩβ����
		wordsBoundList.push_back(end);

	int wordCount = wordsBoundList.size();
	for(int i=0;i<wordCount;i+=2)
	{//ȥ�������㣨����������С��N[7]���أ�
		if(wordsBoundList[i+1] - wordsBoundList[i] < 7)
		{
			vector<int>::iterator stepItem = wordsBoundList.begin()+i;
			wordsBoundList.erase(stepItem);
			stepItem = wordsBoundList.begin()+i;
			wordsBoundList.erase(stepItem);
			i-=2;
			wordCount-=2;
		}
	}
}