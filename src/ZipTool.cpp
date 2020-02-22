#include "ZipTool.h"
#include"parse.h"
#include<cstring>
#define MAX_BUFFER 1024*1024*10

/*
�ӿ�˵��:
zip�������������ֱ�����ͨѹ���Ϳ���ѡ�ȼ���ѹ���㷨
unzip������һ����Ϊ��ѹ�㷨
zip������Ҫ�����ѹ������,��ѹ�����鳤�ȣ�ѹ���󳤶ȵ�ָ�룬�Լ�ѹ���ȼ�*(��ѡ)
unzip�����������ѹ���飬����ѹ���ȣ���ѹ��ĳ���
*/
unsigned char*  ZipTool::zip(unsigned char* src, unsigned long srcLen, unsigned long* destLen)
{
	//unsigned char* expArray = parseArray(src, srcLen);//��ȡ�����������

	//long expLen = srcLen * 8;//����������ĳ���

	unsigned char* tmp = new unsigned char[srcLen];//ѹ����Ĵ���ռ䣬��ʱ����Ϊ��srcͬ�ȴ�С
	
	*destLen = srcLen;//��ʼ��ѹ����ĳ��Ⱥ�ԭʼ������ͬ
	compress(tmp, destLen, src, srcLen);
	unsigned char* dest = new unsigned char[*destLen];//���ڴ���ѹ���������
	memcpy(dest, tmp, *destLen);
	delete []tmp;
	//delete[]expArray;
	cout << "ѹ���ɹ� " << "ѹ����Ϊ" << (*destLen /((1.0)* srcLen))<<endl;
	return dest;
}

unsigned char* ZipTool::zip(unsigned char* src, unsigned long srcLen, unsigned long* destLen,int level)
{
	unsigned char* tmp = new unsigned char[srcLen];
	*destLen = srcLen;
	compress2(tmp, destLen, src, srcLen,level);
	unsigned char* dest = new unsigned char[*destLen];
	memcpy(dest, tmp, *destLen);
	delete[]tmp;
	cout << "ѹ���ɹ� " << "ѹ����Ϊ" << (*destLen / ((1.0) * srcLen)) <<endl;
	return dest;
}

unsigned char* ZipTool::unzip(unsigned char* src, unsigned long srcLen, unsigned long* destLen)
{
	unsigned char* tmp = new unsigned char[MAX_BUFFER];//�ݴ��ѹ�������
	uncompress(tmp, destLen, src, srcLen);
	//cout << *destLen << "--��ѹ��" << endl;
	unsigned char* dest = new unsigned char[*destLen];
	memcpy(dest, tmp, *destLen);
	delete[]tmp;
	cout << "��ѹ�ɹ� " << "��ѹ�󳤶�Ϊ" << *destLen << endl;
	return dest;
}


