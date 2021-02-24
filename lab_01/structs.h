#ifndef STRUCTS_H
#define STRUCTS_H
#include <QPoint>
typedef struct
{
  int points[3];
  int points_num;
}polygone_t;
typedef struct
{
  polygone_t * polygones;
  int pol_num;
}polygones_t;
typedef struct
{
   int x, y;
} axis_t;
#endif // STRUCTS_H
