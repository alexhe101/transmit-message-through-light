#include<vector>
#include <opencv2/opencv.hpp>
//#include "opencv2/highgui/highgui.hpp" 
//#include "opencv2/core/core.hpp"
using namespace std;
using namespace cv;
#include"img_decode.h"
#include"img_code.h"

int main()
{
    char readPath[] = "�����ַ\\ͼƬ";    //����ͼƬ�ĵ�ַ

    vector<vector<uchar> > array;
    array = img_decode(readPath);

    char writePath[] = "Ŀ���ַ";        //���ͼƬ�ı���λ��
    img_code(array, writePath);

    return 0;
}