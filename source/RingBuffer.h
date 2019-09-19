/*
  ==============================================================================

    RingBuffer.h
    Created: 22 Mar 2018 4:51:47pm
    Author:  dkoehler

  ==============================================================================
*/

#pragma once

#include <mutex>

namespace dsphelpers {

	template <class T>
	class RingBuffer {
	public:
		RingBuffer(size_t size) :
			buffer(std::unique_ptr<T[]>(new T[size]{ 0 })),
			size(size) {}

		~RingBuffer() {}

		/*
		*/
		void put(T item) {
			buffer[head] = item;
			head = (head + 1) % size;

			//if(head == tail) {
			//	tail = (tail + 1) % size;
			//}
		}
	
		/*
		*/
		T get() {
			//if (empty()) {
			//	return T();
			//}
			auto val = buffer[tail];
			tail = (tail + 1) % size;

			return val;
		}

		/*
		*/
		void reset() {
			head = tail;
		}

		/*
		*/
		bool empty() {
			return head == tail;
		}

		/*
		*/
		bool full() {
			return ((head + 1) % size) == tail;
		}

		/*
		*/
		size_t getSize() {
			return size - 1;
		}

		/*
		*/
		void setTailDistance(int distance) {
			tail = (head - (distance)) % size;
		}

	private:
		std::unique_ptr<T[]> buffer;
		size_t head = 0;
		size_t tail = 0;
		size_t size;
	};

}
