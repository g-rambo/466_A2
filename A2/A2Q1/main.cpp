// ELEC 466 ASSIGNMENT 2 QUESTION 1
// AUTHOR: Graham Ball | V00732898 | June 2017
//
// MEMORY MODULE REWRITE
// STATUS: INCOMPLETE... Didn't have quite enough time to finish this assignment but started it anyways
// - Testbench module is written but doesn't test all commands
// - main is written and complete
// - NOP was replace with WRTBLK with the intention of altering it to RDBLK but time ran out

// Including the SystemC library
#include "systemc.h"

// Defining the read and write memory access commands
#define RDBLK   0 // Defining NOP Command Value
#define RDBYT 1 // Defining NOP Command Value
#define WTBYT 2 // Defining NOP Command Value
#define WTBLK 3 // Defining NOP Command Value

// **DEFINE WHAT THIS IS
static sc_logic Z[8] = {SC_LOGIC_Z, SC_LOGIC_Z, SC_LOGIC_Z, SC_LOGIC_Z, SC_LOGIC_Z, SC_LOGIC_Z, SC_LOGIC_Z, SC_LOGIC_Z};


//************************************************************************************
//************************************************************************************
//************************************************************************************
// MEMORY MODULE

SC_MODULE(mem_ctrl) {

// Declaring Ports at the start of the module
	sc_in < sc_uint<2> > comm;	// Com port is set as input only as type sc_unint2
	sc_in < sc_uint<8> > addr;	// Addr port is set as input only as type sc_unint8
	sc_inout <sc_lv<8> > data;	// Data port is set as bidirectional as type sc_lv8 <-- **DEFINE WHAT THIS IS
	// Declaring clocks
	sc_in_clk clk;			// clk is declared as an input
	// Declaring Flag ports
	sc_in <bool> reset;		// Reset port is set as input only of type bool
	sc_in <bool> new_comm;		// new_comm port is set as input only of type bool
	sc_out <bool> complete;		// Complete port is set as input only of type bool
	// Declaring memory space
	sc_lv <8> memory[256];		// memory space
	////////////////////////////
	// FUNCTION mem_process() //
	////////////////////////////
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
		  				case RDBLK: 
		  					addr_s = addr.read();
							data_s = data.read();
							cout 	<< "@" << sc_time_stamp() 
	  		     					<< ": WTBLK-0, address = " << addr_s 
			     					<< ", data = " << data_s << endl;
		  					wait();		// Wait for one clock cycle do nothing
							memory[addr_s] = data_s;

							addr_s++;
							data_s = data.read();
							cout 	<< "@" << sc_time_stamp() 
	  		     				<< ": WTBLK-1, address = " << addr_s 
			     				<< ", data = " << data_s << endl;
		  					wait();		// Wait for one clock cycle do nothing
							memory[addr_s] = data_s;

							addr_s++;
							data_s = data.read();
							cout 	<< "@" << sc_time_stamp() 
	  		     					<< ": WTBLK-2, address = " << addr_s 
			     					<< ", data = " << data_s << endl;
		  					wait();		// Wait for one clock cycle do nothing
							memory[addr_s] = data_s;

							addr_s++;
							data_s = data.read();
							cout 	<< "@" << sc_time_stamp() 
	  		     					<< ": WTBLK-3, address = " << addr_s 
			     					<< ", data = " << data_s << endl;
		  					wait();		// Wait for one clock cycle do nothing
							memory[addr_s] = data_s;			     
		  					break;

		  				case RDBYT:
		     					addr_s = addr.read();
							data_s = memory[addr_s];		  	
							cout 	<< "@" << sc_time_stamp() 
	  		     					<< ": RDBYT, address = " << addr_s 
			     					<< ", data = " << data_s << endl;
		  					wait();		// Wait for one clock cycle do nothing
							data.write(data_s);
		  					break;
		  				case WTBYT:
		  					addr_s = addr.read();
							data_s = data.read();
							cout 	<< "@" << sc_time_stamp() 
	  		     					<< ": WTBYT, address = " << addr_s 
			     					<< ", data = " << data_s << endl;
		  					wait();		// Wait for one clock cycle do nothing
		  					memory[addr_s] = data_s;
							break;
		  				case WTBLK:
		  					addr_s = addr.read();
							data_s = data.read();
							cout 	<< "@" << sc_time_stamp() 
	  		     					<< ": WTBLK-0, address = " << addr_s 
			     					<< ", data = " << data_s << endl;
		  					wait();		// Wait for one clock cycle do nothing
							memory[addr_s] = data_s;
							addr_s++;
							data_s = data.read();
							cout 	<< "@" << sc_time_stamp() 
	  		     				<< ": WTBLK-1, address = " << addr_s 
			     				<< ", data = " << data_s << endl;
		  					wait();		// Wait for one clock cycle do nothing
							memory[addr_s] = data_s;
							addr_s++;
							data_s = data.read();
							cout 	<< "@" << sc_time_stamp() 
	  		     					<< ": WTBLK-2, address = " << addr_s 
			     					<< ", data = " << data_s << endl;
		  					wait();		// Wait for one clock cycle do nothing
							memory[addr_s] = data_s;
							addr_s++;
							data_s = data.read();
							cout 	<< "@" << sc_time_stamp() 
	  		     					<< ": WTBLK-3, address = " << addr_s 
			     					<< ", data = " << data_s << endl;
		  					wait();		// Wait for one clock cycle do nothing
							memory[addr_s] = data_s;			     
		  					break;
		  				default: 
		  					cout << "Illegal command : " << comm_s << endl;
		  					break;
					}
					complete.write(true);
					while (new_comm.read() == true) {
						if (reset.read() == true) break;
						wait();		// Wait for one clock cycle do nothing
					}
					if (comm_s == RDBYT) data.write(Z);	// stop driving
					complete.write(false);
	    			}
	  		}

	  	wait();		// Wait for one clock cycle do nothing
		}
	} // End of mem_process()

	/////////////////
	// CONSTRUCTOR //
	/////////////////
	SC_CTOR(mem_ctrl) { 
		SC_CTHREAD(mem_process, clk.pos());	// Instance of mem_process created and relevant clock edge assigned
  	}

}; // End of mem_ctrl module


//************************************************************************************
//************************************************************************************
//************************************************************************************
// TESTBENCH MODULE

SC_MODULE(mem_testbench) {

	sc_out < sc_uint<2> > comm;
	sc_out < sc_uint<8> > addr;
	sc_inout < sc_lv<8> > data;

	sc_in_clk clk;
	sc_out <bool> reset;
	sc_out <bool> new_comm;
	sc_in <bool> complete;
	
	/////////////////////////
	// Validation FUNCTION //
	/////////////////////////
	void Output_Validation() {
  		new_comm.write(true);				// Set new_comm flag to true
		while (complete.read() == false) wait();	// wait until complete is set to true
		// When complete is set to true proceed...
		// Output the following:
		cout 	<< "@" << sc_time_stamp()		
	  		<< ": complete = " << complete.read() << endl;
		// Depending on comm state
		if (comm.read() == RDBLK);		// If the command is RDBLK
		else if (comm.read() == RDBYT)		// If the command is RDBYT
			// Output the following:
			cout << "*** data read = " << data.read() << endl;
		else					
			data.write(Z);			// Else: STOP
	
		new_comm.write(false);			// Set the new_comm flag to false
	while (complete.read() == true) wait();		// Wait until complete flag is false
	// When complete is set to false proceed...
	// Output the Following:
	cout 	<< "@" << sc_time_stamp() 
	  	<< ": complete = " << complete.read() << endl;
	} // End of Handshake function

	/////////////////////////
	// TB_PROCESS FUNCTION //
	/////////////////////////
	void tb_process() {	

		reset.write(true);	// Set reset flag to true
		wait();			// Wait for one clock cycle do nothing
		reset.write(false);	// Set reset flag to false

		// *** PROCESS 1 ***		
		addr.write(0);		// Set addr to 64
		data.write(0);		// Set data to 0
		comm.write(WTBYT);	// Set the command to WTBYT
		Output_Validation();	// Call validation function to print command results
		// *** PROCESS 2 ***
		addr.write(64);	// Set addr to 128
		data.write(255);	// Set data to 255
		comm.write(WTBYT);	// Set the command to WTBYT
		Output_Validation();	// Call validation function to print command results
				
  	} // End of tb_process

	/////////////////
	// CONSTRUCTOR //
	/////////////////
	SC_CTOR(mem_testbench) { 
		SC_CTHREAD(tb_process, clk.pos());	// instance of tb_process is created and relevant clock edge assigned
	} // End of constructor

}; // End of test_bench module


//************************************************************************************
//************************************************************************************
//************************************************************************************
// MAIN EXECUTABLE FUNCTION

int sc_main(int argc, char* argv[])
{
	// Module Instances
	mem_ctrl MC("MC");				// Memory module instance created
	mem_testbench TEST("TEST");			// TestBench module instance created
	sc_clock clk("clk", 10, SC_NS);			// CLock instance created

	// Internal signals
	sc_signal <bool> reset, new_comm, complete;	// reset, new command, and complete flag signals created
	sc_signal < sc_uint<2> > comm;			// command type signal created
	sc_signal < sc_uint<8> > addr;			// memory address signal created
	sc_signal_rv <8> data;				// _rv needed because of multiple drivers
  

	// Memory Module Interconnect
	MC.clk(clk);
	MC.reset(reset);
	MC.data(data);
	MC.addr(addr);
	MC.comm(comm);
	MC.new_comm(new_comm);
	MC.complete(complete);

	// TestBench Module Interconnect
	TEST.clk(clk);
	TEST.reset(reset);
	TEST.data(data);
	TEST.addr(addr);
	TEST.comm(comm);
	TEST.new_comm(new_comm);
	TEST.complete(complete);

	// start the simulation
	sc_start(5000, SC_NS);

	return 0;
}
