#if !defined(FIGURES)
#define FIGURES

struct point_t{
    double x;
    double y;
    double z;
};

struct triangle_t{
    point_t first;
    point_t second;
    point_t third;
    point_t& operator[](const int i)       { return i ? (1==i ? second : third) : first; }
};

#endif // FIGURES
