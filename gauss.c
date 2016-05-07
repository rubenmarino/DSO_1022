#include "main.h"

// source channel, target channel, width, height, radius

void gaussBlur_4(byte *scl, byte *tcl, u_int w, u_int h, float r)
{
    int bxs[3];

    boxesForGauss(bxs, r, 3);
    boxBlur_4(scl, tcl, w, h, (bxs[0] - 1) / 2);
    boxBlur_4(tcl, scl, w, h, (bxs[1] - 1) / 2);
    boxBlur_4(scl, tcl, w, h, (bxs[2] - 1) / 2);
}

void boxBlur_4(byte *scl, byte *tcl, u_int w, u_int h, u_int r)
{
    u_int i;
    u_int length;
    length = w * h;
    for (i = 0; i < length; i++) tcl[i] = scl[i];
    boxBlurH_4(tcl, scl, w, h, r);
    boxBlurT_4(scl, tcl, w, h, r);
}

void boxBlurH_4(byte *scl, byte *tcl, u_int w, u_int h, u_int r)
{
    float iarr, val;
    u_int i, j;
    int ti, li, ri, fv, lv;

    iarr = 1 / (float)(2*r + 1);
    for (i = 0; i < h; i++)
    {
        ti = i*w;
        li = ti;
        ri = ti + r;
        fv = scl[ti];
        lv = scl[ti + w - 1];
        val = (r + 1) * fv;
        for (j = 0; j < r; j++)
            val += scl[ti + j];
        for (j = 0; j <= r; j++)
        {
            val += scl[ri++] - fv;
            tcl[ti++] = roundf(val * iarr);
        }
        for (j = r + 1; j < w - r; j++)
        {
            val += scl[ri++] - scl[li++];
            tcl[ti++] = roundf(val * iarr);
        }
        for (j = w - r; j < w; j++)
        {
            val += lv - scl[li++];
            tcl[ti++] = roundf(val * iarr);
        }
    }
}

void boxBlurT_4(byte *scl, byte *tcl, u_int w, u_int h, u_int r)
{
    float iarr, val;
    int ti, li, ri, i, j, fv, lv;

    iarr = 1 / (float)(r + r + 1);
    for (i = 0; i < w; i++)
    {
        ti = i;
        li = ti;
        ri = ti + r*w;
        fv = scl[ti];
        lv = scl[ti + w * (h - 1)];
        val = (r + 1) * fv;
        for (j = 0; j < r; j++) 
            val += scl[ti + j * w];
        for (j = 0; j <= r; j++)
        {
            val += scl[ri] - fv;
            tcl[ti] = roundf(val * iarr);
            ri += w;
            ti += w;
        }
        for (j = r + 1; j < h - r; j++)
        {
            val += scl[ri] - scl[li];
            tcl[ti] = roundf(val * iarr);
            li += w;
            ri += w;
            ti += w;
        }
        for (j = h - r; j < h; j++)
        {
            val += lv - scl[li];
            tcl[ti] = roundf(val * iarr);
            li += w;
            ti += w;
        }
    }
}

void boxesForGauss(int *sizes, float sigma, u_int n) // standard deviation, number of boxes

{
    int i, wl;
    float wIdeal, mIdeal, m;

    if (n > 20) n = 20;

    wIdeal = sqrtf((float) (12 * sigma * sigma / n) + 1); // Ideal averaging filter width 

    wl = floorf(wIdeal);
    if (wl % 2 == 0) wl--;

    mIdeal = (float) (12 * sigma * sigma - n * wl * wl - 4 * n * wl - 3 * n) / (float) (-4 * wl - 4);
    m = roundf(mIdeal);
    //  sigmaActual = Math.sqrt( (m*wl*wl + (n-m)*wu*wu - n)/12 );

    for (i = 0; i < n; i++)
    {
        if (i < m)
            sizes[i] = wl;
        else
            sizes[i] = wl + 2;
    }
}
