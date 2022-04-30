#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <wingdi.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>

// 

#define TM_SCREEN_REDRAW_TIMER 1
#define TM_TIMER_1 2

#define M_PI       3.14159265358979323846
#define M_PIdiv180 0.0174532925199



HINSTANCE hInst;
HWND hWnd;

bool ready = false;

BITMAPINFOHEADER bmiHeader = { 0 };
BITMAPINFO info = { 0 };
BITMAPV5HEADER header = { 0 };
int image_size = 800 * 450 * 3;
char* aimp_buffer;
HBITMAP MainBitmap;

int time = 0;
float watch;

typedef struct
{
    short r;
    short g;
    short b;

} SV_Color;

typedef struct
{
    double x;
    double y;

} SV_Point;

SV_Point p1 = { 300,100 };  //400, 300      //450, 330
SV_Point p2 = { 330,100 };
SV_Point p3 = { 300,115 };
SV_Point p4 = { 330,115 };




LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void PaintExperiments();    //HWND, HDC
void DisplayError(DWORD, const char*);
void SV_SetPixel(int, int, SV_Color);
void SV_Rotate_Point(double, double, double*, double*, double);


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


    SetTimer(hWnd, TM_SCREEN_REDRAW_TIMER, 15, NULL);  // 35
    SetTimer(hWnd, TM_TIMER_1, 35, NULL);


    aimp_buffer = (char*)malloc(image_size * sizeof(char));


    


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


        HDC hDC = BeginPaint(hWnd, &ps);


        PaintExperiments();    //hWnd, hDC
        
        SetDIBitsToDevice(hDC, 0, 0, 800, 450, 0, 0, 0, 450, (char*)aimp_buffer, &info, DIB_RGB_COLORS);



        //TextOut(hdc, 10, 10, L"experiments", 11);

        EndPaint(hWnd, &ps);

        ready = true;
    }
    break;

    case WM_TIMER:

        switch (wParam)
        {
        case TM_SCREEN_REDRAW_TIMER:



            //InvalidateRect(hWnd, 0, TRUE);

            InvalidateRect(hWnd, NULL, FALSE);

            return 0;


        case TM_TIMER_1:
            //if (ready)
            // process the 10-second timer 

            if (time < 100)
            {
                time++;
            }
            else
            {
                time = 0;
            }

            


            return 0;
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




void PaintExperiments()  // HWND hWnd, HDC hDC
{

    memset((char*)aimp_buffer, 0, image_size * sizeof(char));

    /*

    int counter = 0;
    for (counter = 0; counter < image_size;)
    {
        aimp_buffer[counter++] = 0;
        aimp_buffer[counter++] = 100;
        aimp_buffer[counter++] = 0;
    }
*/

    // MainBitmap = CreateDIBitmap(GetDC(hWnd), (BITMAPINFOHEADER*)&header, CBM_INIT, (char*)aimp_buffer, &info, DIB_RGB_COLORS);


    //HBITMAP hBMP = (HBITMAP)LoadImage(hInst, L"D:\\Coding\\cpt64\\other_files\\video_memory_initial_image.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    //GetDIBits(hDC, hBMP, 0, 450, (char*)aimp_buffer, &info, DIB_RGB_COLORS);


    /*
       HDC hMemDC = CreateCompatibleDC(hDC);
       SelectObject(hMemDC, hBMP);
       BitBlt(hDC, 0, 0, 1000, 1000, hMemDC, 0, 0, SRCCOPY);
       //DeleteDC(hMemDC);
     */


    /*
    int rand1 = 0;
        rand1 = (rand() % 10000000) * 10;

    for (int i = 100; i < 1000 + rand1; i++)
    {
        aimp_buffer[i] = 150;
    }
    */

    double Ax = 0, Ay = 0;
    double Cx = 315 , Cy = 107;     // 400 200
    // SV_Point p1 = { 450, 330 };

    Ax = 100 + time;
    Ay = 200;

    SV_Color Cc = { 255, 255, 255 };
    SV_SetPixel (Cx, Cy, Cc);

    //p1.x += time;
   
    //p1.x = 450;
    //p1.y = 330;

    double angle = time * 3.6;  // 0.89  

    angle = 20;


    SV_Rotate_Point(Cx, Cy, &p1.x, &p1.y, angle);   //
    SV_Rotate_Point(Cx, Cy, &p2.x, &p2.y, angle);
    SV_Rotate_Point(Cx, Cy, &p3.x, &p3.y, angle);
    SV_Rotate_Point(Cx, Cy, &p4.x, &p4.y, angle);



    SV_Color c = { 255, 255, 255 };  //  255, 0, 0 

    SV_SetPixel(p1.x, p1.y, c);
    SV_SetPixel(p2.x, p2.y, c);
    SV_SetPixel(p3.x, p3.y, c);
    SV_SetPixel(p4.x, p4.y, c);

    


    //int* p = (HBITMAP*)hBMP;



    
    //HBITMAP CreateBitmap( [in] int  nWidth,  [in] int nHeight,  [in] UINT   nPlanes, [in] UINT  nBitCount, [in] const VOID * lpBits    );


    wchar_t cWatch[30] = L"12345";


    char array[30];
    //sprintf_s(array,20, "%f", watch);

    swprintf(cWatch, 20, L"%f", watch);

    //TextOut(hDC, 10, 10, cWatch, 20); //cWatch

}



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


void SV_SetPixel(int x, int y, SV_Color col)
{
    //color col = { 255,255,255 };
    int ny = 450 - y;
    int pointNum = 0;

    pointNum = (ny * 800 + x) * 3;

    aimp_buffer[pointNum] = col.b;
    aimp_buffer[pointNum + 1] = col.g;
    aimp_buffer[pointNum + 2] = col.r;
}


//void increent(int& val){    val++;}


void SV_Rotate_Point(double CenterX, double CenterY, double *PointX, double *PointY, double angle)
{
    double lengthCA = 0;

    double p1 = pow((CenterX - *PointX), 2);
    double p2 = pow((CenterY - *PointY), 2);


    lengthCA = sqrt(p1 + p2);

    //lengthCA = 199;

    //lengthAB = lengthCA * sin(angle * M_PIdiv180);

    double ang = (*PointY - CenterY) / lengthCA;

    

    //angle = 1;

    //ang = 0.999;

    if (*PointX < CenterX)
    {
        ang = (double)(CenterY - *PointY) / lengthCA;
    }

    double a = (double)acos(ang) * 180 / M_PI;


    if (*PointX == CenterX)
    {
        if (*PointY > CenterY)
        {a = 0;}else{a = 180;}
    }

    if (*PointY == CenterY)
    {
        if (*PointX > CenterX)
        {a = 90;}else{a = 270;}
    }

    if (*PointX < CenterX)
    {
        a += 180;
    }


    //angle = 10;

    

    //double sinAlpha = sin((89 * M_PI) / 180);
    //double cosAlpha = cos((angle * M_PI) / 180);


    //double xCircle1 = CenterX + (*PointX - CenterX) * cosAlpha - (*PointY - CenterY) * sinAlpha;
    //double yCircle1 = CenterY + (*PointX - CenterX) * sinAlpha + (*PointY - CenterY) * cosAlpha;



    *PointX = CenterX + lengthCA * sin((angle+a * M_PI)/180);  // sin(angle * val)
    *PointY = CenterY + lengthCA * cos((angle+a * M_PI)/180);  // cos(angle * val)



    double p12 = pow((CenterX - *PointX), 2);
    double p22 = pow((CenterY - *PointY), 2);


    double lengthCA2 = sqrt(p12 + p22);

    double dif = lengthCA - lengthCA2;


    watch = dif;
    
}








