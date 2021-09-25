#ifndef _FIFO_HPP
#define _FIFO_HPP

#include <mutex>
#include <deque>

template<class T> class fifo
{
  protected:
    std::mutex  _fifo_mutex;
    std::deque  _fifo;
  public:
    fifo();
    ~fifo();
    T* pop();
    void push(T*);
};

#endif
