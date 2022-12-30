// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

extern std::vector<std::string> keys;
extern std::vector<unsigned char> dbNet_param;
extern std::vector<unsigned char> crnnNet_param;
extern std::vector<unsigned char> dbNet_bin;
extern std::vector<unsigned char> crnnNet_bin;

int LoadStream(WORD wResID, char** pData, HMODULE hModule)
{
    HRSRC   hResInfo = FindResource(hModule, MAKEINTRESOURCE(wResID), TEXT("BIN"));
    HGLOBAL hResData = LoadResource(hModule, hResInfo);
    LPVOID    lpResData = LockResource(hResData);
    DWORD dwSize = SizeofResource(hModule, hResInfo);

    if (lpResData == nullptr || dwSize == 0)
    {
        return 0;
    }

    *pData = (char*)malloc(dwSize);
    if (!pData)
    {
        return 0;
    }

    memcpy(*pData, lpResData, dwSize);
    FreeResource(hResData);

    return dwSize;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        char* det_bin, * det_param, * rec_bin, * rec_param, * memkeys;
        int s_det_bin, s_det_param, s_rec_bin, s_rec_param, s_memkeys;

        s_det_bin = LoadStream(IDR_BIN1, &det_bin, hModule);
        s_det_param = LoadStream(IDR_BIN2, &det_param, hModule);
        s_rec_bin = LoadStream(IDR_BIN3, &rec_bin, hModule);
        s_rec_param = LoadStream(IDR_BIN4, &rec_param, hModule);
        s_memkeys = LoadStream(IDR_BIN5, &memkeys, hModule);

        dbNet_param.insert(dbNet_param.end(), det_param, det_param + s_det_param);
        dbNet_bin.insert(dbNet_bin.end(), det_bin, det_bin + s_det_bin);

        crnnNet_param.insert(crnnNet_param.end(), rec_param, rec_param + s_rec_param);
        crnnNet_bin.insert(crnnNet_bin.end(), rec_bin, rec_bin + s_rec_bin);

        dbNet_param.push_back(0);
        crnnNet_param.push_back(0);

        // load keys

        std::istringstream inStr(memkeys);
        std::string line;
        int size = 0;
        while (getline(inStr, line))
        {
            keys.emplace_back(line);
            size++;
        }

        delete[]det_bin;
        delete[]det_param;
        delete[]rec_bin;
        delete[]rec_param;
        delete[]memkeys;

        std::cout << cv::getBuildInformation() << std::endl;
        int gpu_count = ncnn::get_gpu_count();
        std::cout << "There are supported " << ncnn::get_gpu_count() << " gpus" << std::endl;
        for (int i = 0; i < gpu_count; i++)
        {
            std::cout << ncnn::get_gpu_device(i)->info.device_name() << std::endl;
        }
        std::cout << "Ocr Core : ch_PP-OCRv3" << std::endl;
        std::cout << "libOcr had inited" << std::endl;
    }
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

