#include "systemc.h"
#include "fifo_if.h"


template <class T> class fifo : public sc_module, public fifo_out_if <T>, public fifo_in_if <T>
{
private:
  int ri;                   	// read index
  int wi;                   	// write index
  int size;			// size
  T* buf;
  int num_items;		// number of items in fifo
  
public:
  // constructor
  SC_HAS_PROCESS (fifo);
  fifo (sc_module_name nm, int size_ = 1) : sc_module(nm)
  {
    size = size_;
    buf = new T[size];
    wi = 0;
    ri = 0;
    num_items = 0;
  }

  ~fifo()                   //destructor
  {
    delete [] buf;
  }

  bool write(T data)
  {
    if (wi == ri && num_items != 0) return false;
    else
    {
      buf[wi++] = data;
      wi = wi%size;
      num_items++;
      return true;
    }
  }

  bool read(T& data) 
  {
    wait(0, SC_NS);
    if (num_items > 0) 
    {
      data = buf[ri++];
      ri = ri%size;
      num_items--;
      return true;
    }
    return false;
  }


};
