// ELEC 466 ASSIGNMENT 2 QUESTION 1
// AUTHOR: Graham Ball | V00732898 | June 2017
//
// MEMORY MODULE REWRITE

// Including the SystemC library
#include "systemc.h"

// Defining the read and write memory access commands
#define NOP   0 // Defining NOP Command Value
#define RDBYT 1 // Defining NOP Command Value
#define WTBYT 2 // Defining NOP Command Value
#define WTBLK 3 // Defining NOP Command Value

// **DEFINE WHAT THIS IS
static sc_logic Z[8] = {SC_LOGIC_Z, SC_LOGIC_Z, SC_LOGIC_Z, SC_LOGIC_Z, SC_LOGIC_Z, SC_LOGIC_Z, SC_LOGIC_Z, SC_LOGIC_Z};

SC_MODULE(mem_ctrl) {

// Declaring Ports at the start of the module
  sc_in < sc_uint<2> > comm;	// Com port is set as input only as type sc_unint2
  sc_in < sc_uint<8> > addr;	// Addr port is set as input only as type sc_unint8
  sc_inout <sc_lv<8> > data;	// Data port is set as bidirectional as type sc_lv8 <-- **DEFINE WHAT THIS IS
// Declaring clocks
  sc_in_clk clk;		// clk is declared as an input
// Declaring Flag ports
  sc_in <bool> reset;		// Reset port is set as input only of type bool
  sc_in <bool> new_comm;	// new_comm port is set as input only of type bool
  sc_out <bool> complete;	// Complete port is set as input only of type bool
// Declaring memory space
  sc_lv <8> memory[256];	// memory space

// Function mem_process()
  void mem_process() {	
	// Declaring local variables
  	sc_uint <2> comm_s;		// local variable: command sample
  	sc_uint <8> addr_s;		// local variable: address sample
  	sc_lv <8> data_s;		// local variable: data sample
	
	// Continuous while loop is entered
	while (true) {
		
	  	if (reset.read() == true) complete.write(false);
	  	else {
	    		if (new_comm.read() == true) { 
			comm_s = comm.read();
			switch (comm_s) {
		  	case NOP: 
		  		cout 	<< "@" << sc_time_stamp() 
	  		     		<< ": NOP" << endl;
		  		wait();
		  		break;
		  	case RDBYT:
		     		addr_s = addr.read();
				data_s = memory[addr_s];		  	
				cout 	<< "@" << sc_time_stamp() 
	  		     		<< ": RDBYT, address = " << addr_s 
			     		<< ", data = " << data_s << endl;
		  		wait();
				data.write(data_s);
		  		break;
		  	case WTBYT:
		  		addr_s = addr.read();
				data_s = data.read();
				cout 	<< "@" << sc_time_stamp() 
	  		     		<< ": WTBYT, address = " << addr_s 
			     		<< ", data = " << data_s << endl;
		  		wait(); 
		  		memory[addr_s] = data_s;
				break;
		  	case WTBLK:
		  		addr_s = addr.read();
				data_s = data.read();
				cout 	<< "@" << sc_time_stamp() 
	  		     		<< ": WTBLK-0, address = " << addr_s 
			     		<< ", data = " << data_s << endl;
		  		wait();
				memory[addr_s] = data_s;
				addr_s++;
				data_s = data.read();
				cout 	<< "@" << sc_time_stamp() 
	  		     		<< ": WTBLK-1, address = " << addr_s 
			     		<< ", data = " << data_s << endl;
		  		wait();
				memory[addr_s] = data_s;
				addr_s++;
				data_s = data.read();
				cout 	<< "@" << sc_time_stamp() 
	  		     		<< ": WTBLK-2, address = " << addr_s 
			     		<< ", data = " << data_s << endl;
		  		wait();
				memory[addr_s] = data_s;
				addr_s++;
				data_s = data.read();
				cout 	<< "@" << sc_time_stamp() 
	  		     		<< ": WTBLK-3, address = " << addr_s 
			     		<< ", data = " << data_s << endl;
		  		wait();
				memory[addr_s] = data_s;			     
		  		break;
		  	default: 
		  		cout << "Illegal command : " << comm_s << endl;
		  		break;
		}
		complete.write(true);
		while (new_comm.read() == true) {
			if (reset.read() == true) break;
			wait();
		}
		if (comm_s == RDBYT) data.write(Z);	// stop driving
		complete.write(false);
	    }
	  }
	  wait();
	}
  }

  SC_CTOR(mem_ctrl) { 
	SC_CTHREAD(mem_process, clk.pos()); 
  }

};


SC_MODULE(mem_testbench) {

  sc_out < sc_uint<2> > comm;
  sc_out < sc_uint<8> > addr;
  sc_inout < sc_lv<8> > data;

  sc_in_clk clk;
  sc_out <bool> reset;
  sc_out <bool> new_comm;
  sc_in <bool> complete;
  
  void handshake() {
  	new_comm.write(true);	
	while (complete.read() == false) wait();
	
	cout 	<< "@" << sc_time_stamp() 
	  	<< ": complete = " << complete.read() << endl;

	if (comm.read() == NOP);
	else if (comm.read() == RDBYT)
		cout << "*** data read = " << data.read() << endl;
	else 
		data.write(Z);		// stop driving
	
	new_comm.write(false);
	while (complete.read() == true) wait();
	
	cout 	<< "@" << sc_time_stamp() 
	  	<< ": complete = " << complete.read() << endl;
  }
  
  void tb_process() {	

	reset.write(true);
	wait();

	reset.write(false);
	addr.write(64);
	data.write(0);
	comm.write(WTBYT);	
	handshake();	

	addr.write(128);
	data.write(255);
	comm.write(WTBYT);	
	handshake();	
	
	addr.write(64);
	comm.write(RDBYT);	
	handshake();		
		
	addr.write(128);
	comm.write(RDBYT);	
	handshake();
		
	addr.write(0);
	data.write(129);
	comm.write(WTBLK);
	handshake();	

	addr.write(0);
	comm.write(RDBYT);	
	handshake();

	addr.write(3);
	comm.write(RDBYT);	
	handshake();

	comm.write(NOP);	
	handshake();	
				
  }

  SC_CTOR(mem_testbench) { 
	SC_CTHREAD(tb_process, clk.pos()); 
  }

};


int sc_main(int argc, char* argv[])
{
    // module instances
    mem_ctrl MC("MC");
    
    mem_testbench TB("TB");

    sc_clock clk("clk", 10, SC_NS);

    sc_signal <bool> reset, new_comm, complete;
    sc_signal < sc_uint<2> > comm;
    sc_signal < sc_uint<8> > addr;
    sc_signal_rv <8> data;
    // _rv needed because of multiple drivers
  

    // interconnect
    MC.clk(clk);
    MC.reset(reset);
    MC.data(data);
    MC.addr(addr);
    MC.comm(comm);
    MC.new_comm(new_comm);
    MC.complete(complete);

    TB.clk(clk);
    TB.reset(reset);
    TB.data(data);
    TB.addr(addr);
    TB.comm(comm);
    TB.new_comm(new_comm);
    TB.complete(complete);

    // start the simulation
    sc_start(5000, SC_NS);

    return 0;
}
