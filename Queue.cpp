#include "Queue.h"

Queue::Queue(unsigned int length) : input_(length), output_(length) {}

Queue::~Queue() {}

void Queue::enqueue(const double element) { input_.push_back(element); }

void Queue::fillOutput() {
  while (!input_.empty()) {
    output_.push_back(input_[input_.getSize() - 1]);
    input_.pop_back();
  }
}
void Queue::dequeue() {
  if (output_.empty()) {
    fillOutput();
  }
  output_.pop_back();
}

double& Queue::front() {
  if (output_.empty()) {
    fillOutput();
  }
  return output_[output_.getSize() - 1];
}

unsigned int Queue::getSize() { return input_.getSize() + output_.getSize(); }

bool Queue::empty() { return input_.empty() && output_.empty(); }

double* Queue::getArray() {
  double* inputArray = input_.getArray();
  double* outputArray = output_.getArray();

  unsigned int inputSize = input_.getSize();
  unsigned int outputSize = output_.getSize();
  unsigned int answerSize = inputSize + outputSize;

  double* answerArray = new double[answerSize];

  if (outputSize != 0) {
    /*
     * Store output_'s elements in reverse order.
     * Remember: Last element from output_ is the one in the front of the queue.
     */
    for (unsigned int i = 0; i != outputSize; i++) {
      answerArray[i] = outputArray[outputSize - 1 - i];
    }
  }

  if (inputSize != 0) {
    /*
     * Store input_'s elements as it is. It is not necessary to reverse here.
     * Remember: Last element from input_ is the last enqueued element.
     */
    for (unsigned int i = outputSize; i != answerSize; i++) {
      answerArray[i] = inputArray[i - outputSize];
    }
  }
  delete[] inputArray;
  delete[] outputArray;
  return answerArray;
}
