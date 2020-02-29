#include "Decode.h"
void Decode::rotate(Mat& srcImg, Mat& dst)//����Դͼ��Ŀ�����,�õ���ƫ���ͼ�����dst
{
	Mat newImg = srcImg;
	QRCodeDetector qrDetector;
	vector<Point2f> list;
	qrDetector.detect(srcImg, list);
	Mat warpPerspective_mat(3, 3, CV_32FC1);//3��3��ת����
	Mat warpPerspective_dst = Mat::zeros(ROW, COL, newImg.type());//��ת���Ŀ��

	vector<Point2f> dstRect;//Ŀ�ĵ�
	dstRect.push_back(Point2f(0, 0));
	dstRect.push_back(Point2f(ROW-1, 0));
	dstRect.push_back(Point2f(ROW-1, COL-1));
	dstRect.push_back(Point2f(0, COL-1));
	warpPerspective_mat = getPerspectiveTransform(list, dstRect);//������ת����
	warpPerspective(newImg, warpPerspective_dst, warpPerspective_mat, warpPerspective_dst.size());//����͸�ӱ任

	dst = warpPerspective_dst;
	cvtColor(warpPerspective_dst, dst, COLOR_BGR2GRAY);
	threshold(dst, dst, 150, 255, THRESH_BINARY | THRESH_OTSU);
	cvtColor(dst, dst, COLOR_GRAY2BGR);//��ɫ�ָ�
	//srcImg = dst;
}

int Decode::getBit(Vec3b pix)
{
	int a, b, c;
	a = pix[0];
	b = pix[1];
	c = pix[2];
	if (a == 0 && b == 0 && c == 0)
	{
		return 0;//��
	}
	else if (a == 255 && b == 255 && c == 255)
	{
		return 1;//��
	}
}

int Decode::getType(Mat& srcImg)
{
	int typecode = 0;//code=3
	int k = 1;//KΪ�����������ϵ��
	for (int i = 0; i < 4; i++)
	{
		Vec3b pix = srcImg.at<Vec3b>(16, i);
		typecode += k * getBit(pix);
		k *= 2;
	}
	switch (typecode)
	{
	case 3: //1100
		return BEGIN;
	case 0://0000
		return NORMAL;
	case 12://0011
		return END;
	case 15:
		return SINGLE;
	}//С�˷��ֽ�
}

int Decode::getLength(Mat& srcImg)
{
	int length = 0;
	int k = 1;
	for (int i = 0; i < 12; i++)
	{
		Vec3b pix = srcImg.at<Vec3b>(16, 4+i);
		length += k * getBit(pix);
		k *= 2;
	}
	return length;
}

unsigned char* Decode::decode(Mat& srcImg,int type,int& length)
{
	Mat dst;
	rotate(srcImg, dst);
	if (type == SINGLE || type == END)
	{
		length = getLength(dst);
	}
	else
	{
		length = MAXSIZE;
	}
	unsigned char* tmp = new unsigned char[length];
	for (int i = 0; i < length; i++) { tmp[i] = 0; }
	//block A
	int index = 0;//�ݴ�������±�
	for (int i = 17; i < 80; i++)//������
	{
		for (int part = 0; part < 2; part++)//�����ֽ�
		{
			int code = 0;
			int k = 1;
			for (int j = 0; j < 8; j++)
			{
				Vec3b pix = dst.at<Vec3b>(i, j + part * 8);
				code += k * getBit(pix);
				k *= 2;
			}
			if(index>=length) return tmp;
			tmp[index++] = (unsigned char)code;
		}
	}
	//block A�������

	//Block B����

	for (int i = 0; i < 16; i++)//������
	{
		for (int part = 0; part < 8; part++)//�����ֽ�
		{
			int code = 0;
			int k = 1;
			for (int j = 0; j < 8; j++)
			{
				Vec3b pix = dst.at<Vec3b>(i, 16+j + part * 8);
				code += k * getBit(pix);
				k *= 2;
			}
			if (index >= length) return tmp;
			tmp[index++] = (unsigned char)code;
		}
	}

	//Block B�������

	//Block C���뿪ʼ

	for (int i = 16; i < 96; i++)//������
	{
		for (int part = 0; part < 10; part++)//�����ֽ�
		{
			int code = 0;
			int k = 1;
			for (int j = 0; j < 8; j++)
			{
				Vec3b pix = dst.at<Vec3b>(i, 16 + j + part * 8);
				code += k * getBit(pix);
				k *= 2;
			}
			if (index >= length) return tmp;
			tmp[index++] = (unsigned char)code;
		}
	}

	return tmp;
}