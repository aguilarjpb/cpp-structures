#ifndef VECTOR_H
#define VECTOR_H

#include <cassert>
#include <string>

/**
 * Vector class
 * Representation of a vector and its operations
 */
class Vector {
 public:
  /**
   * Constructor. Initializes capacity and memory allocation
   * @param capacity of the vector. 0 by default
   */
  Vector(unsigned int capacity = 0);

  /**
   * Costructor. Copy vector v.
   * @param v vector to be copied
   */
  Vector(const Vector& v);

  /**
   * Constructor. Create vector from array
   * @param array to be copied in the current object
   * @param nArray size of the array
   */
  Vector(const double* const array, const unsigned int nArray);

  /**
   * Destructor.
   * @warning this destructor MUST be virtual.
   */
  virtual ~Vector();

  /** @name Operations */
  //@{
  /**
   * Addition of vectors
   * @param v the first vector to be added
   * @param w the second vector to be added
   * @return the vector v + w
   */
  friend Vector operator+(Vector v, const Vector& w);

  /**
   * Dot product
   * @param v the first vector
   * @param w the second vector
   * @returns a single number: the sum of the products of the corresponding
   * entries of the two vectors
   */
  friend double dotProduct(const Vector& v, const Vector& w);
  //@}

  /**
   * Getter of the array
   * @warning a new array is returned and its destruction is responsibility of
   * the caller of this function
   * @return pointer to a new array with a copy of the member one.
   */
  double* getArray() const;

  /**
   * Setter of the array
   * @param array to be copied in the current object
   * @param nArray size of the array
   */
  void setArray(const double* const array, const unsigned int nArray);

  /**
   * Get the number of elements of the vector
   * @return size of vector
   */
  unsigned int getSize() const;

  /**
   * Get the vector as a string
   * @return a string that represents the vector
   */
  std::string toString();

  /**
   * Adds a new element at the end of the vector, after its current last element
   * @param elem element to be added
   */
  void push_back(double elem);

  /**
   * Removes the last element in the vector.
   * @warning Nothing happends if vector is empty
   */
  void pop_back();

  /**
   * Returns whether the vector is empty
   * @return whether vector's size is zero
   */
  bool empty();

  /**
   * Array subscript operator
   * @param pos position in array
   * @return reference to vector's object
   */
  double& operator[](unsigned int pos);

  /**
   * Array subscript operator
   * @param pos position in array
   * @return const reference to vector's object
   *
   * Why? What if vector's elements are const? in that
   * case, the above defined operator can not be used
   */
  const double& operator[](unsigned int pos) const;

  /**
   * Addition assignment
   * @param w the vector to be added to this
   */
  Vector& operator+=(const Vector& w);

  /**
   * Swap two vectors
   * @param first vector to be swapped
   * @param second vector to be swapped
   */
  friend void swap(Vector& first, Vector& second);

  /**
   * Assignment operator
   * @param other vector to be copied
   * @return reference to this after copy
   */
  Vector& operator=(Vector other);

 private:
  double* array_;
  unsigned int size_;
  unsigned int capacity_;

  /**
   * Expands or shrinks vector capacity
   * @param n is the size of the vector
   * @param val default value
   */
  void resize(unsigned int newSize);
};

#endif /* VECTOR_H */
