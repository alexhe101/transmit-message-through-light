#include"code.h"

namespace Code
{
	constexpr unsigned long BytesPerFrame = 1022;
	constexpr int FrameSize = 100;
	constexpr int FrameOutputRate = 10;
	constexpr int SafeAreaWidth = 2;
	constexpr int BPatternSize = 18;
	//  constexpr int SPatternSize = 5;    
	constexpr int areanum = 5;
	const Vec3b pixel[8] =
	{
		Vec3b(0,0,0),Vec3b(0,0,255),Vec3b(0,255,0),Vec3b(0,255,255),
		Vec3b(255,0,0),Vec3b(255,0,255),Vec3b(255,255,0),Vec3b(255,255,255)
	};
	const unsigned long len_max[areanum] = { 126,128,512,128,128 };
	const int areapos[areanum][2][2] =
	{//[2][2],��һά�ȴ���߿��ڶ�ά�ȴ������Ͻ�����
		{{63,16},{BPatternSize + 1,SafeAreaWidth}},
		{{16,64},{SafeAreaWidth,BPatternSize}},
		{{64,64},{BPatternSize,BPatternSize}},
		{{16,64},{FrameSize - BPatternSize,BPatternSize}},
		{{64,16},{BPatternSize,FrameSize - BPatternSize}}
	};
	enum color
	{
		Black = 0,
		White = 7
	};
	enum class FrameType
	{
		Start = 0,
		Normal = 1,
		End = 2,
		Single = 3,

	};
	void Main(unsigned char* info, unsigned long len, const char* savePath, const char* outputFormat) // �ַ�����Ϣ�����ȣ�����·���������ʽ
	{
		Mat output;
		char fileName[128];
		int count = 0;
		if (len <= BytesPerFrame)
		{
			unsigned char BUF[BytesPerFrame + 5];
			memcpy(BUF, info, sizeof(unsigned char) * len);
			for (int i = len; i <= BytesPerFrame; ++i)
				BUF[i] = rand() % 256;
			output = amplify(CodeFrame(FrameType::Single, BUF, len, 0));
			sprintf_s(fileName, "%s\\%05d.%s", savePath, count++, outputFormat);
			imwrite(fileName, output);
		}
		else
		{
			int PicNum = 0;
			output = amplify(CodeFrame(FrameType::Start, info, len, PicNum++));
			sprintf_s(fileName, "%s\\%05d.%s", savePath, count++, outputFormat);
			imwrite(fileName, output);
			do
			{
				len -= BytesPerFrame;
				info += BytesPerFrame;
				if (len > BytesPerFrame)
					output = amplify(CodeFrame(FrameType::Normal, info, BytesPerFrame, PicNum++));
				else
				{
					unsigned char BUF[BytesPerFrame + 5];
					memcpy(BUF, info, sizeof(unsigned char) * len);
					for (int i = len; i <= BytesPerFrame; ++i)
						BUF[i] = rand() % 256;
					output = amplify(CodeFrame(FrameType::End, BUF, len, PicNum++));
				}
				sprintf_s(fileName, "%s\\%05d.%s", savePath, count++, outputFormat);
				imwrite(fileName, output);
				/*              �����Ƿ������ڶ�λ
								int i = 0;
								Mat dst = imread(fileName);
								QRCodeDetector qrDetector;
								vector<Point2f> list;
								qrDetector.detect(dst, list);
								if (list.empty())
								{
									cout << i <<" nothing" << endl;
								}
								i++;
				*/
			} while (len > BytesPerFrame);
		}
		return;
	}
	Mat amplify(const Mat& src)
	{
		Mat output;
		constexpr int FrameOutputSize = FrameSize * FrameOutputRate;
		output = Mat(FrameOutputSize, FrameOutputSize, CV_8UC3);
		for (int i = 0; i < FrameOutputSize; ++i)
		{
			for (int j = 0; j < FrameOutputSize; ++j)
			{
				output.at<Vec3b>(i, j) = src.at<Vec3b>(i / FrameOutputRate, j / FrameOutputRate);
			}
		}
		return output;
	}
	Mat CodeFrame(FrameType frameType, unsigned char* info, unsigned long tailLen, int PicNum)
	{
		Mat codeMat = Mat(FrameSize, FrameSize, CV_8UC3, Vec3b(255, 255, 255));     //��ƬΪ��ɫ
		if (frameType == FrameType::Start || frameType == FrameType::Normal)
			//3/1/14:30����������󳤶ȣ���󳤶������һ�ų���+BytesPerFrame*��������      
			tailLen = BytesPerFrame;
		BulidSafeArea(codeMat);       //���ư�ȫ��
		BulidQrPoint(codeMat);        //���ƶ�λ��       
		BulidFrameFlag(codeMat, frameType, tailLen);
		BulidPicNum(codeMat, PicNum);
		if (tailLen != BytesPerFrame)           //����ʱ����ͼ��Ҫ���룬δȷ�����������
			tailLen = BytesPerFrame;
		for (int i = 0; i < areanum && tailLen > 0; ++i)
		{
			int len_now = std::min(tailLen, len_max[i]);
			BulidInfoRect(codeMat, info, len_now, i);
			tailLen -= len_now;
			info += len_now;
		}

		return codeMat;
	}
	void BulidSafeArea(Mat& mat)  //������ȫ���
	{
		constexpr int pos[4][2][2] =
		{
			{{0,FrameSize},{0,SafeAreaWidth}},
			{{0,FrameSize},{FrameSize - SafeAreaWidth,FrameSize}},
			{{0, SafeAreaWidth },{0,FrameSize}},
			{{FrameSize - SafeAreaWidth,FrameSize},{0,FrameSize}}
		};
		for (int k = 0; k < 4; ++k)
			for (int i = pos[k][0][0]; i < pos[k][0][1]; ++i)
				for (int j = pos[k][1][0]; j < pos[k][1][1]; ++j)
					mat.at<Vec3b>(i, j) = pixel[White];
		return;
	}
	void BulidQrPoint(Mat& mat)
	{
		//���ƴ��ά��ʶ���
		constexpr int pointPos[4][2] =
		{
			{0,0},
			{0,FrameSize - BPatternSize},
			{FrameSize - BPatternSize,0},
			{FrameSize - BPatternSize,FrameSize - BPatternSize}
		};
		const Vec3b vec3bBig[9] =
		{
			pixel[Black],
			pixel[Black],
			pixel[Black],
			pixel[White],
			pixel[White],
			pixel[Black],
			pixel[Black],
			pixel[White],
			pixel[White]
		};
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < BPatternSize; ++j)
				for (int k = 0; k < BPatternSize; ++k)
					mat.at<Vec3b>(pointPos[i][0] + j, pointPos[i][1] + k) =
					vec3bBig[(int)max(fabs(j - 8.5), fabs(k - 8.5))];       //��ӡ����
	}
	void BulidInfoRect(Mat& mat, unsigned char* info, unsigned long len, int areaID)
	{
		const unsigned char* pos = (const unsigned char*)info;
		const unsigned char* end = pos + len;
		for (int i = 0; i < areapos[areaID][0][0]; ++i)
		{
			uint32_t outputCode = 0;
			for (int j = 0; j < areapos[areaID][0][1] / 8; ++j)    // 1 char = 8 �ֽ�
			{
				outputCode |= *pos++;
				/*for (int k = 0; k < 3; ++k)
				{
					outputCode <<= 8;
					if (pos != end)
						outputCode |= *pos++;
				}*/
				for (int k = areapos[areaID][1][1]; k < areapos[areaID][1][1] + 8; ++k)
				{
					//mat.at<Vec3b>(i+areapos[areaID][1][0], j*8+k) = pixel[outputCode&7];
					//outputCode >>= 3;
					mat.at<Vec3b>(i + areapos[areaID][1][0], j * 8 + k) = pixel[(outputCode & 1) ? 7 : 0];
					outputCode >>= 1;
				}
				if (pos == end) break;
			}
			if (pos == end) break;
		}
	}
	void BulidFrameFlag(Mat& mat, FrameType frameType, unsigned long tailLen)
	{
		switch (frameType)
		{
		case FrameType::Start:
			mat.at<Vec3b>(BPatternSize, SafeAreaWidth) = pixel[Black];
			mat.at<Vec3b>(BPatternSize, SafeAreaWidth + 1) = pixel[Black];
			break;
		case FrameType::Normal:
			mat.at<Vec3b>(BPatternSize, SafeAreaWidth) = pixel[Black];
			mat.at<Vec3b>(BPatternSize, SafeAreaWidth + 1) = pixel[White];
			break;
		case FrameType::End:
			mat.at<Vec3b>(BPatternSize, SafeAreaWidth) = pixel[White];
			mat.at<Vec3b>(BPatternSize, SafeAreaWidth + 1) = pixel[Black];
			break;
		case FrameType::Single:
			mat.at<Vec3b>(BPatternSize, SafeAreaWidth) = pixel[White];
			mat.at<Vec3b>(BPatternSize, SafeAreaWidth + 1) = pixel[White];
			break;
		}
		for (int i = 4; i < 16; ++i)
		{
			mat.at<Vec3b>(BPatternSize, SafeAreaWidth + i) = pixel[(tailLen & 1) ? 7 : 0];
			tailLen >>= 1;
		}
	}
	void BulidPicNum(Mat& mat, int PicNum)
	{
		mat.at<Vec3b>(BPatternSize, SafeAreaWidth + 2) = pixel[(PicNum & 1) ? 7 : 0];
		mat.at<Vec3b>(BPatternSize, SafeAreaWidth + 3) = pixel[(PicNum & 1) ? 7 : 0];
	}
}