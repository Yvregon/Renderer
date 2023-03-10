#include "tgaimage.h"
#include "Obj.h"
#include "geometry.h"

mat<4,4> ModelView;
mat<4,4> Viewport;
mat<4,4> Projection;
    
Vector3f light_dir = Vector3f(0, 0, 1);
Vector3f camera = Vector3f(1, 1, 3);

constexpr int width = 1024;
constexpr int height = 1024;

triangle_t y_sort(triangle_t triangle){
    if (triangle.first.y < triangle.second.y) std::swap(triangle.first.y, triangle.second.y);
    if (triangle.first.y < triangle.third.y) std::swap(triangle.first.y, triangle.third.y);
    if (triangle.second.y < triangle.third.y) std::swap(triangle.second.y, triangle.third.y);

    return triangle;
}

void line(int x0, int y0, int x1, int y1, TGAColor color, TGAImage &image){

    //Barycentric algorithm
    bool steep = false;

    if(std::abs(x0-x1) < std::abs(y0 - y1)){
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }

    if(x0 > x1){
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    int dx = x1 - x0;
    int dy = y1 - y0;
    int y = y0;
    int dimprec = 2 * std::abs(dy);
    int imprec = 0;

    for(int x = x0; x <= x1; x++){

        if(steep){
            image.set(y, x, color);
        }else{
            image.set(x, y, color);
        }

        imprec += dimprec;
        if(imprec > dx){
            y += dy > 0 ?1 : -1;
            imprec -= 2 * dx;
        }
    }
    //
}

std::vector<std::vector<float>> fill_triangle(triangle_t triangle, std::vector<std::vector<float>> z_buffer, TGAColor color, TGAImage &image){

    Vector3f light_dir = Vector3f(0, 0, 1);

    Vector3f A = Vector3f(triangle.first);
    Vector3f B = Vector3f(triangle.second);
    Vector3f C = Vector3f(triangle.third);

    Vector3f surface_vec = getSurfaceNormal(triangle);

    float intensity = surface_vec.getX() * light_dir.getX() + surface_vec.getY() * light_dir.getY() + surface_vec.getZ() * light_dir.getZ();

    if(intensity >= 0){
        std::vector<point_t> cover = covering_square(triangle.first, triangle.second, triangle.third);
        point_t min = cover[0];
        point_t max = cover[1];

        for(int x = min.x; x <= max.x; x++){
            for(int y = min.y; y <= max.y; y++){

                point_t to_color = {(float) x, (float) y, 0.f};
                Vector3f bary = get_barycentric_vector(triangle, to_color);

                float z_index = bary.getX() * A.getZ() + bary.getY() * B.getZ() + bary.getZ() * C.getZ();

                if(z_buffer[x][y] < z_index && is_into_triangle(triangle, to_color)){
                    z_buffer[x][y] = z_index;
                    image.set(x, y, TGAColor(intensity*color.bgra[2], intensity*color.bgra[1], intensity*color.bgra[0], 255));
                }
            }
        }
    }

    return z_buffer;
}

std::vector<std::vector<float>> fill_triangle(triangle_t triangle, triangle_t normal, triangle_t texture, std::vector<std::vector<float>> z_buffer, TGAImage &textureImg, TGAImage &texture_nm, TGAImage &image){

    Vector3f A = Vector3f(triangle.first);
    Vector3f B = Vector3f(triangle.second);
    Vector3f C = Vector3f(triangle.third);

    //Flat shading
    Vector3f surface_vec = getSurfaceNormal(triangle);
    //Vector3f vertex_vec = Vector3f(vertex_normal(A, B, C).getNorm(), vertex_normal(B, C, A).getNorm(), vertex_normal(C, A, B).getNorm());
    //Vector3f vertex_vec = Vector3f(Vector3f(normal.first).getX(), Vector3f(normal.second).getY(), Vector3f(normal.third).getZ());

    float forward_ind = surface_vec.getX() * camera.getX() + surface_vec.getY() * camera.getY() + surface_vec.getZ() * camera.getZ();

    if(forward_ind >= 0){
        std::vector<point_t> cover = covering_square(triangle.first, triangle.second, triangle.third);
        point_t min = cover[0];
        point_t max = cover[1];
        
        for(int x = min.x; x <= max.x; x++){
            for(int y = min.y; y <= max.y; y++){

                point_t to_color = {(float) x, (float) y, 0.f};
                Vector3f bary = get_barycentric_vector(triangle, to_color);

                float z_index = bary.getX() * A.getZ() + bary.getY() * B.getZ() + bary.getZ() * C.getZ();
                float x_index = bary.getX() * texture.first.x + bary.getY() * texture.second.x + bary.getZ() * texture.third.x;
                float y_index = bary.getX() * texture.first.y + bary.getY() * texture.second.y + bary.getZ() * texture.third.y;

                TGAColor nm_pix = texture_nm.get(x_index, y_index);
                Vector3f text_norm = Vector3f(nm_pix.bgra[2]*2./255.-1., nm_pix.bgra[1]*2./255.-1., nm_pix.bgra[0]*2./255.-1.);

                float nx = text_norm.getX();
                float ny = text_norm.getY();
                float nz = text_norm.getZ();

                //auto n = Vector3f(normal.first) * bary.getX() + Vector3f(normal.second) * bary.getY() + Vector3f(normal.third) * bary.getZ();
                auto n = Vector3f(nx, ny, nz);
                n.normalize();

                float diffuse = std::max(0.f, scalar(n, light_dir));
                Vector3f r = (n*2.*scalar(n, light_dir) - light_dir).normalize();
                float specular = pow(std::max(-r.getZ(), 0.0f), 10.);
                float intensity = diffuse + specular;
         //       std::cerr<<intensity<< std::endl;

                if(x>=0 && x < 1024 && y>=0 && y < 1024){

                    if(z_buffer[x][y] < z_index && is_into_triangle(triangle, to_color)){

                        z_buffer[x][y] = z_index;

                        TGAColor color = textureImg.get(x_index, y_index);
                        color = {255, 255, 255, 255};
                        float red = std::min(intensity*color.bgra[2], 255.f);
                        float green = std::min(intensity*color.bgra[1], 255.f);
                        float blue = std::min(intensity*color.bgra[0], 255.f);
                        image.set(x, y, TGAColor(red, green, blue, 255));  
                    }
                }
            }
        }
    }

    return z_buffer;
}

void line(point_t one, point_t two, TGAColor color, TGAImage &image){
    int x0 = (one.x + 1) * width/2, x1 = (two.x + 1) * width/2, y0 = (one.y + 1) * height/2, y1 = (two.y + 1) * height/2;
    
    line(x0, y0, x1, y1, color, image);
}

void draw_wireframe(std::vector<triangle_t> triangles, TGAColor color, TGAImage &image){

    triangle_t triangle = triangles[0];

    for(std::vector<triangle_t>::iterator it = triangles.begin(); it != triangles.end(); it++){
        triangle_t triangle = (triangle_t) *it;

		line(triangle.first, triangle.second, color, image);
		line(triangle.second, triangle.third, color, image);
		line(triangle.third, triangle.first, color, image);
	}
}

void draw(std::vector<triangle_t> triangles, TGAColor color, TGAImage &image){

    
    std::vector<std::vector<float>> z_buffer(width, std::vector<float>(height));

    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            z_buffer[i][j] = std::numeric_limits<float>::min();
        }
    }

    triangle_t triangle = triangles[0];

    for(std::vector<triangle_t>::iterator it = triangles.begin(); it != triangles.end(); it++){
        triangle_t triangle = (triangle_t) *it;
        triangle = resize(triangle, width, height);

		z_buffer = fill_triangle(triangle, z_buffer, color, image);
	}
}

void draw(std::vector<triangle_t> triangles, std::vector<triangle_t> normals, std::vector<triangle_t> textures, TGAImage &textureImg, TGAImage &texture_nm, TGAImage &image){

    //Z-buffer init
    std::vector<std::vector<float>> z_buffer(width, std::vector<float>(height));

    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            z_buffer[i][j] = -std::numeric_limits<float>::max();
        }
    }

    for(int i = 0; i < triangles.size(); i++){
        triangle_t triangle = triangles[i];
        for (int v : {0,1,2}) {
            mat<4,1> p = Viewport * Projection * ModelView * mat<4,1>{{{triangle[v].x},{triangle[v].y},{triangle[v].z},{1.}}};
            triangle[v] = {p[0][0]/p[3][0], p[1][0]/p[3][0], p[2][0]/p[3][0]};
        }

        triangle_t texture = textures[i];
        triangle_t normal = normals[i];

		z_buffer = fill_triangle(triangle, normal, texture, z_buffer, textureImg, texture_nm, image);
	}
}

int main(int argc, char** argv){
    
    const TGAColor white = {255, 255, 255, 255};
    const TGAColor green = {0, 255, 0, 255};
    const TGAColor red = {255, 0, 0, 255};

    camera = Vector3f(0, 0, 1);
    Vector3f center = Vector3f(0, 0, 0.);
    Vector3f up = Vector3f(0., 1., 0.);
    light_dir = Vector3f(1, 1, 1);
    light_dir.normalize();

    ModelView = model_view(camera, center, up);
    Viewport = viewport(width/8, height/8, width*3/4, height*3/4);
    Projection = projection((camera-center).getNorm());

    TGAImage framebuffer = TGAImage(width, height, TGAImage::RGB);
    TGAImage texture;
    texture.read_tga_file("objs/diablo3_pose_diffuse.tga");
    texture.flip_vertically();

    TGAImage texture_nm;
    texture_nm.read_tga_file("objs/diablo3_pose_nm.tga");
    texture_nm.flip_vertically();
    
    Obj obj = Obj("objs/diablo3_pose.obj");
    obj.parseObject();

    draw(obj.getFaces(), obj.getNormals() ,obj.getTextures(), texture, texture_nm, framebuffer);

    framebuffer.write_tga_file("render/diablo.tga");

    return EXIT_SUCCESS;
}
