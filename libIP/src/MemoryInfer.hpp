/******************************************************************************
* MemoryInfer: Memory inference template class for FPGAs                      *
* Jeferson Santiago da Silva (eng.jefersonsantiago@gmail.com)                 *
******************************************************************************/

#include <iostream>
#include <math.h>
#include <string>
#include <array>
#include "defined_types.h"

#ifndef _MEMORY_HPP_
#define _MEMORY_HPP_

// Template Parameters:
// T_Element: type of the stored element
// N_Elements: Memory size (in number of elements
// Construction parameters:
// instance_name: human readable name of the object instance
// instance_id: unique class instance identifier
template<typename T_Element, uint_64 N_Elements>
class MemoryInfer {
    private:
		typedef ap_uint<numbits(N_Elements)> addrType;
		typedef std::array<T_Element, N_Elements> MemoryType;

        IF_SOFTWARE(const std::string instance_name;)
        uint_16 instance_id;
		MemoryType MemoryArray;
	public:

		// Constructor
        MemoryInfer(IF_SOFTWARE(const std::string instance_name,)
					const uint_16 instance_id) :
					IF_SOFTWARE(instance_name{instance_name},)
					instance_id{instance_id},
					MemoryArray{} {}

		MemoryInfer() : MemoryArray{}, instance_id{65535u} {}

        // General MISC information
        const uint_64 getMemorySize() const {return N_Elements;}
        IF_SOFTWARE(const std::string getInstName()  {return instance_name;})
        uint_16 getInstId() const {return instance_id;};

		// Access functions
        void access (T_Element* wrData, addrType wrAddr, bool wrEn,
					 T_Element* rdData, addrType rdAddr, bool rdEn);

};

// Function Definitions kept here to avoid "undefined reference to" errors
template<typename T_Element, uint_64 N_Elements>
void MemoryInfer<T_Element, N_Elements>
	::access (	T_Element* wrData, addrType wrAddr, bool wrEn,
				T_Element* rdData, addrType rdAddr, bool rdEn) {
#pragma HLS PIPELINE II=1
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS DATA_PACK variable=rdData
#pragma HLS DATA_PACK variable=wrData
//#pragma HLS RESOURCE variable=MemoryArray core=RAM_S2P_BRAM
#pragma HLS DEPENDENCE variable=MemoryArray inter false
	if (wrEn) MemoryArray[wrAddr] = *wrData;
	if (rdEn) *rdData = MemoryArray[rdAddr];
}
#endif //_MEMORY_HPP_
