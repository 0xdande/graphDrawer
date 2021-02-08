#include "vertice.h"

int Vertice::x() { return x_; }
int Vertice::y() { return y_; }

void Vertice::SetX(int&& x) { x_ = x; }
void Vertice::SetY(int&& y) { y_ = y; }
