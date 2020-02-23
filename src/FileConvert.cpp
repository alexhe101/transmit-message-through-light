#define _CRT_SECURE_NO_WARNINGS
#include "FileConvert.h"
#include<iostream>
#include<ctime>
#include<cstdio>

using namespace std;


FileConvert::~FileConvert()
{
	delete[]dst;
}

unsigned long FileConvert::GetFileSize(FILE* p)
{
	unsigned int current_read_position = ftell(p);
	fseek(p, 0, SEEK_END);
	unsigned long size = ftell(p);
	fseek(p, current_read_position, SEEK_SET);
	return size;
}

unsigned char* FileConvert::FileToByte(char filename[], unsigned long* size)
{
	p = fopen(filename, "rb");
	*size = GetFileSize(p);
	unsigned char* dst = new unsigned char[*size];
	fread(dst, sizeof(unsigned char), *size, p);
	fclose(p);
	return dst;
}//��ȡ�ļ���������

void FileConvert::GenerateRandomArray(unsigned char* src, unsigned long size)
{
	srand((unsigned)time(NULL));
	for (int i = 0; i < size; i++)
	{
		src[i] = rand() % 128;
	}
}//����ָ����С�����������

void FileConvert::ByteToFile(unsigned char* src, char filename[], unsigned long size)
{
	p = fopen(filename, "wb");
	fwrite(src, sizeof(unsigned char), size, p);
	fclose(p);
}//������д���ļ���

void FileConvert::GenerateRandFile(char filename[], unsigned long size)
{
	unsigned char* tmp = new unsigned char[size];
	GenerateRandomArray(tmp, size);
	ByteToFile(tmp, filename, size);
	delete[]tmp;
}//��������ļ�