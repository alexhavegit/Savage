#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <wingdi.h>
// 

#define TM_TIMER_1 1

HINSTANCE hInst;
HWND hWnd;

BITMAPINFOHEADER bmiHeader = { 0 };
BITMAPINFO info = { 0 };
BITMAPV5HEADER header = { 0 };
char* aimp_buffer;





LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void PaintExperiments(HWND, HDC);
void DisplayError(DWORD, const char*);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    WNDCLASSEXW wcex = { 0 };

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    //wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_VCPROJECT1));
    wcex.hCursor = LoadCursor(0, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    //wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_VCPROJECT1);
    wcex.lpszClassName = L"wndclass";
    //wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    RegisterClassExW(&wcex);

    // Выполнить инициализацию приложения:
    hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

    hWnd = CreateWindowW(L"wndclass", L"Savage", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

    if (!hWnd)
    {
        return FALSE;
    }



    SetTimer(hWnd, TM_TIMER_1, 100, NULL);


    int counter = 0;
    int image_size = 800 * 450 * 3;

    aimp_buffer = (char*)malloc(image_size * sizeof(char));
    for (counter = 0; counter < image_size;)
    {
        aimp_buffer[counter++] = 0;
        aimp_buffer[counter++] = 100;
        aimp_buffer[counter++] = 0;
    }



    //ads_scrbuf->avo_buffer = (void*)aimp_buffer;
    //ads_scrbuf->im_height = 600;
   // ads_scrbuf->im_width = 600;
    //ads_scrbuf->im_scanline = 600;


    memset(&info, 0, sizeof(info));
    memset(&info.bmiHeader, 0, sizeof(info.bmiHeader));
    info.bmiHeader.biBitCount = 24;
    info.bmiHeader.biHeight = -450;   // !!! -
    info.bmiHeader.biWidth = 800;
    info.bmiHeader.biSize = sizeof(info.bmiHeader);
    info.bmiHeader.biPlanes = 1;
    info.bmiHeader.biCompression = BI_RGB;

    

    memset(&header, 0, sizeof(BITMAPV5HEADER));
    header.bV5Width = 800;
    header.bV5Height = 450;
    header.bV5BitCount = 24;
    header.bV5Size = sizeof(BITMAPV5HEADER);
    header.bV5Planes = 1;
    header.bV5Compression = BI_RGB;


    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}




LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Разобрать выбор в меню:
        switch (wmId)
        {
        case 12:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        

        PaintExperiments(hWnd, hdc);

        TextOut(hdc, 10, 10, L"experiments", 11);

        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}




void PaintExperiments(HWND hWnd, HDC hDC)
{


    //BYTE* pData = (BYTE*)aimp_buffer + sizeof(BITMAPINFOHEADER) + info.bmiHeader.biClrUsed * sizeof(COLORREF);

    HBITMAP hBitmap = CreateDIBitmap(GetDC(hWnd), (BITMAPINFOHEADER*)&header,
        CBM_INIT, (char*)aimp_buffer, &info, DIB_RGB_COLORS);

    // *hBitmap

    // (void*)pData
    // (void*)ads_scrbuf->avo_buffer
    //DisplayError(GetLastError(), L"BeginPaint");

    HBITMAP hBMP = (HBITMAP)LoadImage(hInst, L"D:\\Coding\\cpt64\\other_files\\video_memory_initial_image.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    GetDIBits(hDC, hBMP, 0, 450, (char*)aimp_buffer, &info, DIB_RGB_COLORS);

    /*
       HDC hMemDC = CreateCompatibleDC(hDC);
       SelectObject(hMemDC, hBMP);
       BitBlt(hDC, 0, 0, 1000, 1000, hMemDC, 0, 0, SRCCOPY);
       //DeleteDC(hMemDC);
     */


    int rand1 = 0;
    rand1 = (rand() % 10000000) * 10;

    for (int i = 100; i < 1000 + rand1; i++)
    {
        aimp_buffer[i] = 150;
    }




    int* p = (HBITMAP*)hBMP;


    //SetDIBitsToDevice(hDC, 0, 0, 100, 100, 0, 0, 0, 100, (char*)aimp_buffer, &info, DIB_RGB_COLORS);


    SetDIBitsToDevice(hDC, 0, 0, 800, 450, 0, 0, 0, 450, (char*)aimp_buffer, &info, DIB_RGB_COLORS);

    //HBITMAP CreateBitmap( [in] int  nWidth,  [in] int nHeight,  [in] UINT   nPlanes, [in] UINT  nBitCount, [in] const VOID * lpBits    );




}

/*
HBITMAP MyCreateBitmap(
    HDC hdc,
    INT cx,
    INT cy,
    INT nColors)
{
    BITMAPINFOHEADER bmih;

    if (nColors == 2) {
        return CreateBitmap(cx, cy, 1, 1, NULL);
    }
    else {
        bmih.biSize = sizeof(BITMAPINFOHEADER);
        bmih.biWidth = cx;
        bmih.biHeight = cy;
        bmih.biPlanes = 1;              // 1 plane, 4 bpp is
        bmih.biBitCount = 4;            // 16 colors.

        bmih.biCompression =
        bmih.biSizeImage =
        bmih.biXPelsPerMeter =
        bmih.biYPelsPerMeter =
        bmih.biClrUsed =
        bmih.biClrImportant = 0;

        return CreateDIBitmap(hdc, &bmih, 0L, NULL, NULL, 0);
    }
}






*/






void DisplayError(DWORD dw, const char* szFuncName)
{
    LPSTR buff;
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
        NULL,
        dw,
        LANG_NEUTRAL,
        (LPSTR)&buff,
        0,
        NULL);
    MessageBox(NULL, buff, szFuncName, MB_OK);
}