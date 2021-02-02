#include "vertice.h"

uint8_t Vertice::x() { return x_; }
uint8_t Vertice::y() { return y_; }

void Vertice::SetX(uint8_t&& x) { x_ = x; }
void Vertice::SetY(uint8_t&& y) { y_ = y; }
