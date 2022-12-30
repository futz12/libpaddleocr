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

__ocr __declspec(dllexport) ocr_InitOcr(const bool use_vulkan//是否启用GPU
    , int _thread//启用线程数，-1为启用全部核心
);

int __declspec(dllexport) ocr_Deal(__ocr ocr, unsigned char* img_src, const int img_size, Object** ResList);

void __declspec(dllexport) ocr_DestructRet(Object* ResList);

void __declspec(dllexport) ocr_Destroy(__ocr ocr);