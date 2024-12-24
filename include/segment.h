#ifndef SEGMENT_H
#define SEGMENT_H

#include "point.h"

/**
 * @struct segment
 * @brief Represents a line segment in 2D space.
 * 
 * This structure defines a line segment in a 2-dimensional space with start and end points.
 * 
 * @param start The start point of the line segment.
 * @param end The end point of the line segment.
 */
typedef struct segment {
    point start, end;
} segment;

#endif // SEGMENT_H