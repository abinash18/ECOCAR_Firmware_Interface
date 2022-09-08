#ifndef CIRCULARBUFFER_HPP
#define CIRCULARBUFFER_HPP

namespace ecocar{


	template<class T, int Size> class CircularBuffer
	{
		private:
		T buffer[Size];
		int firstindex = 0;
		int numitems = 0;
		int capacity = Size;
		public:
		bool Push(T item)
		{
			if(numitems == capacity)
				return false;
			int placement_index = (firstindex + numitems) % capacity;
			buffer[placement_index] = item;
			numitems++;
			return true;
		}
		T Peek(void)
		{
			return buffer[firstindex];
		}
		int Count(void)
		{
			return numitems; 
		}
		T Pop(void)
		{
			//make sure there's an item here first
			T ret = this->Peek();
			numitems--;
			firstindex = (firstindex + 1) % capacity;
			return ret;
		}
		

	};

}

template <class T>
struct IMessageDispatch
{
	virtual bool Send(T item);
};


#endif
