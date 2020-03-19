#include "Qrdecode.h"
#include"Decode.h"
#include"FileConvert.h"
#include<iostream>
using namespace std;
using namespace cv;


void Qrdecode::mydecode()
{
	char fileName[128];
	char tmpName[128];
	int indexptr = 0;//���ڴ洢Ŀ������ĵ�ǰ�±�
	Decode dec;
	string path = "imageOutput\\";
	FileConvert converter;
	converter.VideoTransToPic();
	/*
	�Ȱ�����ͼƬת��ΪROI

	*/
	int count = 1;
	Mat img;
	Mat dst;
	int foundindex = 0;
	while (true)
	{
		sprintf_s(fileName, "imageOutput\\%05d.png", count++);
		img = imread(fileName);
		if (img.data == NULL) break;
		int found = dec.findQranchor(img, dst);
		if (found == 1)
		{
			foundindex = count - 1;
			sprintf_s(tmpName, "imageTmp\\%05d.png", foundindex);
			imwrite(tmpName, dst);
			cout << "find anchor " << fileName << endl;
		}
		else
		{
			cout << "not found" << fileName << endl;
		}
	}
	
	//����ÿ��ͼƬ�����ͬ�����Ƿ����仯
		//ͬʱ���˵���Ӱͼ
		//������Ҫ�����ִ��봫��array�У���ȡ��ά������
	vector<string> fileNames;
	int currentFlag = 0;
	int originFlag = 0;
	char beginPic[120];
	sprintf_s(fileName, "imageTmp\\%05d.png", 1);
	fileNames.push_back(fileName);
	img = imread(fileName);
	currentFlag = dec.getFlag(img);
	originFlag = currentFlag;
	for (int i = 2; i <= foundindex; i++)
	{
		sprintf_s(fileName, "imageTmp\\%05d.png", i);
		img = imread(fileName);
		if (img.data == NULL) continue;
		currentFlag = dec.getFlag(img);
		if (currentFlag==2&&originFlag==1|| currentFlag == 1 && originFlag == 2)
		{
			if (dec.getRate(img) - 0.51 < 0)//������ǲ�Ӱ
			{
				fileNames.push_back(fileName);
				originFlag = currentFlag;
			}
		}
		else
		{
			continue;
		}
	}

	int numOfPic = fileNames.size();//converter.GetFilesNumber(path);
	cout << "num of true pic " << numOfPic << endl;
	char filename[10];
	img = imread(fileNames[numOfPic-1]);//�����ͼƬ��������
	int length;
	vector<int> Binary;
	unsigned char* output = NULL;
	int type;
	dec.decode(img, length, type,Binary);//Ϊ�˻�ȡ���һ�ŵĳ���
	int total_length = (numOfPic - 1) * 1022 + length;
	char outfilename[] = "out.bin";
	count=0;//ͼ���ļ�����
	

	cout << total_length << endl;
	cout << indexptr;

	if (type == SINGLE)//���ֻ�е���ͼ
	{
		//lengthΪ���ļ�����Ч���ݳ���
	}//�����˳�
	else
	{
		//lengthΪ���ļ��ĳ���
		Binary.clear();
	}

	count = 0;
	int tmplen;

	while (true)
	{
		if (count == numOfPic) break;
		img = imread(fileNames[count++]);//����ͼ��
		dec.decode(img, tmplen, type,Binary);
		if (tmplen > 1022) tmplen = 1022;
		cout << "dealing with " << fileNames [count-1]<< " "<<tmplen << endl;
	}

	output = dec.binToDec(Binary);

	converter.ByteToFile(output, outfilename, total_length);

	delete[]output;
}