/******************************************************************************
* Packet Header: Header basic class for FPGA packet processing                *
* Jeferson Santiago da Silva (eng.jefersonsantiago@gmail.com)                 *
******************************************************************************/

#include <iostream>
#include <math.h>
#include <string>
#include <array>

#include "defined_types.h"
#include "pktBasics.hpp"

// This macro is to enable implementation of the PHV using a single register accumulator or a shift-regiter (implemented as array)
// Weird results here: the latter approach reduces area!!
#define ARRAY_FOR_SR 1	// Impossible to reach latency 1 if 0

// Define to enable lookup implementation for the extraction bit shifts
#define LOOKUPTABLE_FOR_SHIFT_CALC 1	//For "small" headers reduces area and improves performance

// Macro to define the stuff data out size and the key shift
#define STUFF_AND_SHIFT_SIZE_OP(A, B, C) ((uint_16(A) < uint_16(B)) ? uint_16(B - C) : uint_16(B - uint_16(C%B)))

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
		static constexpr uint_16 HEADER_SIZE_IN_BITS = bytes2Bits(N_Size);
		static constexpr uint_16 PKT_SIZE_IN_BITS = bytes2Bits(N_MaxPktSize);
		static constexpr uint_16 DATAOUT_STUFF_SIZE = STUFF_AND_SHIFT_SIZE_OP(HEADER_SIZE_IN_BITS, N_BusSize, HEADER_SIZE_IN_BITS);
		static constexpr uint_16 DATAOUT_STUFF_SHIFT = N_BusSize - DATAOUT_STUFF_SIZE;
		static constexpr uint_16 RECEIVED_MAX_WORDS = (HEADER_SIZE_IN_BITS%N_BusSize == 0 \
														? HEADER_SIZE_IN_BITS/N_BusSize \
														: (HEADER_SIZE_IN_BITS/N_BusSize + 1));
		static constexpr uint_16 RECEIVED_WORDS_SIZE = numbits(RECEIVED_MAX_WORDS);
		static constexpr uint_16 RECEIVED_BITS_SIZE = numbits(PKT_SIZE_IN_BITS);

		//Used types
		typedef ap_uint<HEADER_SIZE_IN_BITS> ExtractedHeaderType;
		typedef std::array<ap_uint<HEADER_SIZE_IN_BITS>, RECEIVED_MAX_WORDS> ExtractedHeaderPipeType;
		typedef ap_uint<RECEIVED_WORDS_SIZE> receivedWordsType;
		typedef ap_uint<RECEIVED_BITS_SIZE> receivedBitsType;

		IF_SOFTWARE(const std::string instance_name;)
		const uint_16 instance_id;

		const HeaderFormat<N_Fields, T_Key, N_Key> HeaderLayout;

		bool HeaderIdle;
		bool HeaderDone;
		bool HeaderException;

		uint_16 NextHeader;
		bool NextHeaderValid;

		ExtractedHeaderType ExtractedHeader;
		ExtractedHeaderPipeType ExtractedHeaderPipe;
		receivedWordsType receivedWords;
		receivedBitsType receivedBits;

		PacketData<N_BusSize> PacketOutReg;

		const uint_16 stateTransShiftVal;
		std::array<int_16, RECEIVED_MAX_WORDS> shiftNumLookup;

	public:
        // Constructor
        Header(
				IF_SOFTWARE(const std::string instance_name,)
				const uint_16 instance_id,
				const HeaderFormat<N_Fields, T_Key, N_Key>& HLayout) :
				IF_SOFTWARE(instance_name{instance_name},)
				instance_id{instance_id},
				HeaderLayout(HLayout),
				//PacketOutReg{0, 0, false, false, 0},	// Vivado does not like it!!
				HeaderIdle{true},
				receivedWords{0},
				receivedBits{0},
				NextHeaderValid{false},
				HeaderDone{false},
				NextHeader{false},
				stateTransShiftVal{	STUFF_AND_SHIFT_SIZE_OP(HEADER_SIZE_IN_BITS, \
									N_BusSize,
									(HLayout.KeyLocation.first + HLayout.KeyLocation.second))}
		{
			for (int it = 0; it < shiftNumLookup.size(); ++it)
				shiftNumLookup[it] = HEADER_SIZE_IN_BITS - N_BusSize*(it+1);
		}

        // General MISC information
        const uint_16 getHeaderSize() const {return N_Size;}
        const uint_16 getFieldNum() const {return HeaderLayout.Fields.size();}
        const uint_16 getOutStuffBits() const {return DATAOUT_STUFF_SIZE;}
        IF_SOFTWARE(void printFields() {
			for (auto it : HeaderLayout.Fields)
				std::cout << "Field: " << it.FieldName << std::endl;
		})
        IF_SOFTWARE(void printNextHeaders() {
			for (auto it : HeaderLayout.Key)
				std::cout << "Key: " << std::hex << it.KeyVal << " Next Header: " << it.NextHeaderName << std::endl;
		})
		IF_SOFTWARE(const std::string getInstName() const {return instance_name;})
        uint_16 getInstId() const {return instance_id;};

		void stateTransition(PacketData<N_BusSize> PacketIn, receivedBitsType receivedBits, bool* NextHeaderValid, uint_16* NextHeader);
		void ExtractFields(PacketData<N_BusSize> PacketIn, receivedWordsType receivedWords, bool* HeaderDone, PHVData<N_Size>* PHV);
		void HeaderAnalysis(PacketData<N_BusSize> PacketIn, PHVData<N_Size>* PHV, PacketData<N_BusSize>* PacketOut);

};

// FIFO access function
template<uint_16 N_Size, uint_16 N_Fields, typename T_Key, uint_16 N_Key, typename T_DataBus, uint_16 N_BusSize, uint_16 N_MaxPktSize>
void Header<N_Size, N_Fields, T_Key, N_Key, T_DataBus, N_BusSize, N_MaxPktSize>
	::stateTransition (PacketData<N_BusSize> PacketIn, receivedBitsType receivedBits, bool* NextHeaderValid, uint_16* NextHeader)
{
#pragma HLS INLINE
#pragma HLS UNROLL
#pragma HLS LATENCY min=1 max=1
//#pragma HLS PIPELINE II=1

	// Combinational logic
	T_Key DataInMask = (1 << HeaderLayout.KeyLocation.second) - 1;
	auto tmpReceivedBits = receivedBits + N_BusSize;
	T_Key tmpKeyVal = T_Key(PacketIn.Data >> stateTransShiftVal) & DataInMask;

	HeaderException = false;
	if (HeaderLayout.LastHeader) {
		std::cout << "Last header. No state transition" << std::endl;
		return;
	}
	if (!*NextHeaderValid && tmpReceivedBits > HeaderLayout.KeyLocation.first) {
		HeaderException = true;
		loop_key: for (auto it : HeaderLayout.Key) {
			if (it.KeyVal == tmpKeyVal & it.KeyMask) {
#ifndef __SYNTHESIS__
				std::cout << "Found a valid transition from " << instance_name << " to "\
							<< it.NextHeaderName << ". Key: " << std::hex << uint(it.KeyVal) << std::endl;
#else
				std::cout << "Found a valid transition from " << instance_id << " to "\
							<< it.NextHeader << ". Key: " << std::hex << uint(it.KeyVal) << std::endl;
#endif
				*NextHeader = it.NextHeader;
				*NextHeaderValid = true;
				HeaderException	= false;
			}
		}
		if (HeaderException) {
			std::cout << "Header transition not found. Aborting processing" << std::endl;
		}
	}
};

template<uint_16 N_Size, uint_16 N_Fields, typename T_Key, uint_16 N_Key, typename T_DataBus, uint_16 N_BusSize, uint_16 N_MaxPktSize>
void Header<N_Size, N_Fields, T_Key, N_Key, T_DataBus, N_BusSize, N_MaxPktSize>
	::ExtractFields(PacketData<N_BusSize> PacketIn, receivedWordsType receivedWords, bool* HeaderDone, PHVData<N_Size>* PHV)
{
#pragma HLS INLINE
#pragma HLS LATENCY min=1 max=1
//#pragma HLS PIPELINE II=1

	auto tmpShiftVal = N_BusSize*(receivedWords+1);	//Implemented as SR not mul
#if LOOKUPTABLE_FOR_SHIFT_CALC
	auto tmpDinlShifted = ExtractedHeaderType(PacketIn.Data) << shiftNumLookup[receivedWords];
#else
	auto tmpDinlShifted = ExtractedHeaderType(PacketIn.Data) << (HEADER_SIZE_IN_BITS - tmpShiftVal);
#endif
	auto tmpDinrShifted = ExtractedHeaderType((PacketIn.Data) >> (N_BusSize - HEADER_SIZE_IN_BITS));
	PHVData<N_Size>  tmpPHV;
	tmpPHV.ID = PacketIn.ID;
	tmpPHV.Valid = *HeaderDone;

	if (!*HeaderDone) {
		if (receivedWords < RECEIVED_MAX_WORDS) {
			if (HEADER_SIZE_IN_BITS > N_BusSize)  {
				if (receivedWords == 0) {
					ExtractedHeaderPipe[0] = ExtractedHeader = tmpDinlShifted;

				} else {
					ExtractedHeader |= tmpDinlShifted;
					ExtractedHeaderPipe[receivedWords] = ExtractedHeaderPipe[receivedWords-1] | tmpDinlShifted;
				}
			} else {
				ExtractedHeader = ExtractedHeaderPipe[0] = tmpDinrShifted;
			}
		} else  {
#if not ARRAY_FOR_SR
			tmpPHV.Data = ExtractedHeader;
#else
			tmpPHV.Data = ExtractedHeaderPipe[receivedWords - 1];
#endif
			tmpPHV.Valid = *HeaderDone = true;
			std::cout << "Extracted: " << std::dec << N_Size << " Bytes. PHV: " << std::hex << PHV->Data << std::endl;
			*PHV = tmpPHV;
		}
	}
}

template<uint_16 N_Size, uint_16 N_Fields, typename T_Key, uint_16 N_Key, typename T_DataBus, uint_16 N_BusSize, uint_16 N_MaxPktSize>
void Header<N_Size, N_Fields, T_Key, N_Key, T_DataBus, N_BusSize, N_MaxPktSize>
	::HeaderAnalysis(PacketData<N_BusSize> PacketIn, PHVData<N_Size>* PHV, PacketData<N_BusSize>* PacketOut)
{
#pragma HLS LATENCY min=1 max=1
#pragma HLS DEPENDENCE variable=HeaderIdle WAR false
#pragma HLS DEPENDENCE variable=NextHeaderValid WAR false
#pragma HLS DEPENDENCE variable=HeaderDone WAR false
#pragma HLS DEPENDENCE variable=NextHeader WAR false

	if (PacketIn.Start && PacketIn.HeaderID == instance_id) {
		std::cout << "Received a new packet" << std::endl;
		HeaderIdle = false;
	}

	std::cout << "Packet Word In: " << std::hex << PacketIn.Data << std::endl;

	if (!HeaderIdle) {

		// Header extration
		ExtractFields(PacketIn, receivedWords, &HeaderDone, PHV);

		// State transition evaluation
		stateTransition(PacketIn, receivedBits, &NextHeaderValid, &NextHeader);

		auto stuffSizeMask = ap_uint<DATAOUT_STUFF_SIZE>((ap_uint<DATAOUT_STUFF_SIZE+1>(1) << DATAOUT_STUFF_SIZE) - 1);
		PacketOut->Data = ((T_DataBus(PacketOutReg.Data) & stuffSizeMask) << DATAOUT_STUFF_SHIFT)
						| T_DataBus(PacketIn.Data) >> DATAOUT_STUFF_SIZE;
		PacketOut->ID = PacketOutReg.ID;
		PacketOut->Start = PacketOutReg.Start & HeaderDone;
		PacketOut->Finish = PacketOutReg.Finish;
		PacketOut->HeaderID = NextHeader;

		std::cout << "Packet Word Out: " << std::hex << PacketOut->Data << std::endl;
		PacketOutReg = PacketIn;

		// Execution control
		if (PacketIn.Finish /*|| HeaderException*/) {
			std::cout << "Packet has finished" << std::endl;
			HeaderIdle = true;
			receivedWords = 0;
			receivedBits = 0;
			NextHeaderValid = false;
			HeaderDone = false;
			NextHeader = 0;
		} else {
			++receivedWords;
			receivedBits+=N_BusSize;
		}
	}
}
#endif //_PKT_HEADER_HPP_
