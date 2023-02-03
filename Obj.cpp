#include "Obj.h"

Obj::Obj(std::string filepath){
    this->filepath = filepath;
}

std::vector<std::string> Obj::split(std::string s, std::string delimiter)
{

    int cursor = 0;
    int end = 0;

    std::string substring;
    std::vector<std::string> result;

    while((end = s.find (delimiter, cursor)) != std::string::npos){
        substring = s.substr (cursor, end - cursor);
        cursor = end + delimiter.length();;
        result.push_back(substring);
    }

    result.push_back (s.substr (cursor));

    return result;
}

void Obj::parseObject(){

    std::vector<point_t> v_points;
    std::vector<point_t> vn_points;
    std::vector<point_t> vt_points;
    std::string file_format = split(filepath, ".")[1];

    if(file_format.compare("obj")){
        std::cerr << "Unsupported file format : " <<  file_format << std::endl;
    }else{

        std::fstream image;
        std::string text;
        image.open(filepath);

        while(getline(image, text)){

            std::vector<std::string> buffer = split(text, " ");

            if(!buffer[0].compare("v")){

                point_t point = {stod(buffer[1]), stod(buffer[2]), stod(buffer[3])};
                v_points.push_back(point);

            }else if(!buffer[0].compare("vn")){

                point_t point = {stod(buffer[2]), stod(buffer[3]), stod(buffer[4])};
                vn_points.push_back(point);

            }else if(!buffer[0].compare("vt")){

                point_t point = {stod(buffer[2])*1024, stod(buffer[3])*1024, stod(buffer[4])};
                vt_points.push_back(point);

            }else if(!buffer[0].compare("f")){

                int indice = stoi(split(buffer[1], "/")[0]) - 1;
                int texture_ind = stoi(split(buffer[1], "/")[1]) - 1;
                int normal_ind = stoi(split(buffer[1], "/")[2]) - 1;
                point_t first = v_points[indice];
                point_t t1 = vt_points[texture_ind];
                point_t n1 = vn_points[normal_ind];


                indice = stoi(split(buffer[2], "/")[0]) - 1;
                texture_ind = stoi(split(buffer[2], "/")[1]) - 1;
                normal_ind = stoi(split(buffer[2], "/")[2]) - 1;
                point_t second = v_points[indice];
                point_t t2 = vt_points[texture_ind];
                point_t n2 = vn_points[normal_ind];


                indice = stoi(split(buffer[3], "/")[0]) - 1;
                texture_ind = stoi(split(buffer[3], "/")[1]) - 1;
                normal_ind = stoi(split(buffer[3], "/")[2]) - 1;
                point_t third = v_points[indice];
                point_t t3 = vt_points[texture_ind];
                point_t n3 = vn_points[normal_ind];


                triangle_t v_triangle = {first, second, third};
                triangle_t vt_triangle = {t1, t2, t3};
                triangle_t vn_triangle = {n1, n2, n3};

                faces.push_back(v_triangle);
                textures.push_back(vt_triangle);
                normals.push_back(vn_triangle);
            }
        }

        image.close();
        std::cout << "File opened" << std::endl;
    }
}

std::vector<triangle_t> Obj::getFaces(){
    return faces;
}

std::vector<triangle_t> Obj::getTextures(){
    return textures;
}

std::vector<triangle_t> Obj::getNormals()
{
    return normals;
}
