#ifndef MAINHEADER
#define MAINHEADER

#include <windows.h>
#include <commctrl.h>
#include <math.h>
#include <stdio.h>
#include <gl/glew.h>
//#include <gl/gl.h>
#include "resource.h"
#include "lib/HTmarch.h"

/*****************  Flags  ***********************/
#define OGL_STOP 0
#define OGL_RUN 1
#define OGL_DEFORMAR 2

#define OSC_STOP 0
#define OSC_RUN 1

/*****************  Constantes  ***********************/
#define RAW_TO_VOLT(IN, VDIV) ((float)IN * VDIV / 32)

#define TIEMPOS 30
#define VOLTAJES 8
#define DEVICEINDEX 0
#define AC 0
#define DC 1

/*************  Variables Globales  ******************/
int OglFlag;				// Flag usado por la tarea de dibujado.
int OSC_Flag;
long pos_ini[50];			// Guarda todas las posiciones de los controles.

short *CH1Data, *CH2Data;               // Datos muestreados
unsigned long gPoint;
short nCal[32];				// Datos de calibración del osc.

short osc_mode;				// Modo de funcionamiento del osciloscopio.
int time_div;

byte *sourceBitmap, *targetBitmap;

// buffer parece ser la max. cant. de muestras que entrega el osc., y screen es la cantidad de muestras en 10 divisiones.
// los ultimos 2 casilleros (2s y 5s) se deben formar con 2 o mas escaneos del osciloscopio
static ULONG buffer_len[32] = {1016,1016,1016,1016,1016,1016,1016,1016,1016,1016,1016,130048,130048,130048,130048,130048,130048,130048,130048,130048,523264,523264,523264,1047552,1047552,1047552,1047552,1047552,1047552,1047552};
static ULONG screen_len[32] = {960,960,960,960,960,960,960,960,960,960,960,800,800,800,500,1000,2000,5000,10000,20000,50000,100000,200000,500000,1000000,1000000,1000000,1000000,2000000,5000000};

static float volt_div[8] = {0.02, 0.05, 0.1, 0.2, 0.5, 1, 2, 5};

struct
{
	unsigned char encendido;
	unsigned char modo;
	unsigned char atenuacion;
	int volt_div;
} ch1, ch2;

struct
{
	short mode;
	short source;
	short level;
	short slope;
} trigger;

/*************  Funciones OpenGL *********************/
DWORD WINAPI InitGL(HWND);		// Tarea que dibuja la pantalla constantemente
int MySetPixelFormat(HDC hdc);
void RenderWave(void);
void Rectangulo (GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat grosor, GLfloat rec_brillo);
void LineGlow (GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);

/*************  Funciones del Osciloscopio ***********/
void ProcesarComandos(HWND *hwnd, WPARAM *wParam);
void PlayOsc(void);     // Tarea que lee los datos del osciloscopio

/*************  Funciones de Ventanas ****/
void inicializar (HWND *hwnd);
void InverseStretch(HWND parent_hwnd, int identificador, long Xdist, long Ydist);
void Anclar_a_la_Derecha(HWND parent_hwnd, int id_inicial, int id_final, long *posiciones);
void Calcular_Posiciones (HWND parent_hwnd, int id_inicial, int id_final, long *posiciones);

/*************** Funciones Blur gauss ************************/
void gaussBlur_4(byte *scl, byte *tcl, u_int w, u_int h, float r);
void boxBlur_4(byte *scl, byte *tcl, u_int w, u_int h, u_int r);
void boxBlurH_4 (byte *scl, byte *tcl, u_int w, u_int h, u_int r);
void boxBlurT_4 (byte *scl, byte *tcl, u_int w, u_int h, u_int r);
void boxesForGauss(int *sizes, float sigma, u_int n);

#endif
