#ifndef VERTICE_H
#define VERTICE_H
#include <cstdint>
class Vertice {
 public:
  Vertice() : x_(0), y_(0) {}
  Vertice(uint8_t x, uint8_t y) : x_(x), y_(y) {}
  uint8_t x();
  uint8_t y();

  void SetX(uint8_t&& x);
  void SetY(uint8_t&& y);

 private:
  uint8_t x_;
  uint8_t y_;
};

#endif  // VERTICE_H
