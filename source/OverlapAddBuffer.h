/*
  ==============================================================================

    OverlapAddBuffer.h
    Created: 23 Mar 2018 12:26:25pm
    Author:  domin

  ==============================================================================
*/

#pragma once

#include "RingBuffer.h"

namespace dsphelpers {
	/**
	*/
	template <class T>
	class OverlapAddBuffer {
	public:
		OverlapAddBuffer(size_t size, int order) : size(size), order(order) {
			for (int i = 0; i < order; ++i) {
				buffers.push_back(new T[size]{0});
				indices.push_back((-i*size/order) % size);
			}
			bufferCounter = 0;
		}

		~OverlapAddBuffer() {
			for (auto buffer : buffers) {
				delete buffer;
			}
		}

		/**
		*/
		void fill(T sample) {

		}

		/**
		*/
		T* getNextBuffer() {
			T* ptr = buffers[bufferCounter];
			indices[bufferCounter] = 0;
			//indices[bufferCounter] = (indices[0] - bufferCounter*size/order) % size;
			if (++bufferCounter == order)
				bufferCounter = 0;
			return ptr;
		}
		
		/**
		*/
		T read() {
			float sum = 0;
			for (int i = 0; i < order; ++i) {
				sum += buffers[i][indices[i]];
				indices[i] = (indices[i] + 1) % size;
			}
			
			return sum;
		}

		/**
		*/
		void resize(size_t size, int order) {
			size = size;
			order = order;
			reset();
		}

		/**
		*/
		void reset() {
			buffers.clear();
			indices.clear();
			for (int i = 0; i < order; ++i) {
				buffers.push_back(new T[size]{0});
				indices.push_back((-i*size / order) % size);
			}
			bufferCounter = 0;
		}


	private:
		std::vector<T*> buffers;
		std::vector<int> indices;

		size_t size;
		int order;
		int bufferCounter;

	};
}
