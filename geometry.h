#include "figures.h"
#include <math.h>
#include <vector>
#include "Vector3f.h"

double square_area(triangle_t triangle);

bool is_into_triangle_old_school(point_t v1, point_t v2, point_t v3, point_t p);

//The points must be sorted by decreasing order along the Y-axis
bool is_into_triangle(point_t v1, point_t v2, point_t v3, point_t p);

//The vertex must be sorted by decreasing order along the Y-axis
bool is_into_triangle(triangle_t triangle, point_t p);

std::vector<point_t> covering_square(point_t v1, point_t v2, point_t v3);

Vector3f get_barycentric_vector(triangle_t triangle, point_t p);

Vector3f getSurfaceNormal(triangle_t triangle_t);

triangle_t resize(triangle_t triangle, int width, int height);

triangle_t perspective(triangle_t triangle, Vector3f camera_pos);

Vector3f cross(Vector3f u, Vector3f v);

Vector3f vertex_normal(Vector3f v, Vector3f a, Vector3f b);

float scalar(Vector3f u, Vector3f v);
