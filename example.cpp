#include <cassert>
#include <iostream>
#include <utility>
#include <vector>

template <typename T>
void swap(T& a, T& b) {
  T c = a;
  a = b;
  b = c;
}

template <typename T>
class Matrix {
private:
  std::vector<std::vector<T>> Data;

  class Iterator {
  private:
    Matrix<T>& M;
    size_t i, j;

  public:
    Iterator(size_t i, size_t j, Matrix& M) : M(M), i(i), j(j) {}

    T& operator*() const {
      return M.Data[i][j];
    }

    Iterator& operator++() {
      auto [h, w] = M.size();
      ++j;
      if (j == w) {
        ++i;
        j = 0;
      }
      return *this;
    }

    Iterator& operator++(int) {
      ++(*this);
      return *this;
    }

    bool operator==(Iterator other) const {
      return i == other.i && j == other.j;
    }

    bool operator!=(Iterator other) const {
      return i != other.i || j != other.j;
    }
  };

  Iterator Begin, End;

public:
  Matrix(const std::vector<std::vector<T>>& data) : Data(data),
                                                    Begin(Iterator(0, 0, *this)),
                                                    End(Iterator(data.size(), 0, *this)) {}

  Iterator& begin() {
    return Begin;
  }

  Iterator begin() const {
    return Begin;
  }

  Iterator& end() {
    return End;
  }

  Iterator end() const {
    return End;
  }

  friend std::ostream& operator<<(std::ostream& out, const Matrix<T>& m) {
    for (size_t i = 0; i < m.Data.size(); ++i) {
      for (size_t j = 0; j < m.Data[i].size(); ++j) {
        if (j != 0)
          out << "\t";
        out << m.Data[i][j];
      }
      if (i != m.Data.size() - 1)
        out << "\n";
    }
    return out;
  }

  std::pair<size_t, size_t> size() const {
    if (Data.size() == 0)
      return {0, 0};
    return {Data.size(), Data[0].size()};
  }

  Matrix& operator+=(const Matrix& other) {
    auto [h, w] = other.size();
    for (size_t i = 0; i < h; ++i)
      for (size_t j = 0; j < w; ++j)
        Data[i][j] += other.Data[i][j];
    return *this;
  }

  Matrix operator+(const Matrix& other) const {
    Matrix temp(Data);
    temp += other;
    return temp;
  }

  template <typename Scalar>
  Matrix& operator*=(const Scalar& s) {
    auto [h, w] = size();
    for (size_t i = 0; i < h; ++i)
      for (size_t j = 0; j < w; ++j)
        Data[i][j] *= s;
    return *this;
  }

  template <typename Scalar>
  Matrix operator*(const Scalar& s) const {
    Matrix temp(Data);
    temp *= s;
    return temp;
  }

  Matrix operator*(const Matrix& other) const {
    auto [h, width] = size();
    auto [height, w] = other.size();
    assert(width == height);
    std::vector<std::vector<T>> temp(h, std::vector<T>(w));
    for (size_t i = 0; i < h; ++i)
      for (size_t j = 0; j < w; ++j)
        for (size_t k = 0; k < width; ++k)
          temp[i][j] += Data[i][k] * other.Data[k][j];
    return Matrix(temp);
  }

  Matrix& operator*=(const Matrix& other) {
    *this = *this * other;
    return *this;
  }

  Matrix transposed() const {
    auto [h, w] = size();
    auto temp = Matrix(std::vector<std::vector<T>>(w, std::vector<T>(h)));
    for (size_t i = 0; i < h; ++i)
      for (size_t j = 0; j < w; ++j)
        temp.Data[j][i] = Data[i][j];
    return temp;
  }

  Matrix& transpose() {
    Data = transposed().Data;
    return *this;
  }
};