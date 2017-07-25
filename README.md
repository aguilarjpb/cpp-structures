# README

## Introduction

The main idea of this project is to define classes in C++ to model a queue based on a vector. Both data structures must be implemented without using STL libraries.

The purpose of this README is to explain in a detailed manner my thought process and the design decisions.

## About the structure of this README

Topics concerning the Vector class implementation are addressed first. This is followed by the Queue class topics.

The internal order maintains a correspondence with the natural order in which the problems were faced.

Some research was conducted in order to provide more formal explanations. The consulted references are listed at the end.

## Vector

### Use of realloc()

At first the function realloc() from cstdlib was considered to be used in order to implement the vector's growing behavior.
This decision would force the use of malloc() or calloc() and free() instead of the C++ version of them: new [] and delete [].
Although it might work in this case because only double type is being used, this is a bad choice given that a generic version of Vector could be required later. The problem is that realloc() may move the current memory block to a new location if it is necessary and in this case neither copy/move constructors or destructors are used. This could lead to inconsistent data. As an example, objects being moved might contain pointers that remain pointing to addresses in the previous memory space. Taking this in account, a combination of new [] an delete [] was used instead.

### Duplicate capacity when full
It is important to note that this decision involves a space-time tradeoff. Lets do a complexity analysis of the push_back() operation to present this formally.

There are two cases to analyze: Whether the capacity of the vector is full or not.

- Capacity is not full:

	As the underlying structure of the vector is an array, the operation is reduced to setting a position to a determinate value and that is O(1).

- Capacity is full:
	In this case, it is also needed to set a position in the vector's array, but a resize() operation has to be done previously to increase vector's capacity.

	The following steps have to be done in a resize() operation:

		1. Create a new array with extra capacity

		2. Copy the elements of the old array to the new one

		3. Set this new array as the vector's array

	It is clear that the second operation is O(previous array length) and it is the one that "bounds" the complexity of the three steps mentioned above.

	Now, let n denote the extra capacity added in a resize() operation. Taking the previous analysis into account, it can be concluded that if n = 1, the run time of successive push_back() calls gets slower, but he memory is managed efficiently, i.e only the necessary memory is allocated. Conversely, if a sufficiently bigger number is given to n, the operation gets almost constant at the price of more memory allocated that might never be used.

	It can be proved that the decision of setting the new capacity as the double of the previous one is enough to reach an amortized constant time of push_back().

### Destructor must be virtual
Recalling the definition of a virtual function:

> A virtual function is a member function that you expect to be redefined in derived classes. When you refer to a derived class object using a pointer or a reference to the base class, you can call a virtual function for that object and execute the derived class's version of the function.

Virtual functions ensure that the correct function is called for an object, regardless of the expression used to make the function call."

Therefore, whether the destructor has to be virtual depends on whether it is going to be possible to delete polymorphically or not (i.e, delete via a pointer to base).
In order to realize the importance of making Vector's destructor virtual in this case, lets assume it non-virtual. Consider now any derived class of Vector (e.g. Queue) and the following situation:

```C++
Vector *v = new Derived();
delete v;
```

Since Vector's destructor is not virtual, and v is Vector * pointing to a Derived object, delete v has undefined behavior . This reason is enough to make it virtual.

Further on this:

> In most implementations, the call to the destructor will be resolved like any non-virtual code, meaning that the destructor of the base class will be called but not the one of the derived class, resulting in a resources leak.

### Getter

In order to implement the getter of the array member variable, two options were considered:

```C++
1. void getArray(double* array) const;
     //or
2. double* getArray() const;
```

Before choosing one or the other, there is a important concern that should be taken into account: The access level of the array must be respected. Given that the array's access level is private, it is totally wrong to return a pointer to the array. This decision would expose it, making its access level useless. Therefore, a new array must be created to hold the result of this function. In the first option, the creation must be done by the caller of the getter prior the call and a pointer to this new array is passed as argument. In the second option, the creation is in charge of the getter.
In both cases, the destruction of the result array is a responsibility of the caller.

The second option was preferred because it is simpler and more readable.

### Overloaded operators

#### =

It is important to note the necessity of implementing the copy assignment operator. If it were not overloaded, it would be implicitly declared:

> The implicitly-defined copy assignment operator for a non-union class X performs memberwise copy assignment of its subobjects. [n3126.pdf section 12.8 §30]

And in this case, having double* array_t as member variable, this implementation would result in a copy of pointers. This is clearly an unwanted behavior.

As an implementation detail, the copy-and-swap idiom was used in order to avoid code duplication and provide a strong exception guarantee.

#### [ ]

The decision of overloading this operator is not as critical as the others. The main reason to do this is that it seems more natural to class' users to use this operator instead of "void set(double elem, unsigned int pos)" and "double get(unsigned int pos)" methods.

#### +, +=

Some available options to implement the addition operator:
```C++
1. friend Vector operator+(const Vector& v, const Vector& w);
     //or
2. Vector& operator+(const Vector& v);
     //or
3. friend Vector addition(const Vector& v, const Vector& w);
```
Option (2) was immediately discarded. It is know that as a rule of thumb that binary arithmetic operators should not be defined as member functions (Many sources, as an example: , E.2.1  Binary Operators). Finally, option (1) was preferred over option (3). The argument is similar to the one used to justify the overload of [ ]: The operation matches with the operator's well-known semantics. If it were not the case, (3) would be better because the name of the function would help a little bit more to understand the operation.

As final details:

- Note that the friend keyword is needed because the function needs access to class' members.

- A small change was made to the chosen option in order to get advantage of the compiler optimizations known as copy elision :

	```C++
	friend Vector operator+(Vector v, const Vector& w);
	```

Although these optimizations might not be available because  they are applied under certain circumstances or may not even be performed by the compiler, performance is not going to get worse because of this change.
- Addition of two vectors of different size was generalized

Concerning the compound operator addition assignment, it was also provided not only because it might be expected to be available as well but because it can be used to implement the previous one.

### Dot Product
Dot product was also generalize to contemplate the case of vectors with different length.

## Queue

### Design: The approach

Given the vector data structure with push_back() and pop_back() operations one could easily simulate a stack. Having this in mind, the problem can be reduced to implement a queue with two stacks. This is a classical and well-known problem. There are different implementations of this it. Here in this project, it was sought to implement the best in terms of time complexity. The Queue class is therefore made of two vectors simulating stacks as private members: Input and Output. The Input 'stack' is used for the enqueue operation and the other one for the dequeue operation. In the following sections, each operation is going to be explained separately.

### Operation: Enqueue

#### Description

Each new element is simply put on top of the Input stack. Note that his is reduced to add a new element to a vector.

#### Complexity analysis

Time complexity: Amortized O(1) (Previously discussed).

Space complexity : O(n) where n is the number of enqueued elements.

### Operation: Dequeue

#### Description

This operation requires to remove the first added element, which is placed at the bottom of the Input stack. Now is when the Output stack is used: It will contain the elements from Input in reversed order. To achieve this, all elements from Input are popped and pushed into Output. From now, the next element to be removed from the queue is always on the top of Output. Once Output is empty, the same steps are applied.

#### Complexity analysis

Time complexity: Amortized O(1) (Previously discussed).

Space complexity : O(1)

### Operation: Front

#### Description

As previously explained, the front of the queue or the next element to be removed is on the top of Output. If Output is empty, the same steps have to be applied. Complexity analysis is the same as Dequeue. The main difference between them is that this operation does not remove the element.

## Implementation details
Many of the implementation topics and practices applied in the implementation of Vector class are valid to this class too.
Only different decisions are going to be explained.

#### Copy Constructor
The definition of the copy constructor was deleted from the header. The reason is because the Queue class does not manage any dynamic allocated resource, and therefore the default copy constructor specified by the standard is enough.

#### Constructor with Queue Length
The only case in which this constructor could make any sense is when the queue's length is fixed , i.e, the case in which the queue length is known beforehand and it is also known that it is not going to grow more than this value. In this case, the inner vectors are initialized with this fixed length and the vector's growing process is avoided yielding a performance improvement.

## References

1. http://www.stroustrup.com/bs_faq2.html
2. http://faculty.cs.tamu.edu/klappi/csce411-s15/csce411-amortized2.pdf
3. https://msdn.microsoft.com/en-us/library/0y01k918.aspx
4. https://www.securecoding.cert.org/confluence/display/cplusplus/OOP52-CPP.+Do+not+delete+a+polymorphic+object+without+a+virtual+destructor
5. http://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
6. http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2010/n3126.pdf
7. http://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom
8. https://www.cs.duke.edu/csed/tapestry/howtoe.pdf
9. https://web.archive.org/web/20140113221447/http://cpp-next.com/archive/2009/08/want-speed-pass-by-value/
10. https://www.cs.cornell.edu/courses/cs3110/2015sp/recitations/16/rec16.html
