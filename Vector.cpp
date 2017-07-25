#include "Vector.h"

Vector::Vector(unsigned int capacity)
    : array_(new double[capacity + 1]()), size_(0), capacity_(capacity + 1) {}

Vector::Vector(const Vector& v)
    : array_(new double[v.capacity_]()),
      size_(v.size_),
      capacity_(v.capacity_) {
  for (unsigned int i = 0; i != size_; i++) {
    array_[i] = v.array_[i];
  }
}

Vector::Vector(const double* const array, const unsigned int nArray)
    : array_(new double[2 * nArray]()), size_(nArray), capacity_(2 * nArray) {
  for (unsigned int i = 0; i != size_; i++) {
    array_[i] = array[i];
  }
}

Vector::~Vector() { delete[] array_; }

void swap(Vector& first, Vector& second) {
  using std::swap;
  swap(first.array_, second.array_);
  swap(first.size_, second.size_);
  swap(first.capacity_, second.capacity_);
}

Vector& Vector::operator=(Vector other) {
  swap(*this, other);
  return *this;
}

void Vector::resize(unsigned int newSize) {
  /*
   * Precondition: newSize >= size_
   * It is not necessary, but vector's shrink behaviour when newSize < size_
   * could have been implemented.
   */
  double* newArray = new double[newSize]();
  for (unsigned int i = 0; i != size_; i++) {
    newArray[i] = array_[i];
  }
  delete[] array_;
  array_ = newArray;
  capacity_ = newSize;
}

void Vector::push_back(double elem) {
  if (size_ >= capacity_) {  // Capacity is full
    resize(2 * capacity_);
  }
  array_[size_] = elem;
  size_++;
}

bool Vector::empty() { return size_ == 0; }

void Vector::pop_back() {
  if (size_ > 0) {
    size_--;  // Now position at size_ is available to be replaced
  }
}

double* Vector::getArray() const {
  double* newArray = new double[size_];
  for (unsigned int i = 0; i != size_; i++) {
    newArray[i] = array_[i];
  }
  return newArray;
}

void Vector::setArray(const double* const array, const unsigned int nArray) {
  if (capacity_ < nArray) {
    resize(2 * nArray);
  }
  for (unsigned int i = 0; i != nArray; i++) {
    array_[i] = array[i];
  }
  size_ = nArray;
}

unsigned int Vector::getSize() const { return size_; }

std::string Vector::toString() {
  // Output format: {x1, x2, ..., xn}
  std::string answer = "{";

  for (unsigned int i = 0; i != size_; i++) {
    answer += std::to_string(array_[i]);

    if (i + 1 != size_) {
      answer += ", ";
    }
  }

  answer += "}";

  return answer;
}

double& Vector::operator[](unsigned int pos) { return array_[pos]; }

const double& Vector::operator[](unsigned int pos) const { return array_[pos]; }

Vector& Vector::operator+=(const Vector& w) {
  // First, add until last common position
  for (unsigned int i = 0; i != std::min(size_, w.size_); i++) {
    array_[i] += w.array_[i];
  }

  // Complete if w's size is different
  if (size_ < w.size_) {
    if (capacity_ < w.size_) {
      resize(2 * w.size_);
    }
    for (unsigned int i = size_; i != w.size_; i++) {
      array_[i] = w.array_[i];
    }
    size_ = w.size_;
  }
}

Vector operator+(Vector v, const Vector& w) {
  v += w;
  return v;
}

double dotProduct(const Vector& v, const Vector& w) {
  double answer = 0;

  // First, add until last common position
  for (unsigned int i = 0; i != std::min(v.size_, w.size_); i++) {
    answer += (v.array_[i] * w.array_[i]);
  }

  // Complete if vectors' size is different
  if (v.size_ < w.size_) {
    for (unsigned int i = v.size_; i != w.size_; i++) {
      answer += w.array_[i];
    }
  } else if (v.size_ > w.size_) {
    for (unsigned int i = w.size_; i != v.size_; i++) {
      answer += v.array_[i];
    }
  }

  return answer;
}
