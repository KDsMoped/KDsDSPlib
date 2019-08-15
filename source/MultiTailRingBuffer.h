/*
  ==============================================================================

    Created: 14 Aug 2019
    Author:  Dominik Köhler

  ==============================================================================
*/

#pragma once

namespace kdsp {

    /*
     A Ring buffer that can have multiple independent tails
    */
	template <class T>
	class MultiTailRingBuffer 
    {
	public:
		MultiTailRingBuffer(size_t size, int numTails) : buffer(std::unique_ptr<T[]>(new T[size]{ 0 })),
											             size(size),
											             numTails(numTails) 
        {
			for (int i = 0; i < numTails; ++i) {
				tails.push_back(0);
			}
		}

		~MultiTailRingBuffer() {}

		/*
		*/
		inline void put(T item) 
        {
			buffer[head] = item;
			head = (head + 1) % size;
		}

		inline void put(const T *data, int count) 
        {
			if (size - 1 - head < count) {
				int part1 = size - 1 - head;
				int part2 = count - part1;
				memcpy(&buffer[head], data, part1 * sizeof(T));
				memcpy(&buffer[0], &data[part1], part2 * sizeof(T));
				head = part2;
			}
			else {
				memcpy(&buffer[head], data, count * sizeof(T));
				head += count;
			}
			
		}

		/*
		*/
		T get(int index) 
        {
			auto val = buffer[tails[index]];
			tails[index] = (tails[index] + 1) % size;

			return val;
		}

		void get(int index, T *data, int count) 
        {
			if (size - 1 - tails[index] < count) {
				int part1 = size - 1 - tails[index];
				int part2 = count - part1;
				memcpy(data, &buffer[tails[index]], part1 * sizeof(T));
				memcpy(&data[part1], &buffer[0], part2 * sizeof(T));
				tails[index] = part2;
			}
			memcpy(data, &buffer[tails[index]], count * sizeof(T));
		}

		/*
		*/
		void reset() 
        {
			for (auto &tail : tails) {
				tail = tails[lastTail()];
			}
			head = tails[lastTail()];
		}

		/*
		*/
		bool empty() 
        {
			return head == tails[lastTail()];
		}

		/*
		*/
		bool full() 
        {
			return ((head + 1) % size) == tails[lastTail()];
		}

		/*
		*/
		size_t getSize() 
        {
			return size - 1;
		}

		/*
		*/
		void offsetTailToHead(int range, int index) 
        {
			tails[index] = (head - delay) % size;
		}

		int lastTail() 
        {
			int index = 0;
			int newDiff;
			int oldDiff = 0;
			for (int i = 0; i < numTails; ++i) {
				newDiff = (head - tails[i]) % size;
				if (newDiff > oldDiff)
					oldDiff = newDiff;
					index = i;
			}
			return index;
		}

		int firstTail() 
        {
			int index = 0;
			int newDiff;
			int oldDiff = size;
			for (int i = 0; i < numTails; ++i) {
				newDiff = (head - tails[i]) % size;
				if (newDiff < oldDiff)
					oldDiff = newDiff;
				index = i;
			}
			return index;
		}

	private:
		std::unique_ptr<T[]> buffer;
		size_t head = 0;
		std::vector<size_t> tails;
		size_t size;
		int numTails;
	};

}
