/*******************************************************************************
 *
 *    Copyright (C) 2014-2018 Greg McGarragh <mcgarragh@atm.ox.ac.uk>
 *
 *    This source code is licensed under the GNU General Public License (GPL),
 *    Version 3.  See the file COPYING for more details.
 *
 ******************************************************************************/

#include "external.h"
#include "internal.h"
#include "misc_util.h"


/*******************************************************************************
 * Return non-zero if the current machine is Little-endian and zero if it is
 * Big-endian.
 ******************************************************************************/
int su_is_little_endian()
{
     ushort x = 1;

     return ((char *) &x)[0];
}



/*******************************************************************************
 * Round the argument to the nearest integer value with an upward rounding
 * direction.
 ******************************************************************************/
double su_rint(double x)
{
     return x >= 0. ? floor(x + .5) : ceil(x - .5);
}



/*******************************************************************************
 * Fill an array with a constant value.
 ******************************************************************************/
void su_init_array_uc(uchar *a, uint n, uchar x)
{
     uint i;

     for (i = 0; i < n; ++i)
          a[i] = x;
}



void su_init_array_us(ushort *a, uint n, ushort x)
{
     uint i;

     for (i = 0; i < n; ++i)
          a[i] = x;
}



void su_init_array_f(float *a, uint n, float x)
{
     uint i;

     for (i = 0; i < n; ++i)
          a[i] = x;
}



void su_init_array_d(double *a, uint n, double x)
{
     uint i;

     for (i = 0; i < n; ++i)
          a[i] = x;
}



/*******************************************************************************
 * Calculate and return the GSICS calibration offset from the values contained
 * in the Level 1.5 header file
 ******************************************************************************/
double su_get_ar_val(double ac, double bc, double g0)
{
     return (ac - (bc * g0));
}



/*******************************************************************************
 * Calculate and return the GSICS calibration slope from the values contained
 * in the Level 1.5 header file
 ******************************************************************************/
double su_get_br_val(double bc, double gs)
{
     return (bc / gs);
}



/*******************************************************************************
 * Compute calender date given Julian Day Number.
 *
 * Hatcher, D. A. 1984
 ******************************************************************************/
#ifdef IXMOD
#undef IXMOD
#endif
#define IXMOD(x, y) (((x % y) + y) % y)

#ifdef FXMOD
#undef FXMOD
#endif
#define FXMOD(x, y) (fmod((fmod(x, y) + y), y))


#define JUL_GREG 2299161

void su_jul_to_cal_date(long jul, int *y, int *m, int *d)
{
     if (jul >= JUL_GREG)
          jul += (long) (floor(floor((jul - 4479.5) / 36524.25) * 0.75 + 0.5) - 37.);

     *y = (int) floor(jul / 365.25) - 4712;
     *d = (int) floor(FXMOD((jul - 59.25), 365.25));
     *m = (int) FXMOD((floor((*d + 0.5) / 30.6) + 2), 12) + 1;
     *d = (int) floor(FXMOD((*d + 0.5), 30.6)) + 1;

     if (*y <= 0)
          --(*y);
}


/*******************************************************************************
 * Compute Julian Day Number given calender date.
 *
 * Hatcher, D. A. 1984
 ******************************************************************************/
#define CAL_GREG (15+31L*(10+12L*1582))

long su_cal_to_jul_day(int y, int m, int d)
{
     int yp;
     int mp;

     long jul;

     if (y == 0) {
          fprintf(stderr, "ERROR: There is no year zero\n");
          return -1;
     }

     yp = y;

     if (yp < 0)
          ++yp;
/*
     yp = yp - floor((12 - m) / 10.);
*/
     if (m < 3)
          --yp;

     mp = IXMOD((m - 3), 12);

     jul = (long) (floor(365.25*(yp+4712)) + floor(30.6*mp+0.5) + d + 59);

     if (d+31L*(m+12L*y) >= CAL_GREG)
          jul -= (long) (floor(floor((yp * 0.01) + 49.) * 0.75) - 38.);
/*
          jul -= (long) (floor(floor((yp * 0.01) +  1.) * 0.75) -  2.);
*/
     return jul;
}



/*******************************************************************************
 * Compute the annual solar distance factor given the day of year.
 *
 * Liou 2002, page 49
 ******************************************************************************/
double su_solar_distance_factor2(double jday)
{
     double t;

     t = (2. * PI * jday) / 365.;

     return 1.000110 +
            0.034221*cos(t)    + 0.001280*sin(t) +
            0.000719*cos(2.*t) + 0.000077*sin(2.*t);
}
