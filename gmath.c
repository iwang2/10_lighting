#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gmath.h"
#include "matrix.h"
#include "ml6.h"

//lighting functions
color get_lighting( double *normal, double *view, color alight, double light[2][3], double *areflect, double *dreflect, double *sreflect) {
  color i, a, d, s;
  normalize(normal);
  normalize(light[LOCATION]);
  normalize(view);
  a = calculate_ambient(alight, areflect);
  d = calculate_diffuse(light, dreflect, normal);
  s = calculate_specular(light, sreflect, view, normal);
  i.red = a.red + d.red + s.red;
  i.green = a.green + d.green + s.green;
  i.blue = a.blue + d.blue + s.blue;
  limit_color(&i);
  return i;
}

color calculate_ambient(color alight, double *areflect ) {
  color a;
  a.red = alight.red * areflect[RED];
  a.blue = alight.blue * areflect[BLUE];
  a.green = alight.green * areflect[GREEN];
  return a;
}

color calculate_diffuse(double light[2][3], double *dreflect, double *normal ) {
  color d;
  double cos = dot_product(normal, light[LOCATION]);
  d.red = light[COLOR][RED] * dreflect[RED] * cos;
  d.blue = light[COLOR][BLUE] * dreflect[BLUE] * cos;
  d.green = light[COLOR][GREEN] * dreflect[GREEN] * cos;
  return d;
}

color calculate_specular(double light[2][3], double *sreflect, double *view, double *normal ) {
  color s;
  double tcos = 2 * dot_product(normal, light[LOCATION]);
  double r[3];
  r[0] = tcos * normal[0] - light[LOCATION][0];
  r[1] = tcos * normal[1] - light[LOCATION][1];
  r[2] = tcos * normal[2] - light[LOCATION][2];
  s.red = light[COLOR][RED] * sreflect[RED] * dot_product(r, view);
  s.green = light[COLOR][GREEN] * sreflect[GREEN] * dot_product(r, view);
  s.blue = light[COLOR][BLUE] * sreflect[BLUE] * dot_product(r, view);
  return s;
}


//limit each component of c to a max of 255
void limit_color( color * c ) {
  if(c->red > 255) c->red = 255;
  if(c->blue > 255) c->blue = 255;
  if(c->green > 255) c->green = 255;
}

//vector functions
//normalize vetor, should modify the parameter
void normalize( double *vector ) {
  double magnitude = sqrt( pow(vector[0],2) + pow(vector[1],2) + pow(vector[2], 2) );
  vector[0] /= magnitude;
  vector[1] /= magnitude;
  vector[2] /= magnitude;
}

//Return the dot porduct of a . b
double dot_product( double *a, double *b ) {
  return *a * *b + *(a+1) * *(b+1) + *(a+2) * *(b+2);
}

double *calculate_normal(struct matrix *polygons, int i) {

  double A[3];
  double B[3];
  double *N = (double *)malloc(3 * sizeof(double));

  A[0] = polygons->m[0][i+1] - polygons->m[0][i];
  A[1] = polygons->m[1][i+1] - polygons->m[1][i];
  A[2] = polygons->m[2][i+1] - polygons->m[2][i];

  B[0] = polygons->m[0][i+2] - polygons->m[0][i];
  B[1] = polygons->m[1][i+2] - polygons->m[1][i];
  B[2] = polygons->m[2][i+2] - polygons->m[2][i];

  N[0] = A[1] * B[2] - A[2] * B[1];
  N[1] = A[2] * B[0] - A[0] * B[2];
  N[2] = A[0] * B[1] - A[1] * B[0];

  return N;
}
