/****************************************************************************************************************
* Window.cpp                                                                                                    *
* AUTOR: Thiago Silva                                                                                           *
* CRIACAO:            29 JULHO 2024                                                                             *
* ULTIMA MODIFICACAO: 08 AGOSTO 2024                                                                            *
* COMPILADOR:         Microsoft Visual C++ 2022                                                                 *
* DESCRICAO: Codigo principal da aplicacao, criacao e configuracao da janela e dos campos utilizado Win32 API.  *
*****************************************************************************************************************/


// Inclui os tipos basicos e as funcoes da Win32 API
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <sstream>
#include <algorithm>
#include <string>
#include "Resources.h"
#include "PDT.h"
using std::stringstream;
using namespace std;

// Habilita estilos e visuais modernos
#pragma comment(linker, "\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")


// Define o uso de caracteres UNICODE
#define UNICODE
#define UNICODE_

// Definicoes dos labels
#define R1_LABEL_ID 100
#define R2_LABEL_ID 200
#define RC_LABEL_ID 300
#define RE_LABEL_ID 400
#define BETA_LABEL_ID 500
#define VCC_LABEL_ID 600

// Ids
#define R1_ID 700
#define R2_ID 800
#define RC_ID 900
#define RE_ID 1000
#define BETA_ID 1100
#define VCC_ID 1200
#define STATIC_TEXT_O_ID 1300
#define STATIC_TEXT_I_ID 1400
#define BUTTON_ID 1500
#define GROUPBOX_ID 1600
#define MENU_ABOUT_ID 1700
#define MENU_SRC_ID 1800

// Variaveis escopo global
HFONT hFont;
HFONT hTitleFont;
HFONT hOutputFont;

HWND hStaticTextO;
HWND hStaticTextI;
HWND hButton;
HWND hGroupBox;
HINSTANCE hInstanceApp;
HMENU hMenu;
RECT clientRect;
bool isClean = true;


// Handle dos valores de entrada
HWND hR1;
HWND hR1Label;
HWND hR2;
HWND hR2Label;
HWND hRC;
HWND hRCLabel;
HWND hRE;
HWND hRELabel;
HWND hBETA;
HWND hBETALabel;
HWND hVCC;
HWND hVCCLabel;

// Prototipo do procedimento da janela
LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);

// Prototipo do quantificador
double quantify(wstring args);

// Programa principal
int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ INT nCmdShow)
{
    HWND         hwnd;
    MSG          msg;
    WNDCLASSEX     wndclass{};

    // Define uma window class name
    wndclass.cbSize = sizeof(wndclass);
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WinProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    wndclass.lpszMenuName = L"Menu";
    wndclass.lpszClassName = L"BasicWindow";
 
    // Registra a nova class
    if (!RegisterClassEx(&wndclass))
    {
        MessageBox(NULL, L"Erro na criacao da janela!", L"Analise CC em Circuitos PDT", MB_ICONERROR);
        return 0;
    }

    hInstanceApp = hInstance;

    // Cria fonte 
    hFont = CreateFont(25, 0, 0, 0, FW_EXTRABOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, VARIABLE_PITCH | FF_SWISS, TEXT("Arial"));
    
    // Cria a fonte do titulo
    hTitleFont = CreateFont(35, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, VARIABLE_PITCH | FF_SWISS, TEXT("Arial"));

    // Cria a fonte da saida
    hOutputFont = CreateFont(22, 0, 0, 0, FW_EXTRABOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, VARIABLE_PITCH | FF_SWISS, TEXT("Arial"));

    // Usado para centralizar janela na tela
    int xWndPos = GetSystemMetrics(SM_CXSCREEN) / 2 - 460;
    int yWndPos = GetSystemMetrics(SM_CYSCREEN) / 2 - 260;
    int wndWidth = 920;
    int wndHeight = 520;


    // Cria a janela principal
    hwnd = CreateWindowEx(NULL, L"BasicWindow",  // Classe da janela
        L"Analise CC em Circuitos PDT",          // Titulo da janela
        WS_OVERLAPPED | WS_SYSMENU,              // Estilo da janela
        xWndPos, yWndPos,                        // Posicao x, y inicial
        wndWidth, wndHeight,                     // Largura e comprimento iniciais
        NULL,                                    // Identificador do objeto pai
        hMenu,                                   // Identificador do menu
        hInstance,                               // Identificador da aplicacao
        NULL);                                   // Parametros de criacao

    // Mostra e atualiza a janela
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Entra no lacdo principal de tratamento de eventos
    while (GetMessage(&msg, NULL, 0, 0))
    {
        // Traduz as mensagens
        TranslateMessage(&msg);

        // Envia as mensagens para a "Window Procedure"
        DispatchMessage(&msg);
    }

    // Finaliza programa
    return int(msg.wParam);
}


// Gerenciador de mensagens do sistema
LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc = nullptr; // Dispositivo Grafico (handle device context)
    PAINTSTRUCT ps{}; // Armazena a regiao invalidada da janela
    RECT rect{};

    switch (msg)
    {
    // CRIACAO DA JANELAS DA APLICACAO
    case WM_CREATE:
        // pega as dimensoes da janela para o posicionamento de objetos
        GetClientRect(hwnd, &clientRect);
        
        // GroupBox que exibe os resultados
        hGroupBox = CreateWindowEx(NULL, L"BUTTON", NULL, WS_CHILD | WS_VISIBLE | BS_GROUPBOX, clientRect.left + 375, clientRect.top + 40, 500, 395, hwnd, HMENU(GROUPBOX_ID), hInstanceApp, NULL);

        // Definicao do titulo da saida
        hStaticTextO = CreateWindowEx(NULL,
            L"STATIC", L"Valores do Circuito", WS_CHILD | WS_VISIBLE, 555, 30, 275, 50, hwnd, HMENU(STATIC_TEXT_O_ID), hInstanceApp, NULL);
        SendMessage(hStaticTextO, WM_SETFONT, (WPARAM)hTitleFont, TRUE);

        // Definicao do titulo da entrada
        hStaticTextI = CreateWindowEx(NULL, 
            L"STATIC", L"Valores de entrada", WS_CHILD | WS_VISIBLE, 55, 30, 275, 150, hwnd, HMENU(STATIC_TEXT_I_ID), hInstanceApp, NULL);
        SendMessage(hStaticTextI, WM_SETFONT, (WPARAM)hTitleFont, TRUE);

        // Caixa de texto para R1
        hR1 = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_TABSTOP, 115, 100, 190, 30, hwnd, HMENU(R1_ID), hInstanceApp, NULL);
        SendMessage(hR1, WM_SETFONT, (WPARAM)hFont, TRUE);

        // Label para R1
        hR1Label = CreateWindowEx(NULL,
            L"STATIC", L"R1", WS_CHILD | WS_VISIBLE, 70, 98, 45, 35, hwnd, HMENU(R1_LABEL_ID), hInstanceApp, NULL);
        SendMessage(hR1Label, WM_SETFONT, (WPARAM)hTitleFont, TRUE);

        // Caixa de texto para R2
        hR2= CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_TABSTOP, 115, 150, 190, 30, hwnd, HMENU(R2_ID), hInstanceApp, NULL);
        SendMessage(hR2, WM_SETFONT, (WPARAM)hFont, TRUE);

        // Label para R2
        hR2Label = CreateWindowEx(NULL,
            L"STATIC", L"R2", WS_CHILD | WS_VISIBLE, 70, 148, 45, 35, hwnd, HMENU(R2_LABEL_ID), hInstanceApp, NULL);
        SendMessage(hR2Label, WM_SETFONT, (WPARAM)hTitleFont, TRUE);

        // Caixa de texto para RC
        hRC = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_TABSTOP, 115, 200, 190, 30, hwnd, HMENU(RC_ID), hInstanceApp, NULL);
        SendMessage(hRC, WM_SETFONT, (WPARAM)hFont, TRUE);

        // Label para RC
        hRCLabel = CreateWindowEx(NULL,
            L"STATIC", L"RC", WS_CHILD | WS_VISIBLE, 70, 198, 45, 35, hwnd, HMENU(RC_LABEL_ID), hInstanceApp, NULL);
        SendMessage(hRCLabel, WM_SETFONT, (WPARAM)hTitleFont, TRUE);

        // Caixa de texto para RE
        hRE = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_TABSTOP, 115, 250, 190, 30, hwnd, HMENU(RE_ID), hInstanceApp, NULL);
        SendMessage(hRE, WM_SETFONT, (WPARAM)hFont, TRUE);

        // Label para RE
        hRELabel = CreateWindowEx(NULL,
            L"STATIC", L"RE", WS_CHILD | WS_VISIBLE, 70, 248, 45, 35, hwnd, HMENU(RE_LABEL_ID), hInstanceApp, NULL);
        SendMessage(hRELabel, WM_SETFONT, (WPARAM)hTitleFont, TRUE);

        // Caixa de texto para BETA
        hBETA = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_TABSTOP, 155, 300, 150, 30, hwnd, HMENU(BETA_ID), hInstanceApp, NULL);
        SendMessage(hBETA, WM_SETFONT, (WPARAM)hFont, TRUE);

        // Label para BETA
        hBETALabel = CreateWindowEx(NULL,
            L"STATIC", L"BETA", WS_CHILD | WS_VISIBLE, 70, 298, 80, 35, hwnd, HMENU(BETA_LABEL_ID), hInstanceApp, NULL);
        SendMessage(hBETALabel, WM_SETFONT, (WPARAM)hTitleFont, TRUE);

        // Caixa de texto para VCC
        hVCC = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_TABSTOP, 155, 350, 150, 30, hwnd, HMENU(VCC_ID), hInstanceApp, NULL);
        SendMessage(hVCC, WM_SETFONT, (WPARAM)hFont, TRUE);

        // Label para VCC
        hVCCLabel = CreateWindowEx(NULL,
            L"STATIC", L"VCC", WS_CHILD | WS_VISIBLE, 70, 348, 65, 35, hwnd, HMENU(VCC_LABEL_ID), hInstanceApp, NULL);
        SendMessage(hVCCLabel, WM_SETFONT, (WPARAM)hTitleFont, TRUE);
        
        // Botao para fazer os calculos
        hButton = CreateWindowEx(NULL, L"BUTTON", L"AVALIAR", WS_CHILD| WS_VISIBLE | BS_PUSHBUTTON , 70, 400, 238, 35, hwnd, HMENU(BUTTON_ID), hInstanceApp, NULL);
        SendMessage(hButton, WM_SETFONT, (WPARAM)hFont, TRUE);

        // Definicao do menu da aplicacao
        hMenu = CreateMenu();
        AppendMenu(hMenu, MF_STRING, MENU_ABOUT_ID, L"Sobre");
        AppendMenu(hMenu, MF_STRING, MENU_SRC_ID, L"Codigo Fonte");
        SetMenu(hwnd, hMenu);
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case MENU_ABOUT_ID:
            MessageBox(NULL, L"A Aplicacao Faz uma Analise CC em Circuitos com Polarizacao Por Divisor de Tensao, Tambem Quantifica e Regulariza os Valores ( 0.123 > 123m ) Automaticamente.", L"Sobre", MB_ICONINFORMATION | MB_OK);
            break;

        case MENU_SRC_ID:
            MessageBox(NULL, L"Codigo Fonte da Aplicacao Pode Ser Encontrado Aqui: https://github.com/Th1ago0/PDT_APP", L"Codigo Fonte da Aplicação", MB_ICONINFORMATION | MB_OK);
            break;

        case BUTTON_ID:
            // Apaga resultado anterior
            if (!isClean)
            {
                RECT Rect = { clientRect.left + 425, clientRect.top - 50, clientRect.right, 400 };
                InvalidateRect(hwnd, &Rect, TRUE);
                UpdateWindow(hwnd);
                isClean = true;
            }

            hdc = GetDC(hwnd);

            /*  DEFINICAO DAS VARIAVEIS DE COMPRIMENTO E VALORES  */

            // R1
            int r1Len = GetWindowTextLength(hR1);
            wchar_t* r1 = new wchar_t[r1Len + 1];
            GetWindowText(hR1, r1, r1Len + 1);

            // R2
            int r2Len = GetWindowTextLength(hR2);
            wchar_t* r2 = new wchar_t[r2Len + 1];
            GetWindowText(hR2, r2, r2Len + 1);

            // RC
            int rcLen = GetWindowTextLength(hRC);
            wchar_t* rc = new wchar_t[rcLen + 1];
            GetWindowText(hRC, rc, rcLen + 1);

            // RE
            int reLen = GetWindowTextLength(hRE);
            wchar_t* re = new wchar_t[reLen + 1];
            GetWindowText(hRE, re, reLen + 1);

            // BETA
            int betaLen = GetWindowTextLength(hBETA);
            wchar_t* beta = new wchar_t[betaLen + 1];
            GetWindowText(hBETA, beta, betaLen + 1);

            // VCC
            int vccLen = GetWindowTextLength(hVCC);
            wchar_t* vcc = new wchar_t[vccLen + 1];
            GetWindowText(hVCC, vcc, vccLen + 1);

            // Instancia de um objeto da classe PDT
            PDT pdt(
                quantify(r1),
                quantify(r2),
                quantify(rc),
                quantify(re),
                quantify(vcc),
                quantify(beta)
            );
            pdt.Start();


            // seleciona nova fonte no contexto do dispositivo
            SelectObject(hdc, hTitleFont);
            SetTextColor(hdc, RGB(0, 0, 0));
            // fundo do texto tranparente
            SetBkMode(hdc, TRANSPARENT);

            RECT strRect = { clientRect.left + 415, clientRect.top - 225, clientRect.right, 400 };
            wstringstream vccT, vbeT, vceT, vbbT, vcT, veT, r1T, r2T, reT, rcT, rthT, ieT, icT, ibT, method;

            // Exibicao do metodo utilizado
            method << pdt.getMethod();
            DrawText(hdc, method.str().c_str(), -1, &strRect, DT_SINGLELINE | DT_LEFT | DT_VCENTER);

            // seleciona nova fonte no contexto do dispositivo
            SelectObject(hdc, hOutputFont);

            /*   EXIBICAO DAS TENSOES   */
            strRect.top = clientRect.top - 50;
            vccT << L"VCC:  " << pdt.getVCC() << L"V";
            DrawText(hdc, vccT.str().c_str(), -1, &strRect, DT_SINGLELINE | DT_LEFT | DT_VCENTER);

            strRect.top = clientRect.top;
            vbbT << L"VBB:  " << pdt.getVBB() << L"V";
            DrawText(hdc, vbbT.str().c_str(), -1, &strRect, DT_SINGLELINE | DT_LEFT | DT_VCENTER);

            strRect.top = clientRect.top + 50;
            vbeT << L"VBE:  " << pdt.getVBE() << L"V";
            DrawText(hdc, vbeT.str().c_str(), -1, &strRect, DT_SINGLELINE | DT_LEFT | DT_VCENTER);

            SetTextColor(hdc, RGB(0, 180, 0));
            strRect.top = clientRect.top + 100;
            vceT << L"VCE:  " << pdt.getVCE() << L"V";
            DrawText(hdc, vceT.str().c_str(), -1, &strRect, DT_SINGLELINE | DT_LEFT | DT_VCENTER);

            SetTextColor(hdc, RGB(0, 0, 0));
            strRect.top = clientRect.top + 150;
            vcT << L"VC:  " << pdt.getVC() << L"V";
            DrawText(hdc, vcT.str().c_str(), -1, &strRect, DT_SINGLELINE | DT_LEFT | DT_VCENTER);

            strRect.top = clientRect.top + 200;
            veT << L"VE:  " << pdt.getVE() << L"V";
            DrawText(hdc, veT.str().c_str(), -1, &strRect, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
            /*   FIM DA EXIBICAO DAS TENSOES   */

            /*   EXIBICAO DAS CORRENTES   */
            strRect.top = clientRect.top - 50;
            strRect.left = clientRect.left + 575;
            ieT << L"IE:  " << pdt.getIE() << L"A";
            DrawText(hdc, ieT.str().c_str(), -1, &strRect, DT_SINGLELINE | DT_LEFT | DT_VCENTER);

            SetTextColor(hdc, RGB(0, 180, 0));
            strRect.top = clientRect.top;
            icT << L"IC:  " << pdt.getIC() << L"A";
            DrawText(hdc, icT.str().c_str(), -1, &strRect, DT_SINGLELINE | DT_LEFT | DT_VCENTER);

            SetTextColor(hdc, RGB(0, 0, 0));
            strRect.top = clientRect.top + 50;
            ibT << L"IB:  " << pdt.getIB() << L"A";
            DrawText(hdc, ibT.str().c_str(), -1, &strRect, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
            /*   FIM DA EXIBICAO DAS CORRENTES   */

            /*   EXIBICAO DAS RESISTENCIAS   */
            strRect.top = clientRect.top - 50;
            strRect.left = clientRect.left + 715;
            r1T << L"R1:  " << pdt.getR1() << L"\u03A9";
            DrawText(hdc, r1T.str().c_str(), -1, &strRect, DT_SINGLELINE | DT_LEFT | DT_VCENTER);

            strRect.top = clientRect.top;
            r2T << L"R2:  " << pdt.getR2() << L"\u03A9";
            DrawText(hdc, r2T.str().c_str(), -1, &strRect, DT_SINGLELINE | DT_LEFT | DT_VCENTER);

            strRect.top = clientRect.top + 50;
            rcT << L"RC:  " << pdt.getRC() << L"\u03A9";
            DrawText(hdc, rcT.str().c_str(), -1, &strRect, DT_SINGLELINE | DT_LEFT | DT_VCENTER);

            strRect.top = clientRect.top + 100;
            reT << L"RE:  " << pdt.getRE() << L"\u03A9";
            DrawText(hdc, reT.str().c_str(), -1, &strRect, DT_SINGLELINE | DT_LEFT | DT_VCENTER);

            strRect.top = clientRect.top + 150;
            rthT << L"RTH:  " << pdt.getRTH() << L"\u03A9";
            DrawText(hdc, rthT.str().c_str(), -1, &strRect, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
            /*   FIM DA EXIBICAO DAS RESISTENCIAS   */

            isClean = false;
            break;
        }
        break;
        
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        EndPaint(hwnd, &ps);
        break;
    
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

// Recebe um valor com seu multiplo (M, k, m, u, n) quantifica ele e retorna o resultado
double quantify(wstring args)
{
    double multiplier = 1.0;

    if (args.find(L"k") != wstring::npos || args.find(L"K") != wstring::npos)
    {
        replace(args.begin(), args.end(), 'k', ' ');
        multiplier = 1e3;
    }
    else if (args.find(L"m") != wstring::npos)
    {
        replace(args.begin(), args.end(), 'm', ' ');
        multiplier = 1e-3;
    }
    else if (args.find(L"u") != wstring::npos)
    {
        replace(args.begin(), args.end(), 'u', ' ');
        multiplier = 1e-6;
    }
    else if (args.find(L"n") != wstring::npos)
    {
        replace(args.begin(), args.end(), 'n', ' ');
        multiplier = 1e-9;
    }
    else if (args.find(L"M") != wstring::npos)
    {
        replace(args.begin(), args.end(), 'M', ' ');
        multiplier = 1e6;
    }

    double value = stod(args);
    return value * multiplier;
}
