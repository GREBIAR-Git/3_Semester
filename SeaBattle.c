#include <windows.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#define Bt1 1

void RegClass(WNDPROC,LPCTSTR);

enum which{
  One1,
  One2,
  One3,
  One4,
  Two1,
  Two2,
  Two3,
  Three1,
  Three2,
  Four
};
enum which List=One1;
struct field{
    BOOL Empty;
    BOOL Alive;
    enum which Who;
    int remained;
};
struct field Our[10][10];
struct field Bot[10][10];
BOOL WhoseTurn=TRUE;
BOOL StageGame=TRUE;
int NumberShips=20;
int NumberShipsPlayer=20;
void Buum(HDC hdc,int One,int Two);
void Miss(HDC hdc,int One,int Two);
void Wound(HDC hdc,int One,int Two);
void DrawShip(HDC hdc,int One,int Two);
void Starting();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  RegClass(WndProc,"MainWin");
  HWND hwndMain = CreateWindow("MainWin", "SeaBattle", WS_OVERLAPPEDWINDOW, 40, 40, 870, 700, NULL, NULL, NULL, NULL);
  ShowWindow(hwndMain, SW_SHOWNORMAL);
  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return 0;
}

void RegClass(WNDPROC Proc,LPCTSTR szName)
{
  WNDCLASSA wcl;
  memset(&wcl, 0, sizeof(WNDCLASS));
  wcl.lpszClassName = szName;
  wcl.lpfnWndProc = Proc;
  wcl.hbrBackground = (HBRUSH)(COLOR_WINDOW + 23);
  wcl.hIcon = LoadIcon(NULL,IDI_ASTERISK);
  wcl.hCursor = LoadCursor(NULL,IDC_ARROW);
  RegisterClassA(&wcl);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_CREATE:
        {
            for(int i=0;i<11;i++)
            {
                for(int f=0;f<11;f++)
                {
                    Our[i][f].Empty=TRUE;
                    Our[i][f].Alive=TRUE;
                }
            }
            Starting();
            CreateWindow("button", "Start", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,360, 600, 120, 30, hwnd, (HMENU)Bt1, NULL, NULL);
            CreateWindow("static", "   A      B      C      D      E      F      G      H      I      J   ", WS_VISIBLE | WS_CHILD| WS_BORDER, 80, 15, 330, 20, hwnd, NULL, NULL, NULL);
            CreateWindow("static", NULL, WS_VISIBLE | WS_CHILD| WS_BORDER, 61, 34, 20, 330, hwnd, NULL, NULL, NULL);
            CreateWindow("static", NULL, WS_VISIBLE | WS_CHILD| WS_BORDER, 431, 34, 20, 330, hwnd, NULL, NULL, NULL);
            int f=1;
            for(int i=41;i<330;i+=33)
            {
                char buffer [33];
                itoa (f,buffer,10);
                CreateWindow("static", buffer, WS_VISIBLE | WS_CHILD, 67, i, 10, 20, hwnd, NULL, NULL, NULL);
                CreateWindow("static", buffer, WS_VISIBLE | WS_CHILD, 437, i, 10, 20, hwnd, NULL, NULL, NULL);
                f++;
            }
            CreateWindow("static", "10", WS_VISIBLE | WS_CHILD, 63, 338, 15, 20, hwnd, NULL, NULL, NULL);
            CreateWindow("static", "10", WS_VISIBLE | WS_CHILD, 433, 338, 15, 20, hwnd, NULL, NULL, NULL);
            CreateWindow("static", "   A      B      C      D      E      F      G      H      I      J   ", WS_VISIBLE | WS_CHILD| WS_BORDER, 450, 15, 330, 20, hwnd, NULL, NULL, NULL);
            CreateWindow("static", "You", WS_VISIBLE | WS_CHILD, 230, 400, 30, 20, hwnd, NULL, NULL, NULL);
            CreateWindow("static", "OPTIMIZIROVANAI OCHERED", WS_VISIBLE | WS_CHILD, 520, 400, 200, 20, hwnd, NULL, NULL, NULL);
            break;
        }
        case WM_COMMAND:
        {
            if(LOWORD(wParam)==Bt1)
            { 
                Starting();
                RECT rcClientRect;
                GetClientRect(hwnd, &rcClientRect);
                rcClientRect.left=420;
                InvalidateRect(hwnd,&rcClientRect,1);
                StageGame=FALSE;
            }
            break;
        }
        case WM_LBUTTONDOWN:
        {
            if (!StageGame)
            {
                int x = LOWORD(lParam);
                int y = HIWORD(lParam);
                int One,Two;
                if((y-34)>=0&&y<364&&(x-451)>=0&&x<780)
                {
                    One=(y-34)/33;
                    Two=(x-451)/33;
                }
                else
                {
                    return 1;
                }
                WhoseTurn=FALSE;
                if (Bot[One][Two].Empty==FALSE&&Bot[One][Two].Alive==TRUE)
                {
                    HDC hdc = GetDC (hwnd);
                    if(Bot[(y-34)/33][(x-451)/33].Alive==TRUE)
                    {
                        Wound(hdc,35+One*33,450+Two*33);
                        Bot[One][Two].Alive=FALSE;
                        for(int i=0;i<10;i++)
                        {
                            for(int f=0;f<10;f++)
                            {
                                if(!Bot[i][f].Empty&&Bot[i][f].Who==Bot[One][Two].Who&&Bot[i][f].remained>0)
                                {
                                    Bot[i][f].remained--;
                                    WhoseTurn=TRUE;
                                }
                            }
                        }
                    }
                    for(int i=0;i<10;i++)
                    {
                        for(int f=0;f<10;f++)
                        {
                            if(!Bot[i][f].Empty&&Bot[i][f].Who==Bot[One][Two].Who&&Bot[i][f].remained==0)
                            {
                                int test1=35+i*33;
                                int test2=450+f*33;
                                if(Bot[i-1][f-1].Empty&&(i-1>=0)&&(f-1>=0))
                                {
                                    Miss(hdc,test1-33,test2-33);
                                    Bot[i-1][f-1].Alive=FALSE;
                                }
                                if(Bot[i][f-1].Empty&&(f-1>=0))
                                {
                                    Miss(hdc,test1,test2-33);
                                    Bot[i][f-1].Alive=FALSE;
                                }
                                if(Bot[i-1][f].Empty&&(i-1>=0))
                                {
                                    Miss(hdc,test1-33,test2);
                                    Bot[i-1][f].Alive=FALSE;
                                }
                                if(Bot[i+1][f].Empty&&(i+1<=9))
                                {
                                    Miss(hdc,test1+33,test2);
                                    Bot[i+1][f].Alive=FALSE;
                                }
                                if(Bot[i][f+1].Empty&&(f+1<=9))
                                {
                                    Miss(hdc,test1,test2+33);
                                    Bot[i][f+1].Alive=FALSE;
                                }
                                if(Bot[i-1][f+1].Empty&&(i-1>=0)&&(f+1<=9))
                                {
                                    Miss(hdc,test1-33,test2+33);
                                    Bot[i-1][f+1].Alive=FALSE;
                                }
                                if(Bot[i+1][f+1].Empty&&(i+1<=9)&&(f+1<=9))
                                {
                                    Miss(hdc,test1+33,test2+33);
                                    Bot[i+1][f+1].Alive=FALSE;
                                }
                                if(Bot[i+1][f-1].Empty&&(i+1<=9)&&(f-1>=0))
                                {
                                    Miss(hdc,test1+33,test2-33);
                                    Bot[i+1][f-1].Alive=FALSE;
                                }
                                Buum(hdc,test1,test2);
                                WhoseTurn=TRUE;
                                NumberShips--;
                            }
                        }
                    }
                }
                else if (Bot[One][Two].Empty==TRUE&&Bot[One][Two].Alive==TRUE)
                {
                    HDC hdc = GetDC (hwnd);
                    Bot[One][Two].Alive=FALSE;
                    Miss(hdc,35+One*33,450+Two*33);
                }  
                else
                {
                    printf("tudsshe\n");
                    WhoseTurn=TRUE;
                }
                if (NumberShips==0)
                {
                    printf("Win\n");
                    StageGame=TRUE;
                    return;
                }
                if (!WhoseTurn) 
                {
                    HDC hdc = GetDC (hwnd);
                    int y = rand() % (10);
                    int x = rand() % (10);
                    BOOL DoblMove;
                    while(!DoblMove||NumberShipsPlayer!=0)
                    {
                        printf("n=%d",NumberShips);
                        while(Our[y][x].Alive==FALSE)
                        {
                            y = rand() % (10);
                            x = rand() % (10);
                        }
                        if(Our[y][x].Empty==FALSE)
                        {
                            Buum(hdc,35+y*33,80+x*33);
                            NumberShipsPlayer--;
                            Our[y][x].Alive=FALSE;
                        }
                        else
                        {
                            Miss(hdc,35+y*33,80+x*33);
                            Our[y][x].Alive=FALSE;
                            DoblMove=TRUE;
                        }
                    }
                    if(NumberShipsPlayer==0)
                    {
                        printf("loss");
                        StageGame=TRUE;
                    }
                }     
            }
            else
            {
                HDC hdc = GetDC (hwnd);
                int x = LOWORD(lParam);
                int y = HIWORD(lParam);
                int One,Two;
                if((y-34)>=0&&y<364&&(x-81)>=0&&x<410)
                {
                    One=(y-34)/33;
                    Two=(x-81)/33;
                }
                else
                {
                    return 1;
                }
                if(List==One1)
                {
                    DrawShip(hdc,35+One*33,80+Two*33);
                    Our[One][Two].Empty=FALSE;
    
                }
                else if(List==One2)
                {
                    DrawShip(hdc,35+One*33,80+Two*33);
                    Our[One][Two].Empty=FALSE;
                }
                else if(List==One3)
                {
                    DrawShip(hdc,35+One*33,80+Two*33);
                    Our[One][Two].Empty=FALSE;
                }
                else if(List==One4)
                {
                    Our[One][Two].Empty=FALSE;
                }
                else if(List==Two1)
                {
                    Our[One][Two].Empty=FALSE;
                }
                else if(List==Two2)
                {
                    Our[One][Two].Empty=FALSE;
                }
                else if(List==Two3)
                {
                    Our[One][Two].Empty=FALSE;
                }
                else if(List==Three1)
                {
                    Our[One][Two].Empty=FALSE;
                }
                else if(List==Three2)
                {
                    Our[One][Two].Empty=FALSE;
                }
                else if(List==Four)
                {
                   Our[One][Two].Empty=FALSE;
                }

            }
        }
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd,&ps);
            HPEN pen = CreatePen(PS_SOLID, 2, RGB(0,0,0));
            HBRUSH brush = CreateSolidBrush(RGB(0,0,0));
            SelectObject(hdc, pen);
            SelectObject(hdc, brush);
            for(int i=79;i<433;i+=33)
            {
                MoveToEx(hdc, i, 35, NULL);
                LineTo(hdc, i, 363);
            }
            for(int i=33;i<370;i+=33)
            {
                MoveToEx(hdc, 80, i, NULL);
                LineTo(hdc, 408, i);
            }
            for(int i=449;i<803;i+=33)
            {
                MoveToEx(hdc, i, 35, NULL);
                LineTo(hdc, i, 363);
            }
            for(int i=33;i<370;i+=33)
            {
                MoveToEx(hdc, 450, i, NULL);
                LineTo(hdc, 778, i);
            }
            EndPaint(hwnd,&ps);
            break;
        }
        case WM_DESTROY:
        {
            PostQuitMessage(0);
        }
        default:
        return DefWindowProcA(hwnd, msg, wParam, lParam);
    }
    return DefWindowProcA(hwnd, msg, wParam, lParam);
}

void Buum(HDC hdc,int One,int Two)
{
    HPEN pen = CreatePen(PS_SOLID, 2, RGB(255,0,0));
    HBRUSH brush = CreateSolidBrush(RGB(255,0,0));
    SelectObject(hdc, pen);
    SelectObject(hdc, brush);
    RoundRect(hdc,Two+4, One+3, Two+33-4,One+33-6,5,5);
}

void Miss(HDC hdc,int One,int Two)
{
    HPEN pen = CreatePen(PS_SOLID, 3, RGB(255,0,0));
    HBRUSH brush = CreateSolidBrush(RGB(255,0,0));
    SelectObject(hdc, pen);
    SelectObject(hdc, brush);
    Ellipse(hdc,Two+14, One+13, Two+33-14,One+33-16);
}

void Wound(HDC hdc,int One,int Two)
{
    HPEN pen = CreatePen(PS_SOLID, 2, RGB(0,255,0));
    HBRUSH brush = CreateSolidBrush(RGB(0,255,0));
    SelectObject(hdc, pen);
    SelectObject(hdc, brush);
    RoundRect(hdc,Two+4, One+3, Two+33-4,One+33-6,5,5);
}
void Starting()
{
    NumberShips=20;
    NumberShipsPlayer=20;
    StageGame=TRUE;
    for(int i=0;i<11;i++)
    {
        for(int f=0;f<11;f++)
        {
            Bot[i][f].Empty=TRUE;
            Bot[i][f].Alive=TRUE;
        }
    }
    Bot[0][0].Empty=FALSE;
    Bot[0][0].Who=One1;

    Bot[0][9].Empty=FALSE;
    Bot[0][9].Who=One2;

    Bot[9][0].Empty=FALSE;
    Bot[9][0].Who=One3;

    Bot[9][9].Empty=FALSE;
    Bot[9][9].Who=One4;

    Bot[2][0].Empty=FALSE;
    Bot[2][0].Who=Two1;
    Bot[3][0].Empty=FALSE;
    Bot[3][0].Who=Two1;

    Bot[2][9].Empty=FALSE;
    Bot[2][9].Who=Two2;
    Bot[3][9].Empty=FALSE;
    Bot[3][9].Who=Two2;

    Bot[9][6].Empty=FALSE;
    Bot[9][6].Who=Two3;
    Bot[9][7].Empty=FALSE;
    Bot[9][7].Who=Two3;

    Bot[5][0].Empty=FALSE;
    Bot[5][0].Who=Three1;
    Bot[6][0].Empty=FALSE;
    Bot[6][0].Who=Three1;
    Bot[7][0].Empty=FALSE;
    Bot[7][0].Who=Three1;

    Bot[9][2].Empty=FALSE;
    Bot[9][2].Who=Three2;
    Bot[9][3].Empty=FALSE;
    Bot[9][3].Who=Three2;
    Bot[9][4].Empty=FALSE;
    Bot[9][4].Who=Three2;

    Bot[0][2].Empty=FALSE;
    Bot[0][2].Who=Four;
    Bot[0][3].Empty=FALSE;
    Bot[0][3].Who=Four;
    Bot[0][4].Empty=FALSE;
    Bot[0][4].Who=Four;
    Bot[0][5].Empty=FALSE;
    Bot[0][5].Who=Four;

    for(int i=0;i<11;i++)
    {
        for(int f=0;f<11;f++)
        {
            if(Bot[i][f].Who==One1) Bot[i][f].remained=1;
            if(Bot[i][f].Who==One2) Bot[i][f].remained=1;
            if(Bot[i][f].Who==One3) Bot[i][f].remained=1;
            if(Bot[i][f].Who==One4) Bot[i][f].remained=1;
            if(Bot[i][f].Who==Two1) Bot[i][f].remained=2;
            if(Bot[i][f].Who==Two2) Bot[i][f].remained=2;
            if(Bot[i][f].Who==Two3) Bot[i][f].remained=2;
            if(Bot[i][f].Who==Three1) Bot[i][f].remained=3;
            if(Bot[i][f].Who==Three2) Bot[i][f].remained=3;
            if(Bot[i][f].Who==Four) Bot[i][f].remained=4;
            
        }
    }
}

void DrawShip(HDC hdc,int One,int Two)
{
    HPEN pen = CreatePen(PS_SOLID, 3, RGB(0,0,255));
    HBRUSH brush = CreateSolidBrush(RGB(0,0,255));
    SelectObject(hdc, pen);
    SelectObject(hdc, brush);
    RoundRect(hdc,Two+4, One+3, Two+33-4,One+33-6,5,5);
}