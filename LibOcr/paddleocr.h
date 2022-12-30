struct _ocr
{
    int count_thread;
    ncnn::Net dbNet;
    ncnn::Net crnnNet;
    const int dstHeight = 48;
};

typedef _ocr* __ocr;

struct Object
{
    cv::Rect_<int> box;
    float score;
    char* Text;
};

__ocr __declspec(dllexport) ocr_InitOcr(const bool use_vulkan//�Ƿ�����GPU
    , int _thread//�����߳�����-1Ϊ����ȫ������
);

int __declspec(dllexport) ocr_Deal(__ocr ocr, unsigned char* img_src, const int img_size, Object** ResList);

void __declspec(dllexport) ocr_DestructRet(Object* ResList);

void __declspec(dllexport) ocr_Destroy(__ocr ocr);