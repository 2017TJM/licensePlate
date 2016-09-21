#pragma once
#include <iostream>
#include <core/core.hpp>
#include <highgui/highgui.hpp>
#include <imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

//�ָ���ַ�
class segment
{
public:
	segment(void);
	~segment(void);

	//�ָ��ַ�
	vector<Rect> SeperateWords(Mat);

private:
	//����Ԥ����
	void imagePrepare(Mat);
	//��ȡ���Ʊ߽�
	void getPlateBoundaryUpDown(int&,int&);
	void getPlateBoundaryLeftRight(int&,int&);
	//��ȡ�ַ��߽�
	void getWordsBoundary(const ushort*,int, int,int);

	//����ͼƬ
	Mat plateInfo, platBinary;
	//����
	int platWidth, platHeight;
	//ͼ���е�����
	vector<vector<Point> > contourAll;
	vector<Vec4i> hierarchy;
	//���ַָ�
	vector<int> wordsBoundList;
};

