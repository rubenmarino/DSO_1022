#include "main.h"

LRESULT CALLBACK DialogoPrincipal(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {
    case WM_HSCROLL:
        if (GetDlgCtrlID((HWND) lParam) == ID_TIMEBASE)
        {
            time_div = SendDlgItemMessage(hwnd, ID_TIMEBASE, TBM_GETPOS, 0, 0);
            SendDlgItemMessage(hwnd, ID_COMBO_H1, CB_SETCURSEL, time_div, 0);
        }
        return 0;

    case WM_VSCROLL:
        if (GetDlgCtrlID((HWND) lParam) == ID_CH1)
        {
            ch1.volt_div = SendDlgItemMessage(hwnd, ID_CH1, TBM_GETPOS, 0, 0);
            SendDlgItemMessage(hwnd, ID_COMBO_CH1, CB_SETCURSEL, ch1.volt_div, 0);
        }
        if (GetDlgCtrlID((HWND) lParam) == ID_CH2)
        {
            ch2.volt_div = SendDlgItemMessage(hwnd, ID_CH2, TBM_GETPOS, 0, 0);
            SendDlgItemMessage(hwnd, ID_COMBO_CH2, CB_SETCURSEL, ch2.volt_div, 0);
        }
        return 0;

    case WM_COMMAND:
        ProcesarComandos(&hwnd, &wParam);
        return 0;

    case WM_INITDIALOG:
        inicializar(&hwnd);
        OglFlag = OGL_RUN;
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) InitGL, hwnd, 0, NULL);

        // Almacenar las posiciones de cada control hijo.
        Calcular_Posiciones(hwnd, ID_GROUP_1, ID_CH2_X100, pos_ini);
        return 0;

    case WM_SIZE:
        Anclar_a_la_Derecha(hwnd, ID_GROUP_1, ID_CH2_X100, pos_ini);
        OglFlag = OGL_DEFORMAR;
        return 0;

    case WM_PAINT:
        return 0;

    case WM_CLOSE:
        OSC_Flag = OSC_STOP;
        OglFlag = OGL_STOP;
        Sleep(500); // esperar a que se cierren demas tareas.
        EndDialog(hwnd, WM_CLOSE);
        DestroyWindow(hwnd);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    HWND hwnd; /* This is the handle for our window */
    MSG messages; /* Here messages to the application is saved */

    hwnd = CreateWindowEx(0, /* Extended possibilites for variation */
                          "Base Window", /* Classname */
                          "Base Window", /* Title Text */
                          WS_OVERLAPPEDWINDOW, /* defaultwindow */
                          CW_USEDEFAULT, /* Windows decides the position */
                          CW_USEDEFAULT, /* where the window end up on the screen */
                          800, /* The programs width */
                          600, /* and height in pixels */
                          HWND_DESKTOP, /* The window is a childwindow to desktop */
                          NULL, /* No menu */
                          hInstance, /* Program Instance handler */
                          NULL /* No Window Creation data */
                          );

    /* Make the window visible on the screen */
    CreateDialog(NULL, MAKEINTRESOURCE(IDD_DIALOGO_PRINCIPAL), hwnd, (DLGPROC) DialogoPrincipal);

    /* Run the nessageloop. It will run until GetMessage( ) returns 0 */
    while (GetMessage(&messages, NULL, 0, 0))
    {
        // Send message to WindowProcedure
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }

    /* The program returvalue is 0 - The value that PostQuitMessage( ) gave */
    return messages.wParam;
}

