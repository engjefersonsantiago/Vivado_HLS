/******************************************************************************
* Packet FIFO: one dimension implementation to save memory resources in FPGAs *
* Jeferson Santiago da Silva (eng.jefersonsantiago@gmail.com)                 *
******************************************************************************/

#include <iostream>
#include <math.h>
#include <string>
#include <array>

#include "defined_types.h"
#include "pktBasics.hpp"

#ifndef _PKT_HEADER_HPP_
#define _PKT_HEADER_HPP_
// Template Parameters:
// N_Size: Header size in bytes
// N_Fields: Number of fields
// T_Key: key type
// N_Key: Number of keys to match
// T_DataBus: Data bus type
// N_BusSize: Data bus size in bits
// N_MaxPktSize: Max packet size in bytes
// Construction parameters:
// instance_name: human readable name of the object instance
// instance_id: unique class instance identifier
template<uint_16 N_Size, uint_16 N_Fields, typename T_Key, uint_16 N_Key, typename T_DataBus, uint_16 N_BusSize, uint_16 N_MaxPktSize>
class Header {
    private:

// Defines here to fix template parameters passage
#define HEADER_SIZE_IN_BITS		bytes2Bits(N_Size)
#define PKT_SIZE_IN_BITS		bytes2Bits(N_MaxPktSize)
#define DATA_STUFF_SIZE			(HEADER_SIZE_IN_BITS < N_BusSize \
								? N_BusSize - mod(uint_16(HEADER_SIZE_IN_BITS), N_BusSize) \
								: N_BusSize - mod(N_BusSize, uint_16(HEADER_SIZE_IN_BITS)))
#define RECEIVED_MAX_WORDS		(mod(HEADER_SIZE_IN_BITS, N_BusSize) == 0 \
								? HEADER_SIZE_IN_BITS/N_BusSize \
								: (HEADER_SIZE_IN_BITS/N_BusSize + 1))
#define RECEIVED_WORDS_SIZE		Log2(RECEIVED_MAX_WORDS)
#define RECEIVED_BITS_SIZE		Log2(PKT_SIZE_IN_BITS)

#ifndef __SYNTHESIS__
		const std::string instance_name;
#endif
        const uint_16 instance_id;

		HeaderFormat<N_Fields, T_Key, N_Key> HeaderLayout;

		ap_uint<HEADER_SIZE_IN_BITS> ExtractedHeader;
		bool HeaderIdle;
		bool HeaderDone;
		bool HeaderException;

		uint_16 NextHeader;
		bool NextHeaderValid;

		// This macro is to enable implementation of the PHV using a single register accumulator or a shift-regiter (implemented as array)
		// Weird results here: the latter approach reduces area (-20% LUTs/FFs)!! 
#define ARRAY_FOR_SR 1

#if not ARRAY_FOR_SR
		//Necessary to take the array size (synthesis recursion errors Log2)
		std::array<bool, RECEIVED_WORDS_SIZE> dummyWordSize;	
		ap_uint<N_BusSize*RECEIVED_MAX_WORDS> ExtractedHeaderFull;
#else
		std::array<ap_uint<N_BusSize*RECEIVED_MAX_WORDS>, RECEIVED_WORDS_SIZE> ExtractedHeaderPipe;
#endif
		ap_uint<RECEIVED_WORDS_SIZE> receivedWords;
		ap_uint<RECEIVED_BITS_SIZE> receivedBits;

		//Necessary to take the array size (synthesis recursion errors Mod)
		std::array<bool, DATA_STUFF_SIZE> dummydataStuffSize;		
		
		T_DataBus DataOut;
	public:
        // Constructor
        Header(	
#ifndef __SYNTHESIS__
				const std::string instance_name,
#endif
				const uint_16 instance_id,
				const HeaderFormat<N_Fields, T_Key, N_Key>& HLayout) :
#ifndef __SYNTHESIS__
				instance_name{instance_name},
#endif
				instance_id{instance_id}
				/*, HeaderLayout{HLayout}*/	// Why this does not work?
		{
			for (auto it = 0; it < HLayout.Fields.size(); ++it) {
				HeaderLayout.Fields[it].Offset = HLayout.Fields[it].Offset;
				HeaderLayout.Fields[it].Length = HLayout.Fields[it].Length;
#ifndef __SYNTHESIS__
				HeaderLayout.Fields[it].FieldName = HLayout.Fields[it].FieldName;
#endif
			}
			HeaderLayout.Key = HLayout.Key;
			HeaderLayout.KeyLocation = HLayout.KeyLocation;
#ifndef __SYNTHESIS__
			HeaderLayout.HeaderName = HLayout.HeaderName;
#endif
		}

        // General MISC information
        const uint_16 getHeaderSize() const {return N_Size;}
        const uint_16 getFieldNum() const {return HeaderLayout.Fields.size();}
        const uint_16 getOutStuffBits() const {return dummydataStuffSize.size();}
#ifndef __SYNTHESIS__
        void printFields() {
			for (auto it : HeaderLayout.Fields)
				std::cout << "Field: " << it.FieldName << std::endl;
		}
        void printNextHeaders() {
			for (auto it : HeaderLayout.Key)
				std::cout << "Key: " << it.KeyVal << " Next Header: " << it.NextHeaderName << std::endl;
		}
		const std::string getInstName() const {return instance_name;};
#endif
        uint_16 getInstId() const {return instance_id;};

		void stateTransition (T_DataBus DataIn, ap_uint<RECEIVED_BITS_SIZE> receivedBits, bool* NextHeaderValid);
		void ExtractFields (T_DataBus DataIn, ap_uint<RECEIVED_WORDS_SIZE> receivedWords, bool* HeaderDone, ap_uint<HEADER_SIZE_IN_BITS>* PHV);
		void HeaderAnalysis (T_DataBus DataIn, bool HeaderStart, bool HeaderFinish, ap_uint<HEADER_SIZE_IN_BITS>* PHV, bool* PHVDone);

};

// FIFO access function
template<uint_16 N_Size, uint_16 N_Fields, typename T_Key, uint_16 N_Key, typename T_DataBus, uint_16 N_BusSize, uint_16 N_MaxPktSize>
void Header<N_Size, N_Fields, T_Key, N_Key, T_DataBus, N_BusSize, N_MaxPktSize>
	::stateTransition (T_DataBus DataIn, ap_uint<RECEIVED_BITS_SIZE> receivedBits, bool* NextHeaderValid)
{
#pragma HLS PIPELINE II=1
#pragma HLS UNROLL

	// Combinational logic
	// Constant would be nice here for the mask, but template does not work
	T_Key DataInMask = (1 << HeaderLayout.KeyLocation.second) - 1;
	auto tmpReceivedBits = receivedBits + N_BusSize;
	auto tmpShiftVal = HeaderLayout.KeyLocation.first - receivedBits;
	T_Key tmpKeyVal = T_Key(DataIn >> tmpShiftVal) & DataInMask;

	HeaderException = false;

	if ((tmpReceivedBits >= HeaderLayout.KeyLocation.first) && !*NextHeaderValid) {
		HeaderException = true;
		loop_key: for (auto it : HeaderLayout.Key) {
			if (it.KeyVal == tmpKeyVal & it.KeyMask) {
#ifndef __SYNTHESIS__
				std::cout << "Found a valid transition from " << instance_name << " to "\
							<< it.NextHeaderName << ". Key: " << it.KeyVal << std::endl;
#else
				std::cout << "Found a valid transition from " << instance_id << " to "\
							<< it.NextHeader << ". Key: " << it.KeyVal << std::endl;
#endif
				NextHeader = it.NextHeader;
				*NextHeaderValid = true;
				HeaderException	= false;
				break;
			}
		}
		if (HeaderException) {
			std::cout << "Header transition not found. Aborting processing" << std::endl;
		}
	}

};

template<uint_16 N_Size, uint_16 N_Fields, typename T_Key, uint_16 N_Key, typename T_DataBus, uint_16 N_BusSize, uint_16 N_MaxPktSize>
void Header<N_Size, N_Fields, T_Key, N_Key, T_DataBus, N_BusSize, N_MaxPktSize>
	::ExtractFields (T_DataBus DataIn, ap_uint<RECEIVED_WORDS_SIZE> receivedWords, bool* HeaderDone, ap_uint<HEADER_SIZE_IN_BITS>* PHV)
{
#pragma HLS PIPELINE II=1
	if (!*HeaderDone) {
#if not ARRAY_FOR_SR
		if (receivedWords < dummyWordSize.size()) {
#else
		if (receivedWords < ExtractedHeaderPipe.size()) {
#endif
			ap_uint<N_BusSize*RECEIVED_MAX_WORDS> DataInShift = 
				(ap_uint<N_BusSize*RECEIVED_MAX_WORDS>(DataIn) << (N_BusSize*receivedWords));
			if (receivedWords == 0) {
#if not ARRAY_FOR_SR
				ExtractedHeaderFull = DataIn;
#else
				ExtractedHeaderPipe[0] = DataIn;
#endif		
			} else {
#if not ARRAY_FOR_SR
				ExtractedHeaderFull |= DataInShift;
#else
				ExtractedHeaderPipe[receivedWords] = 
					ExtractedHeaderPipe[receivedWords - 1] | DataInShift;
#endif		
			}
		} else {
#if not ARRAY_FOR_SR
			*PHV = ExtractedHeaderFull;
#else
			*PHV = ExtractedHeaderPipe[receivedWords - 1];
#endif		
			*HeaderDone = true;
			std::cout << "Extracted: " << N_Size << " Bytes. PHV: " << *PHV << std::endl;
		}
	}
}

template<uint_16 N_Size, uint_16 N_Fields, typename T_Key, uint_16 N_Key, typename T_DataBus, uint_16 N_BusSize, uint_16 N_MaxPktSize>
void Header<N_Size, N_Fields, T_Key, N_Key, T_DataBus, N_BusSize, N_MaxPktSize>
	::HeaderAnalysis (T_DataBus DataIn, bool HeaderStart, bool HeaderFinish, ap_uint<HEADER_SIZE_IN_BITS>* PHV, bool* PHVDone)
{
#pragma HLS DEPENDENCE variable=HeaderIdle false
#pragma HLS DEPENDENCE variable=receivedBits false
#pragma HLS DEPENDENCE variable=receivedWords false
#pragma HLS DEPENDENCE variable=NextHeaderValid false
#pragma HLS DEPENDENCE variable=HeaderException false
#pragma HLS DEPENDENCE variable=HeaderDone false
#pragma HLS PIPELINE II=1

	if (HeaderStart) {
		std::cout << "Received a new packet" << std::endl;
		HeaderIdle = false;
		receivedWords = 0;
		receivedBits = 0;
		NextHeaderValid = false;
		HeaderDone = false;
	}

	std::cout << "Packet: " << DataIn << std::endl;
	std::cout << "Stuff: " << getOutStuffBits() << std::endl;
	
	if (!HeaderIdle) {

		// State transition evaluation
		stateTransition(DataIn, receivedBits, &NextHeaderValid);

		// Header extration
		ExtractFields(DataIn, receivedWords, &HeaderDone, PHV);
		*PHVDone = HeaderDone;

		++receivedWords;
		receivedBits+=N_BusSize;
		if (HeaderFinish /*|| HeaderException*/) {
			HeaderIdle = true;
			std::cout << "Packet has finished" << std::endl;
		}
	}
}
#endif //_PKT_HEADER_HPP_
