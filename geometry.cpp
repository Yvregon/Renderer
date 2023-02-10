#include "geometry.h"
#include <iostream>

double square_area(triangle_t triangle){

    point_t p0 = triangle.first;
    point_t p1 = triangle.second;
    point_t p2 = triangle.third;
    
    return (p1.x - p0.x) * (p2.y - p1.y) - (p2.x - p1.x) * (p1.y - p0.y);
}

bool is_into_triangle_old_school(point_t v1, point_t v2, point_t v3, point_t p){

    bool check_v1_v2 = ((v1.x - p.x) * (v2.y - p.y)) - ((v1.y - p.y) * (v2.x - p.x)) <= 0;
    bool check_v2_v3 = ((v2.x - p.x) * (v3.y - p.y)) - ((v2.y - p.y) * (v3.x - p.x)) <= 0;
    bool check_v3_v1 = ((v3.x - p.x) * (v1.y - p.y)) - ((v3.y - p.y) * (v1.x - p.x)) <= 0;

    return check_v1_v2 && check_v2_v3 && check_v3_v1;
}


bool is_into_triangle(point_t v1, point_t v2, point_t v3, point_t p){

    triangle_t PBC = {p, v2, v3};
    triangle_t PCA = {p, v3, v1};
    triangle_t PAB = {p, v1, v2};
    triangle_t ABC = {v1, v2, v3};

    float sqa_ABC = square_area(ABC);

    if(sqa_ABC != 0){
        double a = square_area(PBC) / sqa_ABC;
        double b = square_area(PCA) / sqa_ABC;
        double c = square_area(PAB) / sqa_ABC;

        return a >= 0 && b >= 0 && c >= 0;
        
    }else{

        return false;
    }
}

bool is_into_triangle(triangle_t triangle, point_t p){
    point_t v1 = triangle.first;
    point_t v2 = triangle.second;
    point_t v3 = triangle.third;

    return is_into_triangle(v1, v2, v3, p);

    return false;
}

std::vector<point_t> covering_square(point_t v1, point_t v2, point_t v3){
    float max_x = std::max(v1.x, std::max(v2.x, v3.x));
    float max_y = std::max(v1.y, std::max(v2.y, v3.y));
    float min_x = std::min(v1.x, std::min(v2.x, v3.x));
    float min_y = std::min(v1.y, std::min(v2.y, v3.y));

    point_t max = {max_x, max_y};
    point_t min = {min_x, min_y};
    std::vector result = {min, max};

    return result;
}

Vector3f get_barycentric_vector(triangle_t triangle, point_t p){
    point_t v1 = triangle.first;
    point_t v2 = triangle.second;
    point_t v3 = triangle.third;

    triangle_t PBC = {p, v2, v3};
    triangle_t PCA = {p, v3, v1};
    triangle_t PAB = {p, v1, v2};
    triangle_t ABC = {v1, v2, v3};

    double a = square_area(PBC) / square_area(ABC);
    double b = square_area(PCA) / square_area(ABC);
    double c = square_area(PAB) / square_area(ABC);

    return Vector3f(a, b, c);
}

Vector3f getSurfaceNormal(triangle_t triangle){

    point_t A = triangle.first;
    point_t B = triangle.second;
    point_t C = triangle.third;

    float x = (B.z - A.z)*(C.y - A.y) -  (C.z - A.z)*(B.y - A.y);
    float y = (B.x - A.x)*(C.z - A.z) -  (C.x - A.x)*(B.z - A.z);
    float z = (B.x - A.x)*(C.y - A.y) -  (C.x - A.x)*(B.y - A.y);

    Vector3f normal = Vector3f(x, y, z);
    normal.normalize();

	return normal;
}

triangle_t resize(triangle_t triangle, int width, int height){

    triangle_t triangle_resized;

    triangle_resized.first.x = (triangle.first.x + 1) * width/2;
    triangle_resized.second.x = (triangle.second.x + 1) * width/2;
    triangle_resized.third.x = (triangle.third.x + 1) * width/2;

    triangle_resized.first.y = (triangle.first.y + 1) * height/2;
    triangle_resized.second.y = (triangle.second.y + 1) * height/2;
    triangle_resized.third.y = (triangle.third.y + 1) * height/2;

    triangle_resized.first.z = (triangle.first.z + 1) * height/2;
    triangle_resized.second.z = (triangle.second.z + 1) * height/2;
    triangle_resized.third.z = (triangle.third.z + 1) * height/2;

    return triangle_resized;
}

float get_divisor(float z, float c){

    float divisor = (1-z/c);

    divisor = divisor == 0.0f ? 1.0f : divisor;
    
    return divisor;
}

point_t perspective_aux(point_t point, Vector3f c){

    return point_t {point.x / get_divisor(point.z, c.getZ()), point.y / get_divisor(point.z, c.getZ()), point.z / get_divisor(point.z, c.getZ())};
}

triangle_t perspective(triangle_t triangle, Vector3f camera_pos){

    triangle_t triangle_resized;

    triangle_resized.first = perspective_aux(triangle.first, camera_pos);
    triangle_resized.second = perspective_aux(triangle.second, camera_pos);
    triangle_resized.third = perspective_aux(triangle.third, camera_pos);

    return triangle_resized;
}

Vector3f cross(Vector3f u, Vector3f v){
    Vector3f w = Vector3f(0, 0, 0);

    w.setX( (u.getY() * v.getZ()) - (u.getZ() * v.getY()));
    w.setY( (u.getZ() * v.getX()) - (u.getX() * v.getZ()));
    w.setZ( (u.getX() * v.getY()) - (u.getY() * v.getX()));

    return w;
}

Vector3f vertex_normal(Vector3f v, Vector3f a, Vector3f b){
    Vector3f v1 = a - v;
    Vector3f v2 = b - v;
    Vector3f v3 = cross(v1, v2);

    v3.normalize();

    return v3;
}

float scalar(Vector3f u, Vector3f v){
    return v.getX() * u.getX() + v.getY() * u.getY() + v.getZ() * u.getZ();
 //   return u.getNorm() * v.getNorm() * std::cos( (v.getX() * u.getX() + v.getY() * u.getY()) / (u.getNorm() + v.getNorm()) );
}

mat<4,4> model_view(Vector3f eye, Vector3f center, Vector3f up){

    Vector3f z = (center-eye).normalize();
    Vector3f x = cross(up,z).normalize();
    Vector3f y = cross(z,x).normalize();
    mat<4,4> Minv = {{{x.getX(), x.getY(), x.getZ(),0},   {y.getX(), y.getY(), y.getZ(),0},   {z.getX(), z.getY(), z.getZ(),0},   {0,0,0,1}}};
    mat<4,4> Tr   = {{{1,0,0,-eye.getX()}, {0,1,0,-eye.getY()}, {0,0,1,-eye.getZ()}, {0,0,0,1}}};

    return Minv*Tr;
}

mat<4,4> viewport(int x, int y, int w, int h) {
    return {{{w, 0, 0, x+w/2.}, {0, h, 0, y+h/2.}, {0,0,1,0}, {0,0,0,1}}};
}

mat<4,4> projection(const double f) {
    return {{{1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0,0,-1/f,1}}};
}
