#ifndef __prog_Script_hpp__
#define __prog_Script_hpp__

#include <string>
#include <fstream>
#include "Image.hpp"

namespace prog
{
  class Script
  {
  public: 
    Script(const std::string &filename);
    ~Script();
    void run();
  private:
    // Current image.
    Image *image;
    // Input stream for reading script commands.
    std::ifstream input;
  private:
    // Private functions
    void clear_image_if_any();
    void open();
    void blank();
    void save();

    void add();
    void replace();
    void rotate_left();
    void rotate_right();
    void to_gray_scale();
    void invert();
    void h_mirror();
    void v_mirror();
    void median_filter();
    void fill();
    
    void crop();
    void xpm2_open();
    void xpm2_save();

    Color get_median_adjacent(int n, int x, int y);
    rgb_value median(std::vector<rgb_value> v);
  };
}
#endif
