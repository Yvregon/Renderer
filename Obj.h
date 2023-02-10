#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include "figures.h"
#include "tgaimage.h"

class Obj{
    private :

        std::string filepath;
        std::vector<triangle_t> faces;
        std::vector<triangle_t> normals;
        std::vector<triangle_t> textures;

        std::vector<std::string> split(std::string s, std::string delimiter);

    public :

        Obj(std::string filepath);
        void parseObject();
        std::vector<triangle_t> getFaces();
        std::vector<triangle_t> getTextures();
        std::vector<triangle_t> getNormals();
};
