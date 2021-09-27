#ifndef _FIFO_HPP
#define _FIFO_HPP

#include <mutex>
#include <deque>

template<class T> class fifo
{
  protected:
    std::mutex    _fifo_mutex;
    std::deque<T> _fifo;
  public:
    T pop_last();
    T pop_and_flush(int *droppedItems);
    void push(T);
};

template <class T> void fifo<T>::push(T newElement)
{
  // Lock the mutex 
  _fifo_mutex.lock();

  _fifo.push_front(newElement);

  _fifo_mutex.unlock();
  
}

template <class T> T fifo<T>::pop_last(void)
{
  // Lock the mutex
  _fifo_mutex.lock();
 
  T retVal = NULL;

  if( !_fifo.empty())
  {
    retVal = _fifo.back();
    _fifo.pop_back();
  }

  _fifo_mutex.unlock();

  return retVal;
}

template <class T> T fifo<T>::pop_and_flush(int *droppedItems)
{
  _fifo_mutex.lock();

  T retVal = NULL;
  *droppedItems = 0;

  if( !_fifo.empty() )
  {
    retVal = _fifo.front();
    _fifo.pop_front();
  }

  while( !_fifo.empty() )
  {
    free(_fifo.front());
    _fifo.pop_front();
    (*droppedItems)++;
  }

  _fifo_mutex.unlock();

  return retVal;
}

#endif
