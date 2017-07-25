#ifndef QUEUE_H
#define QUEUE_H

#include "Vector.h"

class Queue {
 public:
  /**
   * Constructor.
   * @param length is the size of the queue
   */
  Queue(unsigned int length = 0);

  /**
   * Destructor.
   */
  virtual ~Queue();

  /**
   * Enqueue an element (append it to the end of the queue)
   * @param element to be enqueued
   */
  void enqueue(const double element);

  /**
   * Dequeue an element (extract it from the front of the queue)
   * @warning Undefined behaviour when the queue is empty
   */
  void dequeue();

  /**
   * Returns a reference to the next element in the queue.
   * @return reference to the next element in the queue
   * @warning Undefined behaviour when the queue is empty
   */
  double& front();

  /**
   * Getter of the inner structure to store the queue
   * @return array contaning the elements of the queue
   * @warning a new array is returned and its destruction is responsibility of
   */
  double* getArray();

  /**
   * Returns whether the queue is empty
   * @return whether queue's size is zero
   */
  bool empty();

  /**
   * Returns queue size
   * @return queue size
   */
  unsigned int getSize();

 private:
  Vector input_;
  Vector output_;

  /**
   * Moves every element from input to output, starting from the end to the
   * beginning (i.e, in reversed order).
   */
  void fillOutput();
};

#endif /* QUEUE_H */
