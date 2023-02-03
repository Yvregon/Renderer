#include <math.h>
#include "figures.h"

class Vector3f{

    private:

        float x;
        float y;
        float z;
        double norm;

        void computeNorm();

    public:

        Vector3f(float x, float y, float z);

        Vector3f(point_t point);

        void setX(float x);

        void setY(float y);

        void setZ(float z);

        float getX();

        float getY();

        float getZ();

        double getNorm();

        void normalize();

        Vector3f operator+(Vector3f v);

        Vector3f operator-(Vector3f v);

        Vector3f operator*(Vector3f v);

        Vector3f operator*(float f);
};

