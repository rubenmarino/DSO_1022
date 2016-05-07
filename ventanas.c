#include "main.h"

// Esta funcion ensancha la ventana manteniendo una "distancia" determinada respecto a la esquina inf. derecha de su padre.

void InverseStretch(HWND parent_hwnd, int identificador, long Xdist, long Ydist)
{
    RECT child_coord, parent_coord;
    HWND child_hwnd;
    long ancho, alto;

    child_hwnd = GetDlgItem(parent_hwnd, identificador);
    // Obtener las coord. del padre
    GetWindowRect(parent_hwnd, &parent_coord);
    // Obtener las coord. del hijo
    GetWindowRect(child_hwnd, &child_coord);
    // calcular
    ancho = parent_coord.right - child_coord.left - Xdist;
    alto = parent_coord.bottom - child_coord.top - Ydist;
    SetWindowPos(child_hwnd, 0, 0, 0, ancho, alto, SWP_NOMOVE | SWP_NOZORDER);
    glViewport(0, 0, ancho, alto);
}

// Esta funcion mueve la ventana respecto al borde derecho de su padre.

void Anclar_a_la_Derecha(HWND parent_hwnd, int id_inicial, int id_final, long *posiciones)
{
    RECT child_coord, parent_coord;
    HWND child_hwnd;
    int cont;

    // Obtener las coord. del padre
    GetWindowRect(parent_hwnd, &parent_coord);
    cont = 0;
    for (; id_inicial <= id_final; id_inicial++)
    {
        // Obtener las coord. del hijo
        child_hwnd = GetDlgItem(parent_hwnd, id_inicial);
        GetWindowRect(child_hwnd, &child_coord);
        // calcular posicion X del hijo
        child_coord.left = parent_coord.right - posiciones[cont];
        // convertir las coordenadas del hijo en coord. del padre
        ScreenToClient(parent_hwnd, (POINT*) & child_coord.left);
        // Mover
        SetWindowPos(child_hwnd, 0, child_coord.left, child_coord.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER /*| SWP_NOREDRAW*/);
        cont++;
    }
    RedrawWindow(parent_hwnd, NULL, NULL, /*RDW_FRAME |*/ RDW_INVALIDATE /*| RDW_ALLCHILDREN | RDW_ERASE*/);
}

// Calcula las posiciones de todas las ventanas, desde el borde derecho de padre,
// en el rango de ID especificado, y lo guarda en el vector posiciones.

void Calcular_Posiciones(HWND parent_hwnd, int id_inicial, int id_final, long *posiciones)
{
    RECT child_coord, parent_coord;
    HWND child_hwnd;
    int cont;

    // Obtener las coord. del padre
    GetWindowRect(parent_hwnd, &parent_coord);
    cont = 0;
    for (; id_inicial <= id_final; id_inicial++)
    {
        // Obtener las coord. del padre
        GetWindowRect(parent_hwnd, &parent_coord);
        // Obtener las coord. del hijo
        child_hwnd = GetDlgItem(parent_hwnd, id_inicial);
        GetWindowRect(child_hwnd, &child_coord);
        // calcular la posicion inicial del hijo
        posiciones[cont] = parent_coord.right - child_coord.left;
        cont++;
    }
}

void ProcesarComandos(HWND *hwnd, WPARAM *wParam)
{
    switch (LOWORD(*wParam))
    {
        // ****************** Botones **********************
    case ID_YT:
        osc_mode = ID_YT;
        return;
    case ID_XY:
        osc_mode = ID_XY;
        return;

    case ID_TRIGGER_AUTO:
        trigger.mode = 0;
        return;
    case ID_TRIGGER_NORMAL:
        trigger.mode = 1;
        return;
    case ID_TRIGGER_SIMPLE:
        trigger.mode = 2;
        return;

    case ID_TRIGGER_CH_1:
        trigger.source = 0;
        return;
    case ID_TRIGGER_CH_2:
        trigger.source = 1;
        return;

    case ID_FLANCO_DOWN:
        trigger.slope = 1;
        return;
    case ID_FLANCO_UP:
        trigger.slope = 0;
        return;

    case ID_CH1_ON:
        if (IsDlgButtonChecked(*hwnd, ID_CH1_ON) == BST_CHECKED)
        {
            EnableWindow(GetDlgItem(*hwnd, ID_CH1_AC), TRUE);
            EnableWindow(GetDlgItem(*hwnd, ID_CH1_DC), TRUE);
            EnableWindow(GetDlgItem(*hwnd, ID_CH1_X1), TRUE);
            EnableWindow(GetDlgItem(*hwnd, ID_CH1_X10), TRUE);
            EnableWindow(GetDlgItem(*hwnd, ID_CH1_X100), TRUE);
            EnableWindow(GetDlgItem(*hwnd, ID_COMBO_CH1), TRUE);
            EnableWindow(GetDlgItem(*hwnd, ID_CH1), TRUE);
            ch1.encendido = TRUE;
        }
        else
        {
            EnableWindow(GetDlgItem(*hwnd, ID_CH1_AC), FALSE);
            EnableWindow(GetDlgItem(*hwnd, ID_CH1_DC), FALSE);
            EnableWindow(GetDlgItem(*hwnd, ID_CH1_X1), FALSE);
            EnableWindow(GetDlgItem(*hwnd, ID_CH1_X10), FALSE);
            EnableWindow(GetDlgItem(*hwnd, ID_CH1_X100), FALSE);
            EnableWindow(GetDlgItem(*hwnd, ID_COMBO_CH1), FALSE);
            EnableWindow(GetDlgItem(*hwnd, ID_CH1), FALSE);
            ch1.encendido = FALSE;
        }
        return;

    case ID_CH1_AC:
        ch1.modo = AC;
        return;
    case ID_CH1_DC:
        ch1.modo = DC;
        return;
    case ID_CH1_X1:
        ch1.atenuacion = 1;
        return;
    case ID_CH1_X10:
        ch1.atenuacion = 2;
        return;
    case ID_CH1_X100:
        ch1.atenuacion = 3;
        return;

    case ID_CH2_ON:
        if (IsDlgButtonChecked(*hwnd, ID_CH2_ON) == BST_CHECKED)
        {
            EnableWindow(GetDlgItem(*hwnd, ID_CH2_AC), TRUE);
            EnableWindow(GetDlgItem(*hwnd, ID_CH2_DC), TRUE);
            EnableWindow(GetDlgItem(*hwnd, ID_CH2_X1), TRUE);
            EnableWindow(GetDlgItem(*hwnd, ID_CH2_X10), TRUE);
            EnableWindow(GetDlgItem(*hwnd, ID_CH2_X100), TRUE);
            EnableWindow(GetDlgItem(*hwnd, ID_COMBO_CH2), TRUE);
            EnableWindow(GetDlgItem(*hwnd, ID_CH2), TRUE);
            ch2.encendido = TRUE;
        }
        else
        {
            EnableWindow(GetDlgItem(*hwnd, ID_CH2_AC), FALSE);
            EnableWindow(GetDlgItem(*hwnd, ID_CH2_DC), FALSE);
            EnableWindow(GetDlgItem(*hwnd, ID_CH2_X1), FALSE);
            EnableWindow(GetDlgItem(*hwnd, ID_CH2_X10), FALSE);
            EnableWindow(GetDlgItem(*hwnd, ID_CH2_X100), FALSE);
            EnableWindow(GetDlgItem(*hwnd, ID_COMBO_CH2), FALSE);
            EnableWindow(GetDlgItem(*hwnd, ID_CH2), FALSE);
            ch2.encendido = FALSE;
        }
        return;

    case ID_CH2_AC:
        ch2.modo = AC;
        return;
    case ID_CH2_DC:
        ch2.modo = DC;
        return;
    case ID_CH2_X1:
        ch2.atenuacion = 1;
        return;
    case ID_CH2_X10:
        ch2.atenuacion = 2;
        return;
    case ID_CH2_X100:
        ch2.atenuacion = 3;
        return;

    case ID_COMBO_H1:
        if (HIWORD(*wParam) == CBN_SELCHANGE)
        {
            time_div = SendDlgItemMessage(*hwnd, ID_COMBO_H1, CB_GETCURSEL, 0, 0);
            SendDlgItemMessage(*hwnd, ID_TIMEBASE, TBM_SETPOS, TRUE, time_div);
        }
        return;

    case ID_COMBO_CH1:
        if (HIWORD(*wParam) == CBN_SELCHANGE)
        {
            ch1.volt_div = SendDlgItemMessage(*hwnd, ID_COMBO_CH1, CB_GETCURSEL, 0, 0);
            SendDlgItemMessage(*hwnd, ID_CH1, TBM_SETPOS, TRUE, ch1.volt_div);
        }
        return;

    case ID_COMBO_CH2:
        if (HIWORD(*wParam) == CBN_SELCHANGE)
        {
            ch2.volt_div = SendDlgItemMessage(*hwnd, ID_COMBO_CH2, CB_GETCURSEL, 0, 0);
            SendDlgItemMessage(*hwnd, ID_CH2, TBM_SETPOS, TRUE, ch2.volt_div);
        }
        return;


        // ******************** Menu ***********************
    case IDM_PLAY:
        if (OSC_Flag == OSC_STOP)
        {
            OSC_Flag = OSC_RUN;
            CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) PlayOsc, 0, 0, NULL);
        }
        return;

    case IDM_STOP:
        OSC_Flag = OSC_STOP;
        return;
    }
}

void inicializar(HWND *hwnd)
{
    unsigned short contador;
         
    char Tiempo[TIEMPOS][8] = {TEXT("1 ns"), TEXT("2 ns"), TEXT("5 ns"), TEXT("10 ns"), TEXT("20 ns"), TEXT("50 ns"), TEXT("0.1 us"), TEXT("0.2 us"),
    TEXT("0.5 us"), TEXT("1 us"), TEXT("2 us"), TEXT("5 us"), TEXT("10 us"), TEXT("20 us"), TEXT("50 us"), TEXT("0.1 ms"), TEXT("0.2 ms"),
    TEXT("0.5 ms"), TEXT("1 ms"), TEXT("2 ms"), TEXT("5 ms"), TEXT("10 ms"), TEXT("20 ms"), TEXT("50 ms"), TEXT("0.1 s"), TEXT("0.2 s"),
    TEXT("0.5 s"), TEXT("1 s"), TEXT("2 s"), TEXT("5 s")};
    
    char Voltajes[VOLTAJES][8] = {TEXT("20 mV"), TEXT("50 mV"), TEXT("100 mV"), TEXT("200 mV"), TEXT("500 mV"), TEXT("1 V"), TEXT("2 V"), TEXT("5 V")};

    // Reservar memoria para los buffers
    CH1Data = (short*) malloc (1100000 * sizeof(short));
    CH2Data = (short*) malloc (1100000 * sizeof(short));
    
    // Llenar la lista COMBO_BOX
    for (contador = 0; contador < TIEMPOS; contador++)
        SendDlgItemMessage(*hwnd, ID_COMBO_H1, CB_ADDSTRING, 0, (LPARAM) Tiempo[contador]);

    // Setear trackbar time_base
    SendDlgItemMessage(*hwnd, ID_TIMEBASE, TBM_SETRANGE, TRUE, MAKELONG(0, TIEMPOS - 1)); // rango
    SendDlgItemMessage(*hwnd, ID_TIMEBASE, TBM_SETTICFREQ, 1, 0); // una marca x posicion

    // Llenar la lista COMBO_BOX
    for (contador = 0; contador < VOLTAJES; contador++)
    {
        SendDlgItemMessage(*hwnd, ID_COMBO_CH1, CB_ADDSTRING, 0, (LPARAM) Voltajes[contador]);
        SendDlgItemMessage(*hwnd, ID_COMBO_CH2, CB_ADDSTRING, 0, (LPARAM) Voltajes[contador]);
    }

    // Setear trackbar volt_ch1
    SendDlgItemMessage(*hwnd, ID_CH1, TBM_SETRANGE, TRUE, MAKELONG(0, VOLTAJES - 1)); // rango
    SendDlgItemMessage(*hwnd, ID_CH1, TBM_SETTICFREQ, 1, 0); // una marca x posicion

    // Setear trackbar volt_ch1
    SendDlgItemMessage(*hwnd, ID_CH2, TBM_SETRANGE, TRUE, MAKELONG(0, VOLTAJES - 1)); // rango
    SendDlgItemMessage(*hwnd, ID_CH2, TBM_SETTICFREQ, 1, 0); // una marca x posicion

    // Valores por defecto
    
    SendDlgItemMessage(*hwnd, ID_COMBO_H1, CB_SETCURSEL, 15, 0);
    time_div = 14;
    SendDlgItemMessage(*hwnd, ID_COMBO_CH1, CB_SETCURSEL, 7, 0);
    SendDlgItemMessage(*hwnd, ID_COMBO_CH2, CB_SETCURSEL, 7, 0);
    ch1.volt_div = 7;
    ch2.volt_div = 7;
    
}

