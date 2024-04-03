#include <iostream>
#include <fstream>
#include "Script.hpp"
#include "PNG.hpp"
#include "XPM2.hpp"
#include <algorithm>

using namespace std;

namespace prog {
    // Use to read color values from a script file.
    istream& operator>>(istream& input, Color& c) {
        int r, g, b;
        input >> r >> g >> b;
        c.red() = r;
        c.green() = g;
        c.blue() = b;
        return input;
    }

    Script::Script(const string& filename) :
            image(nullptr), input(filename) {
    }
    void Script::clear_image_if_any() {
        if (image != nullptr) {
            delete image;
            image = nullptr;
        }
    }
    Script::~Script() {
        clear_image_if_any();
    }

    void Script::run() {
        string command;
        while (input >> command) {
            cout << "Executing command '" << command << "' ..." << endl;
            if (command == "open") {
                open();
                continue;
            }
            if (command == "blank") {
                blank();
                continue;
            }
            if(command == "fill") {
                fill();
                continue;
            }
            if (command == "save") {
                save();
                continue;
            } 
            if(command == "add") {
                add();
                continue;
            }
            if(command == "replace") {
                replace();
                continue;
            }
            if(command == "rotate_left") {
                rotate_left();
                continue;
            }
            if(command == "rotate_right") {
                rotate_right();
                continue;
            }
            if(command == "to_gray_scale") {
                to_gray_scale();
                continue;
            }
            if(command == "invert") {
                invert();
                continue;
            }
            if(command == "h_mirror") {
                h_mirror();
                continue;
            }
            if(command == "v_mirror") {
                v_mirror();
                continue;
            }
            if(command == "median_filter") {
                median_filter();
                continue;
            }
            if(command == "crop") {
                crop();
                continue;
            }
            if(command == "xpm2_open") {
                xpm2_open();
                continue;
            }
            if(command == "xpm2_save") {
                xpm2_save();
                continue;
            }
        }
    }
    void Script::open() {
        // Replace current image (if any) with image read from PNG file.
        clear_image_if_any();
        string filename;
        input >> filename;
        image = loadFromPNG(filename);
    }
    void Script::blank() {
        // Replace current image (if any) with blank image.
        clear_image_if_any();
        int w, h;
        Color fill;
        input >> w >> h >> fill;
        image = new Image(w, h, fill);
    }
    void Script::save() {
        // Save current image to PNG file.
        string filename;
        input >> filename;
        saveToPNG(filename, image);
    }

    void Script::xpm2_open() {
        // Replace current image (if any) with image read from XPM2 file.
        clear_image_if_any();
        string filename;
        input >> filename;
        image = loadFromXPM2(filename);
    }

    void Script::xpm2_save() {
        // Save current image to XPM2 file.
        string filename;
        input >> filename;
        saveToXPM2(filename, image);
    }

    void Script::fill() {
        // Assign (r, g, b) to all pixels contained in rectangle defined by top-left corner (x, y), width w, and height h.
        int x,y,w,h;
        Color fill;
        input >> x >> y >> w >> h >> fill;

        for (int column=y;column<y+h;column++){
            for (int row=x;row<x+w;row++){
                image->at(row,column)=fill;
            }
        }
    }
    void Script::add() {
        // Copy all pixels from image stored in PNG file filename, except pixels in that image with “neutral” color (r, g, b).
        Image *copying;
        int r, g, b, x, y, i = 0, j = 0;
        string filename;
        input >> filename >> r >> g >> b >> x >> y;
        copying = loadFromPNG(filename);

        for (int row = x; row < x + copying->width(); row++) {
            for (int column = y; column < y + copying->height(); column++) {
                Color current;
                current = copying->at(i,j);
                if (!(current.red()==r && current.green()==g && current.blue()==b)){
                    image->at(row, column) = current;
                }
                j++;
            }
            j=0;
            i++;
        }

        delete copying;
    }
    
    void Script::rotate_left() {
        // Rotates the current image 90 degrees to the left
        Image *new_image = new Image(image->height(),image->width());
        for(int y = 0; y < image->height(); y++){
            for(int x = 0; x < image->width();x++){
                new_image->at(y,new_image->height()-1-x) = image->at(x,y);
            }
        }
        *image = *new_image;
        delete new_image;
    }
    void Script::rotate_right(){
        // Rotates the current image 90 degrees to the right
        Image *new_image = new Image(image->height(),image->width());
        for(int y = 0; y < image->height(); y++){
            for(int x = 0; x < image->width();x++){
                new_image->at(new_image->width()-1-y,x) = image->at(x,y);
            }
        }

        *image = *new_image;
        delete new_image;
    }
    void Script::to_gray_scale(){
        // Transforms each individual pixel (r, g, b) to (v, v, v) where v = (r + g + b)/3
        for(int y = 0; y < image->height(); y++){
            for(int x = 0; x < image->width();x++){
                rgb_value avg = (image->at(x, y).red() + image->at(x, y).green() + image->at(x, y).blue()) / 3;
                image->at(x, y).red() = avg;
                image->at(x, y).green() = avg;
                image->at(x, y).blue() = avg;
             }
        }
    }
    void Script::invert(){
        // Transforms each individual pixel (r, g, b) to (255-r,255-g,255-b)
        for(int y = 0; y < image->height(); y++){
            for(int x = 0; x < image->width();x++){
                image->at(x, y).red() = 255 - image->at(x, y).red();
                image->at(x, y).green() = 255 - image->at(x, y).green();
                image->at(x, y).blue() = 255 - image->at(x, y).blue();
            }
        }
    }
    void Script::h_mirror(){
        // Mirror current image horizontally
        for(int y = 0; y < image->height(); y++){
            for(int x = image->width() / 2; x < image->width();x++){
                Color temp = image->at(x, y);
                image->at(x, y) = image->at(image->width() - x - 1, y); 
                image->at(image->width() - x - 1, y) = temp;
             }
        }
    }
    void Script::v_mirror(){
        // Mirror current image vertically
        for(int y = image->height() / 2; y < image->height(); y++){
            for(int x = 0; x < image->width();x++){
                Color temp = image->at(x, y);
                image->at(x, y) = image->at(x, image->height() - y - 1); 
                image->at(x, image->height() - y - 1) = temp;
             }
        }
    }

    void Script::crop(){
        // Crop the image, reducing it to all pixels contained in the rectangle defined by top-left corner (x, y), width w, and height h. 
        int x, y, w, h;
        input >> x >> y >> w >> h;
        Image *new_image = new Image(w, h);

        for (int row = 0; row < w; row++) {
            for (int column = 0; column < h; column++) {
                new_image->at(row, column) = image->at(x + row, y + column);
            }
        }

        *image = *new_image;
        delete new_image;
    }
    void Script::median_filter(){
        /* 
        Replaces each pixel(x,y) by "a median pixel" of neighboring pixels to (x,y)
        inside an window size(ws) where ws >= 3 and odd.
        More about the median filter algorithm in https://en.wikipedia.org/wiki/Median_filter.
        */
        int ws;
        input >> ws;
        Image* new_image = new Image(image->width(),image->height());
        for(int y = 0; y < image->height(); y++)
        {
            for (int x = 0; x < image->width(); x++)
            {
                new_image->at(x,y) = get_median_adjacent(ws, x, y); 
            }
        }
        *image = *new_image;
        delete new_image;
    
    }
    void Script::replace(){
        // Replaces all (r1,  g1, b1) pixels by (r2,  g2, b2)
        Color c1, c2;
        input >> c1 >> c2;
         for(int y = 0; y < image->height(); y++){
            for(int x = 0; x < image->width();x++){
                if (image->at(x, y).red() == c1.red() && image->at(x, y).green() == c1.green() && image->at(x, y).blue() == c1.blue()) {
                image->at(x, y).red() = c2.red();
                image->at(x, y).green() = c2.green();
                image->at(x, y).blue() = c2.blue();
             }
         }
         }
    }

    Color Script::get_median_adjacent(int n, int x, int y)
    {
        // Returns the median color of neighboring pixels to (x,y) with a range of n
        std::vector<rgb_value> red_values,green_values,blue_values;
        int w = image->width();
        int h = image->height();

        int nx_min = x-n/2 > 0 ? x-n/2 : 0;
        int nx_max = x + n/2 < w - 1 ? x + n/2 : w - 1;

        int ny_min = y-n/2 > 0 ? y-n/2 : 0;
        int ny_max = y + n/2 < h - 1 ? y+n/2 : h-1;

        for(int ny = ny_min ; ny <= ny_max ; ny++)
        {
            for(int nx = nx_min ; nx <= nx_max; nx++)
            {
                red_values.push_back(image->at(nx,ny).red());
                green_values.push_back(image->at(nx,ny).green());
                blue_values.push_back(image->at(nx,ny).blue());
            }
        }
        return Color(median(red_values),median(green_values),median(blue_values));

    }

    rgb_value Script::median(std::vector<rgb_value> v){
        // Returns a median of an vector
        int n = v.size();
        std::sort(v.begin(),v.end());
        rgb_value result = n % 2 != 0 ? v[n/2] : (v[ n / 2 - 1] + v[ n / 2]) / 2;
        return result;
    }


}
