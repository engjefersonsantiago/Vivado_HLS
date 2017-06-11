/******************************************************************************
* Packet FIFO: one dimension implementation to save memory resources in FPGAs *
* Jeferson Santiago da Silva (eng.jefersonsantiago@gmail.com)                 *
******************************************************************************/

#include <iostream>
#include <math.h>
#include <string>
#include <array>

#include "defined_types.h"
#include "MemoryInfer.hpp"

#ifndef _PKT_FIFO_HPP_
#define _PKT_FIFO_HPP_
// Template Parameters:
// T_FifoSize: FIFO size in terms of T_MaxPktSize
// T_FifoElementSize: FIFO data element size in bits
// T_MaxPktSize: Max packet size
// T_MinPktSize: Min packet size
// Construction parameters:
// instance_name: human readable name of the object instance
// instance_id: unique class instance identifier
template<uint_8 T_FifoSize, uint_16 T_FifoElementSize, uint_16 T_MaxPktSize, uint_16 T_MinPktSize>
class PktFIFO {
    private:

		// Defines here to fix template parameters passage
		static constexpr uint_16 FIFO_SIZE_BYTES = T_FifoSize*T_MaxPktSize;
		static constexpr uint_16 FIFO_ARRAY_SIZE = FIFO_SIZE_BYTES/bits2Bytes(T_FifoElementSize);
		static constexpr uint_16 PKT_CNT_SIZE = numbits(FIFO_SIZE_BYTES/T_MinPktSize);
		static constexpr uint_16 PTR_SIZE = numbits(FIFO_ARRAY_SIZE);

		typedef FifoElement<ap_int<T_FifoElementSize>> FIFOElementType;
		typedef ap_uint<PKT_CNT_SIZE> pktCntType;
		typedef ap_uint<PTR_SIZE> ptrType;

		IF_SOFTWARE(const std::string instance_name;)
        uint_16 instance_id;

		// This is required because Xilinx's GCC does not understand object declation with construction parameters
#if __GNUC_PREREQ(4,8)
		MemoryInfer<FIFOElementType, FIFO_ARRAY_SIZE> InfMem {IF_SOFTWARE("Memory",) 0};
#else
		MemoryInfer<FIFOElementType, FIFO_ARRAY_SIZE> InfMem;
#endif

        pktCntType buffPkt;
        ptrType wrPtr;
        ptrType rdPtr;

        bool Empty;
        bool Full;

    public:

        // Constructor
        PktFIFO(
				IF_SOFTWARE(const std::string instance_name,)
				const uint_16 instance_id) :
				IF_SOFTWARE(instance_name{instance_name},)
				instance_id{instance_id},
                buffPkt {0},
				wrPtr {0},
				rdPtr {0},
				Empty {true},
				Full {false} {}

		// General MISC information
        const uint getFIFOSize() const {return InfMem.getMemorySize()*getElementSize();}
        const uint getElementSize() const {return T_FifoElementSize;}
        IF_SOFTWARE(const std::string getInstName() const {return instance_name;})
        uint_16 getInstId() const {return instance_id;}

		// Occupancy status
        void BuffPktInc() {buffPkt++;}
        void BuffPktDec() {buffPkt--;}
        pktCntType getBuffPkt() {return buffPkt;}

        // Pointer Manipulation
        ptrType getPtrVal(ptrType *ptr) {return *ptr;}
        bool PtrDiff(ptrType ptrA, ptrType ptrB) {return ptrA == ptrB;}
        ptrType getPtrDiffVal(ptrType ptrA, ptrType ptrB) {return std::abs(ptrA - ptrB);} // Modulus: Rewrite
        void incPtr (ptrType *ptr) {(*ptr == FIFO_ARRAY_SIZE - 1) ? *ptr = 0u : *ptr+=1;}

        ptrType incPtr (ptrType ptr) {return (ptr == FIFO_ARRAY_SIZE - 1) ? (ptrType)0u : ptr+=1;}

        // Data access functions
        void memory (FIFOElementType *wrData, ptrType wrPtr, bool wrEn,
					FIFOElementType *rdData, ptrType rdPtr, bool rdEn);
        bool write (FIFOElementType *wrData);
        bool read (FIFOElementType *rdData);

        //std::pair<bool, bool> access (FIFOElementType *wrData, bool wrEn, FIFOElementType *rdData, bool rdEn);
        void access (FIFOElementType *wrData, bool wrEn, FIFOElementType *rdData, bool rdEn);

        // Status
        std::pair<bool,bool> getFIFOSts() {return {Empty, Full};}
};

// FIFO write function
template<uint_8 T_FifoSize, uint_16 T_FifoElementSize, uint_16 T_MaxPktSize, uint_16 T_MinPktSize>
bool PktFIFO<T_FifoSize, T_FifoElementSize, T_MaxPktSize, T_MinPktSize>
	::write(FIFOElementType *wrData) {
	auto FIFOWrData = *wrData;
#pragma HLS DATA_PACK variable=wrData
#pragma HLS DATA_PACK variable=FIFOWrData
#pragma HLS DEPENDENCE variable=wrData inter WAR false
#pragma HLS DEPENDENCE variable=FIFOWrData inter WAR false

	if (!Full) {
		Empty = false;
		Full = PtrDiff(incPtr(wrPtr), rdPtr);
		incPtr(&wrPtr);
        if (wrData->PacketIni) BuffPktInc();
        return true;
    } else {
        std::cout << "There is no available room at the FIFO. Data is not being stored" << std::endl;
        return false;
    }
}

// FIFO read function
template<uint_8 T_FifoSize, uint_16 T_FifoElementSize, uint_16 T_MaxPktSize, uint_16 T_MinPktSize>
bool PktFIFO<T_FifoSize, T_FifoElementSize, T_MaxPktSize, T_MinPktSize>
	::read(FIFOElementType *rdData) {
	FIFOElementType tmpRdData;
#pragma HLS DATA_PACK variable=rdData
#pragma HLS DATA_PACK variable=tmpRdData
#pragma HLS DEPENDENCE variable=rdData inter WAR false
#pragma HLS DEPENDENCE variable=tmpRdData inter WAR false

	if (!Empty) {
		Full = false;
		Empty = PtrDiff(incPtr(rdPtr), wrPtr);
		incPtr(&rdPtr);
        if (rdData->PacketEnd) BuffPktDec();
        return true;
    } else {
        std::cout << "The FIFO is Empty. No data to be read" << std::endl;
        return false;
    }
}

// FIFO access function
template<uint_8 T_FifoSize, uint_16 T_FifoElementSize, uint_16 T_MaxPktSize, uint_16 T_MinPktSize>
void PktFIFO<T_FifoSize, T_FifoElementSize, T_MaxPktSize, T_MinPktSize>
	::access (FIFOElementType *wrData, bool wrEn, FIFOElementType *rdData, bool rdEn) {

#pragma HLS PIPELINE II=1
#pragma HLS DEPENDENCE variable=InfMem inter WAR false
#pragma HLS DEPENDENCE variable=wrData inter WAR false
#pragma HLS DEPENDENCE variable=rdData inter WAR false
#pragma HLS DEPENDENCE variable=Full inter false
#pragma HLS DEPENDENCE variable=Empty inter false
#pragma HLS DEPENDENCE variable=buffPkt inter false
#pragma HLS DATA_PACK variable=wrData
#pragma HLS DATA_PACK variable=rdData

	FIFOElementType tmpRdData;
#pragma HLS DATA_PACK variable=tmpRdData
	auto tmpWrEn = wrEn && !Full;
	auto tmpRdEn = rdEn && !Empty;
	InfMem.access(wrData, wrPtr, tmpWrEn, &tmpRdData, rdPtr, tmpRdEn);
	*rdData = tmpRdData;
	if (wrEn) write(wrData);
	if (rdEn) read(&tmpRdData);
}
#endif //_PKT_FIFO_HPP_
