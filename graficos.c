#include "main.h"
#define BLUR_TEXT_TAM 128
#define LUMINOSIDAD 200

DWORD WINAPI InitGL(HWND hDlg) // Tarea de dibujado
{
    HDC hDC;
    HGLRC hglrc;

    hDC = GetDC(GetDlgItem(hDlg, ID_GRAFICO));

    PIXELFORMATDESCRIPTOR pfd = {
        sizeof (PIXELFORMATDESCRIPTOR), // size of this pfd
        1, // version number
        PFD_DRAW_TO_WINDOW | // support window
        PFD_SUPPORT_OPENGL, // support OpenGL
        PFD_TYPE_RGBA, // RGBA type
        32, // 24-bit color depth
        0, 0, 0, 0, 0, 0, // color bits ignored
        0, // no alpha buffer
        0, // shift bit ignored
        0, // no accumulation buffer
        0, 0, 0, 0, // accum bits ignored
        32, // 32-bit z-buffer
        0, // no stencil buffer
        0, // no auxiliary buffer
        PFD_MAIN_PLANE, // main layer
        0, // reserved
        0, 0, 0 // layer masks ignored
    };

    // get the device context's best, available pixel format match
    // make that match the device context's current pixel format
    SetPixelFormat(hDC, ChoosePixelFormat(hDC, &pfd), &pfd);

    hglrc = wglCreateContext(hDC);
    // Select first Window as the rendering context and draw to it
    wglMakeCurrent(hDC, hglrc);

    sourceBitmap = (byte*) malloc(BLUR_TEXT_TAM * BLUR_TEXT_TAM);
    targetBitmap = (byte*) malloc(BLUR_TEXT_TAM * BLUR_TEXT_TAM);

    // ******  inicializar glew *******
    //glewExperimental = GL_TRUE;
    glewInit();

    // Configurar Blending y antialiasing
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST); // Modos de Antialiasing (mejor calidad)
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    // Activar Blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glShadeModel(GL_FLAT);

    // Color de fondo de pantalla
    glClearColor(0, 0.12, 0, 0);

    // Crear una textura vacia para el blur
    GLuint blur_textura;
    glGenTextures(1, &blur_textura);
    glBindTexture(GL_TEXTURE_2D, blur_textura); //Activar textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BLUR_TEXT_TAM, BLUR_TEXT_TAM, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, 0);

    // crear un framebuffer object
    GLuint mifbo;
    glGenFramebuffers(1, &mifbo);

    // enlazar la textura al color del FBO
    glBindFramebuffer(GL_FRAMEBUFFER, mifbo); //Activar framebuffer de usuario
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blur_textura, 0);

    while (1) // loop de renderizado
    {
        switch (OglFlag)
        {
        case (OGL_DEFORMAR):
            InverseStretch(hDlg, ID_GRAFICO, 420, BLUR_TEXT_TAM);
            OglFlag = OGL_RUN;
            break;

        case (OGL_STOP):
            wglMakeCurrent(NULL, NULL);
            ReleaseDC(hDlg, hDC);
            wglDeleteContext(hglrc);
            free(sourceBitmap);
            free(targetBitmap);
            return TRUE;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, mifbo); //Activar framebuffer de usuario
        RenderWave();

        Sleep(10); // Throttle the loop. If we don't put a sleep here, the thread will use 100% cpu
    }
}

void RenderWave(void) // Renderizar forma de onda almacenada en los buffers
{
    GLfloat xx, yy;
    GLint viewport_size[4];
    u_int cont;

    // obtener el tamaño de la ventana
    glGetIntegerv(GL_VIEWPORT, viewport_size);
    glViewport(0, 0, BLUR_TEXT_TAM, BLUR_TEXT_TAM);

    // borra la pantalla
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_LINE_SMOOTH); // activar Antialiasing
    glLineWidth(1.0f);

    glBegin(GL_LINE_STRIP);
    for (cont = 0; cont < screen_len[time_div]; cont++)
    {
        xx = ((GLfloat) 2 * cont / screen_len[time_div]) - 1;
        yy = (GLfloat) CH1Data[cont] / 32768;
        glColor3f(1, 1, 1);
        glVertex2f(xx, yy);
    }
    glEnd();

    glBindFramebuffer(GL_FRAMEBUFFER, 0); //Activar framebuffer a la pantalla
    glViewport(0, 0, viewport_size[2], viewport_size[3]); //Volver el viewport a su tamaño orig.

    glGetTexImage(GL_TEXTURE_2D, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, sourceBitmap);
    gaussBlur_4(sourceBitmap, targetBitmap, BLUR_TEXT_TAM, BLUR_TEXT_TAM, 1.5f);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BLUR_TEXT_TAM, BLUR_TEXT_TAM, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, targetBitmap);

    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_LINE_SMOOTH); // Activar Antialiasing
    glLineWidth(1.5f);

    glBegin(GL_LINE_STRIP);
    for (cont = 0; cont < screen_len[time_div]; cont++)
    {
        xx = ((GLfloat) 2 * cont / screen_len[time_div]) - 1;
        yy = (GLfloat) CH1Data[cont] / 32768;
        glColor4f(1, 1, 1, 0.1f);
        glVertex2f(xx, yy);
    }
    glEnd();
    /*
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_POLYGON);
    glColor3f(1, 1, 1);
    glTexCoord2i(0, 0); //Asignamos las coordenadas de textura del siguiente vértice.
    glVertex2f(-1, -1);
    glTexCoord2i(0, 1);
    glVertex2f(-1, 1);
    glTexCoord2i(1, 1);
    glVertex2f(1, 1);
    glTexCoord2i(1, 0);
    glVertex2f(1, -1);
    glEnd();
    glDisable(GL_TEXTURE_2D);*/

    glFinish();
}


