#include <iostream>
#include <array>
#include <stack>
#include <fstream>
#include <list>
#include <cmath>

using namespace std;

const unsigned dim2 = 20, dim1 = 20;

namespace SOAHash1 {
template <class InputIt, class T>
constexpr InputIt find(InputIt first, InputIt last, const T& value) {
  for (; first != last; ++first) {
    if (*first == value) {
      return first;
    }
  }
  return last;
}
}

class Point {
 public:
  int row, column, move, parent_r, parent_c;
  double value, position;

  Point(const int& r, const int& c, const int& m,
        const array<array<double, dim1 + 1>, dim2 + 1>& G) {
    row = r;
    column = c;
    move = m;
    value = G[r][c];
    position = (m + sqrt(pow(c - 20, 2) + pow(r - 20, 2)));
  }

  Point() {
    row = 1;
    column = 1;
    value = 0;
    position = 1000;
  }
};

bool operator==(const Point& L, const Point& R) {
  return (L.row == R.row && L.column == R.column);
}

bool operator>=(const Point& L, const Point& R) {
  return (L.position >= R.position);
}

list<Point> ListOpen, ListClosed;
stack<string> trace;

Point min() {
  Point tiny;
  tiny.position = 1000;
  for (Point s : ListOpen) {
    auto it = SOAHash1::find(ListClosed.begin(), ListClosed.end(), s);
    if (it == ListClosed.end() && tiny >= s) {
      tiny = s;
    }
  }
  return tiny;
}

enum directionsMove { Down, Up, Left, Right };

void addNextDifferentPoint(Point& direction,
                           const array<array<double, dim1 + 1>, dim2 + 1>& G,
                           const int& c, const int& r, const int& m,
                           const directionsMove& way) {
  if (G[r][c] != 5) {
    switch (way) {
      case Down: {
        direction = Point(r, c, m, G);
        direction.parent_r = r - 1;
        direction.parent_c = c;
        auto it = SOAHash1::find(ListOpen.begin(), ListOpen.end(), direction);
        if (it == ListOpen.end()) {
          ListOpen.push_back(direction);
        }
        break;
      }

      case Up: {
        direction = Point(r, c, m, G);
        direction.parent_r = r + 1;
        direction.parent_c = c;
        auto it = SOAHash1::find(ListOpen.begin(), ListOpen.end(), direction);
        if (it == ListOpen.end()) {
          ListOpen.push_back(direction);
        }
        break;
      }

      case Left: {
        direction = Point(r, c, m, G);
        direction.parent_r = r;
        direction.parent_c = c + 1;
        auto it = SOAHash1::find(ListOpen.begin(), ListOpen.end(), direction);
        if (it == ListOpen.end()) {
          ListOpen.push_back(direction);
        }
        break;
      }

      case Right: {
        direction = Point(r, c, m, G);
        direction.parent_r = r;
        direction.parent_c = c - 1;
        auto it = SOAHash1::find(ListOpen.begin(), ListOpen.end(), direction);
        if (it == ListOpen.end()) {
          ListOpen.push_back(direction);
        }
        break;
      }
    }
  }
}

void generate(const int& r, const int& c, const int& m,
              const array<array<double, dim1 + 1>, dim2 + 1>& G) {
  Point down, left, up, right, chosen;

  if (r + 1 != 21) {
    addNextDifferentPoint(down, G, c, r + 1, m + 1, directionsMove::Down);
  }
  if (c - 1 != 0) {
    addNextDifferentPoint(left, G, c - 1, r, m + 1, directionsMove::Left);
  }
  if (r - 1 != 0) {
    addNextDifferentPoint(up, G, c, r - 1, m + 1, directionsMove::Up);
  }
  if (c + 1 != 21) {
    addNextDifferentPoint(right, G, c + 1, r, m + 1, directionsMove::Right);
  }
}

void set(const Point& end) {
  string route = to_string(end.row) + " " + to_string(end.column);
  trace.push(route);
  if (end.parent_r == 0 && end.parent_c == 0) return;
  for (Point x : ListClosed) {
    if (end.parent_r == x.row && end.parent_c == x.column) set(x);
  }
}

int main(void) {
  string filename("../grid.txt");

  array<array<double, dim1 + 1>, dim2 + 1> G;

  cout << "Press ENTER to load array " << filename;
  getchar();
  cout << "Load data from file..." << endl;

  std::ifstream file(filename.data());

  if (file.fail()) {
    return 0;
  }

  for (auto it = next(G.begin()); it != G.end(); ++it) {
    for (auto it_2 = next(it->begin()); it_2 != it->end(); ++it_2) {
      file >> *it_2;
    }
  }

  file.close();

  cout << endl << "We print on the screen" << endl << endl;
  for (auto it = next(G.begin()); it != G.end(); ++it) {
    for (auto it_2 = next(it->begin()); it_2 != it->end(); ++it_2) {
      cout << " " << *it_2;
    }
    cout << endl;
  }

  Point Start(1, 1, 0, G);
  Start.parent_r = 0;
  Start.parent_c = 0;
  ListOpen.push_front(Start);

  do {
    Point current = min();
    ListClosed.push_back(current);
    if (current.row == 20 && current.column == 20) break;
    generate(current.row, current.column, current.move, G);
  } while (!ListOpen.empty());
  ListOpen.clear();
  Point end = ListClosed.back();
  set(end);
  cout << "Trace: \n";

  for (unsigned i = 1; !trace.empty(); trace.pop()) {
    cout << i++ << ": " << trace.top() << endl;
  }

  cout << endl << "We print on the screen" << endl << endl;
  for (auto it = next(G.begin()); it != G.end(); ++it) {
    for (auto it_2 = next(it->begin()); it_2 != it->end(); ++it_2) {
      cout << " " << *it_2;
    }
    cout << endl;
  }

  cout << endl << endl << "Press ENTER to exit.";
  getchar();

  return 0;
}
