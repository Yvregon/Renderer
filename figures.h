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
};

#endif // FIGURES
