#include "Image.hpp"
#include <vector>
#include <iostream>

namespace prog
{
  Image::Image(int w, int h, const Color &fill)
  {
    //FIXED
    width_ = w;
    height_ = h;
    img.resize(h, std::vector<Color>(w,fill));
  }
  Image::~Image()
  {
    
  }
  int Image::width() const
  {
    return width_;
  }
  int Image::height() const
  {
    return height_;
  }

  Color& Image::at(int x, int y)
  {
    return img.at(y).at(x);
  }

  const Color& Image::at(int x, int y) const
  {
    return img.at(y).at(x);
  }
}
