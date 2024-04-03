#include "Color.hpp"

namespace prog {
    Color::Color() // Default Constructor 
    { 
        this->red_ = 0;
        this->green_ = 0;
        this->blue_ = 0;
    }
    Color::Color(const Color& other) // Copy Constructor
    { 
        this->red_ = other.red();
        this->green_ = other.green();
        this->blue_ = other.blue();
    }
    Color::Color(rgb_value red, rgb_value green, rgb_value blue) {
        this->red_ = red;
        this->green_ = green;
        this->blue_ = blue;
    }
    rgb_value Color::red() const {
        return red_;
    }
    rgb_value Color::green() const {
        return green_;
    }
    rgb_value Color::blue() const {
        return blue_;
    }

    rgb_value& Color::red()  {
        return red_;
    }
    rgb_value& Color::green()  {
      return green_;
    }
    rgb_value& Color::blue()  {
      return blue_;
    }
}
