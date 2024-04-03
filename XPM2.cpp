#include "XPM2.hpp"
#include "PNG.hpp"
#include <iostream>
#include <sstream>
#include <map>
#include <iomanip>
#include <fstream>

namespace prog {
    Image* loadFromXPM2(const std::string& file) {
        // Read image stored in the XPM2 file format.
        std::ifstream in(file);
        int w, h, colors, chars; std::string header;
        in >> header >> header >> w >> h >> colors >> chars;
        if(header != "XPM2"){
            return nullptr;
        }
        Image* image = new Image(w, h);
        std::vector<std::pair<Color, char>> color_key;
        for (int i = 0; i < colors; i++) {               //Define color keys
            char c, key; std::string hex_key; Color color;
            in >> key >> c >> hex_key;
            color.red() = std::stoul(hex_key.substr(1, 2), nullptr, 16);
            color.green() = std::stoul(hex_key.substr(3, 2), nullptr, 16);
            color.blue() = std::stoul(hex_key.substr(5, 2), nullptr, 16);
            color_key.push_back(std::pair<Color, char> {color, key});
        } 
        for (int y = 0; y < image->height(); y++) {      //Define pixels using color keys
            for (int x = 0; x < image->width(); x++) {
                char c; in >> c;
                for (const auto &elem : color_key) {
                    if (elem.second == c)
                        image->at(x, y) = elem.first;
                }
            }
        }
        return image;
    }
    void saveToXPM2(const std::string& file, const Image* image) {
        // Stores current image in a XPM2 file. 
        int colors = 0;
        std::map<Color,char> color_key;
        std::ofstream out(file);
        for(int y = 0; y < image->height(); y++) {
            for(int x = 0; x < image->width();x++) {
                if(color_key.find(image->at(x,y))==color_key.end()){
                    colors++;
                    color_key[image->at(x,y)] = 'a'+ colors;
                }
            }
        }
        out << "! XPM2" << "\n";
        out << image->width() << " " << image->height() << " " << colors << " 1"  << '\n';
        for(auto pair : color_key){
            out << pair.second << " c " << convert_to_hex(pair.first) << '\n';       
        }
        for(int y = 0; y < image->height();y++){
            for(int x = 0; x < image->width();x++){
                out << color_key[image->at(x,y)];
            }
            out << '\n';
        }

    }

    std::string convert_to_hex(Color color) {
        std::stringstream sstream;
        sstream << "#"<< std::setfill ('0') << std::setw(2) << std::hex << (int)color.red() 
        << std::setfill ('0') << std::setw(2) << std::hex << (int)color.green() 
        << std::setfill ('0') << std::setw(2) << std::hex << (int)color.blue();
        return sstream.str();
    }

    bool operator<(const Color& c1,const Color& c2){
        if(c1.red() < c2.red()){
            return true;
        } else if(c1.red() == c2.red()){
            if(c1.green() < c2.green()){
                return true;
            } else if(c1.green()== c2.green()){
                return c1.blue() < c2.blue();
            }
        }
        return false;
    }
}
