#include "main.h"

void PlayOsc(void)
{
    unsigned __int32 sample;
      while (OSC_Flag != OSC_STOP)
      {
          float coso = 0;
          for (sample = 0; sample < buffer_len[time_div]; sample++)
          {
              coso = coso + 0.000001f;
              CH1Data[sample] = sinf ((float)sample * coso ) * 100;
          }
          Sleep(100);
      }


    short lectura;

    if (dsoOpenDevice(DEVICEINDEX) == 0)
    {
        MessageBox(NULL, "No fue posible conectar con el osciloscopio. Verifique la conexion", "Osciloscopio no detectado", MB_ICONWARNING | MB_OK);
        return;
    }


    dsoGetCalLevel(DEVICEINDEX, nCal, 32); // leer datos de calibracion.
    //dsoSetVoltDIV(DEVICEINDEX, 0, ch1.volt_div);
    //dsoSetVoltDIV(DEVICEINDEX, 1, ch2.volt_div);
    //dsoSetTimeDIV(DEVICEINDEX, time_div);

    gPoint = 0;
    trigger.level = 10;

    while (OSC_Flag != OSC_STOP)
    {

        lectura = dsoReadHardData(DEVICEINDEX, CH1Data, CH2Data, buffer_len[time_div], nCal, ch1.volt_div, ch2.volt_div, trigger.mode,
                                  trigger.source, trigger.level, trigger.slope, time_div, 0, screen_len[time_div], &gPoint, 0);

        if (lectura <= 0)
        {
            MessageBox(NULL, "Error al leer los datos", "Error de lectura", MB_ICONWARNING | MB_OK);
            return;
        }

    }
    return;
}
