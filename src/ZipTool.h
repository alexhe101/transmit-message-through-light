#pragma once

#include"../bin/zlib-1.2.11/zlib.h"

#include"../bin/zlib-1.2.11/zconf.h"

#include <iostream>
using namespace std;
class ZipTool
{
public:
	static unsigned char* zip(unsigned char* src, unsigned long srcLen, unsigned long* destLen);//ѹ��,�����ѹ�����飬��ѹ������Ĵ�С�����ڴ���ѹ�������鳤�ȵı���
	static unsigned char* zip(unsigned char* src, unsigned long srcLen, unsigned long* destLen,int level);//�ֵȼ�ѹ��,0-9,9Ϊ����ѹ��
	static unsigned char* unzip(unsigned char* src, unsigned long srcLen, unsigned long* destLen);//��ѹ���������ѹ���飬����ѹ���鳤�ȣ�δѹ��ʱ�ĳ���
};

