#ifndef VERTICE_H
#define VERTICE_H
#include <cstdint>
class Vertice {
public:
  Vertice() : x_(0), y_(0) {}
  Vertice(int x, int y) : x_(x), y_(y) {}
  int x();
  int y();

  void SetX(int &&x);
  void SetY(int &&y);

private:
  int x_;
  int y_;
};

#endif // VERTICE_H
