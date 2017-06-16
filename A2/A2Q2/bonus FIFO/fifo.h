#include "systemc.h"
#include "fifo_if.h"


template <class T> class fifo : public sc_module, public fifo_out_if <T>, public fifo_in_if <T>
{
private:
  int ri;                   	// read index
  int wi;                   	// write index
  int size;			// size
  T* buf;			// FIFO buffer
  int num_items;		// number of items in fifo

  sc_event read_req, write_req;
  sc_event read_ack, write_ack;
  bool read_flag, write_flag;
  
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
    
    read_flag = false;
    write_flag = false;
    SC_THREAD(arbitrator);
    sensitive << read_req << write_req;
  }

  ~fifo()                   	// destructor
  {
    delete [] buf;
  }


  void arbitrator() {
    
    while(1) {

      if (write_flag) {		// write_flag checked first: higher priority
        write_flag = false;
        write_ack.notify();  
      }

      if (read_flag) {		// read_flag checked next: lower priority
        read_flag = false;
        read_ack.notify();  
      }
      
      wait();
    }
  }
  
  
  bool write(T data)
  {
    write_flag = true;
    write_req.notify();
    wait(write_ack);
    
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
    read_flag = true;
    read_req.notify();
    wait(read_ack);
    
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
