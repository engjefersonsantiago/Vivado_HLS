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
#define ARRAY_FOR_SR 0

// Define to enable lookup implementation for the extraction bit shifts
#define LOOKUPTABLE_FOR_SHIFT_CALC 1	//For "small" headers reduces area and improves performance

// Macro to define the stuff data out size and the key shift
#define STUFF_AND_SHIFT_SIZE_OP(A, B, C) (((A) < (B)) ? (B - C) : (B - (C%B)))

// Needs to be defined to force packet bypass
#define ENABLE_STATE_BYPASS

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
// N_MaxSuppHeaders: Max number of supported headers
// N_MaxPktId: Max packet packet ID (in bits)
// Construction parameters:
// instance_name: human readable name of the object instance
// instance_id: unique class instance identifier
// HLayout: header layout structure for the processed header
template<uint16_t N_Size, uint16_t N_Fields, typename T_Key, uint16_t N_Key, typename T_DataBus, uint16_t N_BusSize, uint16_t N_MaxPktSize, uint16_t N_MaxSuppHeaders, uint8_t N_MaxPktId>
class Header {
    private:
		// Defines here to fix template parameters passage
		static constexpr uint16_t HEADER_SIZE_IN_BITS = bytes2Bits(N_Size);
		static constexpr uint16_t PKT_SIZE_IN_BITS = bytes2Bits(N_MaxPktSize);
		static constexpr uint16_t DATAOUT_STUFF_SIZE = STUFF_AND_SHIFT_SIZE_OP(HEADER_SIZE_IN_BITS, N_BusSize, HEADER_SIZE_IN_BITS);
		static constexpr uint16_t DATAOUT_STUFF_SHIFT = N_BusSize - DATAOUT_STUFF_SIZE;
		static constexpr uint16_t RECEIVED_MAX_WORDS = DivAndCeil(HEADER_SIZE_IN_BITS, N_BusSize);
		static constexpr uint16_t GREATER_BUS_OR_HEADER_SIZE = ((HEADER_SIZE_IN_BITS > N_BusSize) ? HEADER_SIZE_IN_BITS: N_BusSize);
		static constexpr uint16_t RECEIVED_WORDS_SIZE = numbits(RECEIVED_MAX_WORDS);
		static constexpr uint16_t RECEIVED_BITS_SIZE = numbits(PKT_SIZE_IN_BITS);

		//Used types
		typedef ap_uint<HEADER_SIZE_IN_BITS> ExtractedHeaderType;
		typedef ap_uint<numbits(GREATER_BUS_OR_HEADER_SIZE)> busOrHeaderBitsuType;
		typedef ap_int<numbits(GREATER_BUS_OR_HEADER_SIZE) + 1> busOrHeaderBitsType;
		typedef std::array<ap_uint<HEADER_SIZE_IN_BITS>, RECEIVED_MAX_WORDS> ExtractedHeaderPipeType;
		typedef ap_uint<RECEIVED_WORDS_SIZE> receivedWordsType;
		typedef ap_uint<RECEIVED_BITS_SIZE> receivedBitsType;
		typedef ap_uint<numbits(N_MaxSuppHeaders)> headerIDType;

		IF_SOFTWARE(const std::string instance_name;)
		const headerIDType instance_id;

		const HeaderFormat<N_Size, N_Fields, T_Key, N_Key, N_MaxSuppHeaders> HeaderLayout;

		bool HeaderIdle;
		bool HeaderDone;
		bool HeaderDonePulse;
		bool HeaderException;

		headerIDType NextHeader;
		bool NextHeaderValid;

		ExtractedHeaderType ExtractedHeader;
		ExtractedHeaderPipeType ExtractedHeaderPipe;
		receivedWordsType receivedWords;
		receivedBitsType receivedBits;

		PacketData<N_BusSize, N_MaxSuppHeaders, N_MaxPktId> PacketOutReg;

		const busOrHeaderBitsuType stateTransShiftVal;
		std::array<busOrHeaderBitsType, RECEIVED_MAX_WORDS> shiftNumLookup;

		const T_Key DataInMask;
		const ap_uint<DATAOUT_STUFF_SIZE> stuffSizeMask;

	public:
        // Constructor
        Header(
				IF_SOFTWARE(const std::string instance_name,)
				const headerIDType instance_id,
				const HeaderFormat<N_Size, N_Fields, T_Key, N_Key, N_MaxSuppHeaders>& HLayout) :
				IF_SOFTWARE(instance_name{instance_name},)
				instance_id{instance_id},
				HeaderLayout(HLayout),
				//PacketOutReg{0, 0, false, false, 0},	// Vivado does not like it!!
				HeaderIdle{true},
				receivedWords{0},
				receivedBits{0},
				NextHeaderValid{false},
				HeaderDone{false},
				NextHeader{0},
				stateTransShiftVal{	STUFF_AND_SHIFT_SIZE_OP(HEADER_SIZE_IN_BITS, \
									N_BusSize,
									(HLayout.KeyLocation.first + HLayout.KeyLocation.second))},
				DataInMask{T_Key((1 << HeaderLayout.KeyLocation.second) - 1)},
				stuffSizeMask{ap_uint<DATAOUT_STUFF_SIZE>((ap_uint<DATAOUT_STUFF_SIZE+1>(1) << DATAOUT_STUFF_SIZE) - 1)}
		{
			for (int it = 0; it < shiftNumLookup.size(); ++it){
				shiftNumLookup[it] = HEADER_SIZE_IN_BITS - N_BusSize*(it+1);
			}
		}

        // General MISC information
        const uint16_t getHeaderSize() const {return N_Size;}
        const uint16_t getFieldNum() const {return HeaderLayout.Fields.size();}
        const uint16_t getOutStuffBits() const {return DATAOUT_STUFF_SIZE;}
        IF_SOFTWARE(void printFields() {
			for (auto it : HeaderLayout.Fields)
				std::cout << "Field: " << it.FieldName << std::endl;
		})
        IF_SOFTWARE(void printNextHeaders() {
			for (auto it : HeaderLayout.Key)
				std::cout << "Key: " << std::hex << it.KeyVal << " Next Header: " << it.NextHeaderName << std::endl;
		})
		IF_SOFTWARE(const std::string getInstName() const {return instance_name;})
        headerIDType getInstId() const {return instance_id;};

		void StateTransition(const PacketData<N_BusSize, N_MaxSuppHeaders, N_MaxPktId>& PacketIn, receivedBitsType receivedBits, bool& NextHeaderValid, headerIDType& NextHeader);
		void ExtractFields(const PacketData<N_BusSize, N_MaxSuppHeaders, N_MaxPktId>& PacketIn, receivedWordsType receivedWords, bool& HeaderDone, PHVData<N_Size, N_MaxSuppHeaders, N_MaxPktId>& PHV, bool& HeaderDonePulse);
		void PipelineAdjust(const PacketData<N_BusSize, N_MaxSuppHeaders, N_MaxPktId>& PacketIn, PacketData<N_BusSize, N_MaxSuppHeaders, N_MaxPktId>& PacketOutReg, PacketData<N_BusSize, N_MaxSuppHeaders, N_MaxPktId>& PacketOut, headerIDType& NextHeader, bool& HeaderDonePulse);
		void HeaderAnalysis(const PacketData<N_BusSize, N_MaxSuppHeaders, N_MaxPktId>& PacketIn, PHVData<N_Size, N_MaxSuppHeaders, N_MaxPktId>& PHV, PacketData<N_BusSize, N_MaxSuppHeaders, N_MaxPktId>& PacketOut);

};

// State transition function
template<uint16_t N_Size, uint16_t N_Fields, typename T_Key, uint16_t N_Key, typename T_DataBus, uint16_t N_BusSize, uint16_t N_MaxPktSize, uint16_t N_MaxSuppHeaders, uint8_t N_MaxPktId>
void Header<N_Size, N_Fields, T_Key, N_Key, T_DataBus, N_BusSize, N_MaxPktSize, N_MaxSuppHeaders, N_MaxPktId>
	::StateTransition (const PacketData<N_BusSize, N_MaxSuppHeaders, N_MaxPktId>& PacketIn, receivedBitsType receivedBits, bool& NextHeaderValid, headerIDType& NextHeader)
{
#pragma HLS INLINE
#pragma HLS UNROLL
#pragma HLS LATENCY min=1 max=1

	// Combinational logic
	//T_Key DataInMask = (1 << HeaderLayout.KeyLocation.second) - 1;
	auto tmpReceivedBits = receivedBits + N_BusSize;
	T_Key tmpKeyVal = T_Key(PacketIn.Data >> stateTransShiftVal) & DataInMask;

	HeaderException = false;
	if (HeaderLayout.LastHeader) {
		IF_SOFTWARE(std::cout << "Last header. No state transition" << std::endl;)
		return;
	}
	if (!NextHeaderValid && tmpReceivedBits > HeaderLayout.KeyLocation.first) {
		HeaderException = true;
		loop_key: for (auto it : HeaderLayout.Key) {
			if (it.KeyVal == tmpKeyVal & it.KeyMask) {
				IF_SOFTWARE(std::cout << "Found a valid transition from " << instance_name << " to "\
							<< it.NextHeaderName << ". Key: " << std::hex << uint64_t(it.KeyVal) << std::endl;)
				NextHeader = it.NextHeader;
				NextHeaderValid = true;
				HeaderException	= false;
			}
		}
		if (HeaderException) {
			IF_SOFTWARE(std::cout << "Header transition not found. Aborting processing" << std::endl;)
		}
	}
};

// Header extraction
template<uint16_t N_Size, uint16_t N_Fields, typename T_Key, uint16_t N_Key, typename T_DataBus, uint16_t N_BusSize, uint16_t N_MaxPktSize, uint16_t N_MaxSuppHeaders, uint8_t N_MaxPktId>
void Header<N_Size, N_Fields, T_Key, N_Key, T_DataBus, N_BusSize, N_MaxPktSize, N_MaxSuppHeaders, N_MaxPktId>
	::ExtractFields(const PacketData<N_BusSize, N_MaxSuppHeaders, N_MaxPktId>& PacketIn, receivedWordsType receivedWords, bool& HeaderDone, PHVData<N_Size, N_MaxSuppHeaders, N_MaxPktId>& PHV, bool& HeaderDonePulse)
{
#pragma HLS INLINE
#pragma HLS LATENCY min=1 max=1
#pragma HLS ARRAY_PARTITION variable=ExtractedHeaderPipe dim=1
#pragma HLS ARRAY_PARTITION variable=shiftNumLookup dim=1

	auto tmpShiftVal = N_BusSize*(receivedWords+1);	//Implemented as SR not mul
	ExtractedHeaderType tmpDinlShifted;
#if LOOKUPTABLE_FOR_SHIFT_CALC
	if (HEADER_SIZE_IN_BITS == N_BusSize)
		tmpDinlShifted = PacketIn.Data;
	else
		tmpDinlShifted = ExtractedHeaderType(PacketIn.Data) << shiftNumLookup[receivedWords];
#else
	if (HEADER_SIZE_IN_BITS == N_BusSize)
		tmpDinlShifted = PacketIn.Data;
	else
		tmpDinlShifted = (ExtractedHeaderType(PacketIn.Data) << (HEADER_SIZE_IN_BITS - tmpShiftVal));
#endif
	ExtractedHeaderType tmpDinrShifted;
	// Hope the tool simplifies it
	if (HEADER_SIZE_IN_BITS <= N_BusSize)  {
		tmpDinrShifted = ExtractedHeaderType((PacketIn.Data) >> (N_BusSize - HEADER_SIZE_IN_BITS));
	}

	IF_SOFTWARE(PHV.Name = instance_name;)
	PHV.ID = instance_id;
	PHV.PktID = PacketIn.ID;
	PHV.ValidPulse = HeaderDonePulse = false;

	if (!HeaderDone) {
		PHV.ValidPulse = PHV.Valid = HeaderDone;
		if (receivedWords < RECEIVED_MAX_WORDS) {
			if (HEADER_SIZE_IN_BITS > N_BusSize)  {
				if (receivedWords == 0) {
#if not ARRAY_FOR_SR
					ExtractedHeader = tmpDinlShifted;
#else
					ExtractedHeaderPipe[0] = tmpDinlShifted;
#endif

				} else {
#if not ARRAY_FOR_SR
					ExtractedHeader |= tmpDinlShifted;
#else
					ExtractedHeaderPipe[receivedWords] = ExtractedHeaderPipe[receivedWords-1] | tmpDinlShifted;
#endif
				}
			} else {
#if not ARRAY_FOR_SR
				ExtractedHeader = tmpDinrShifted;
#else
				ExtractedHeaderPipe[0] = tmpDinrShifted;
#endif
			}
		} else  {
			std::cout << "receivedWords >= RECEIVED_MAX_WORDS  " <<receivedWords << std::endl;

#if not ARRAY_FOR_SR
			PHV.Data = ExtractedHeader & HeaderLayout.PHVMask;
#else
			PHV.Data = ExtractedHeaderPipe[receivedWords - 1] & HeaderLayout.PHVMask;
#endif
			PHV.ValidPulse = PHV.Valid = HeaderDone = HeaderDonePulse = true;
			std::cout << "Extracted: " << std::dec << N_Size << " Bytes. PHV: " << std::hex << PHV.Data << std::endl;
		}
	}
}

// Pipeline adjesment function for further stages
template<uint16_t N_Size, uint16_t N_Fields, typename T_Key, uint16_t N_Key, typename T_DataBus, uint16_t N_BusSize, uint16_t N_MaxPktSize, uint16_t N_MaxSuppHeaders, uint8_t N_MaxPktId>
void Header<N_Size, N_Fields, T_Key, N_Key, T_DataBus, N_BusSize, N_MaxPktSize, N_MaxSuppHeaders, N_MaxPktId>
	::PipelineAdjust(const PacketData<N_BusSize, N_MaxSuppHeaders, N_MaxPktId>& PacketIn, PacketData<N_BusSize, N_MaxSuppHeaders, N_MaxPktId>& PacketOutReg, PacketData<N_BusSize, N_MaxSuppHeaders, N_MaxPktId>& PacketOut, headerIDType& NextHeader, bool& HeaderDonePulse)
{
#pragma HLS LATENCY min=1 max=1
	//auto stuffSizeMask = ap_uint<DATAOUT_STUFF_SIZE>((ap_uint<DATAOUT_STUFF_SIZE+1>(1) << DATAOUT_STUFF_SIZE) - 1);
	PacketOut.Data = (DATAOUT_STUFF_SHIFT == 0) ?
					  T_DataBus(PacketIn.Data) :
					  (((T_DataBus(PacketOutReg.Data) & stuffSizeMask) << DATAOUT_STUFF_SHIFT)
							| (T_DataBus(PacketIn.Data) >> DATAOUT_STUFF_SIZE));
	PacketOut.ID = PacketOutReg.ID;
	PacketOut.Start = HeaderDonePulse;
	PacketOut.Finish = PacketIn.Finish;
	PacketOut.HeaderID = NextHeader;
	IF_SOFTWARE(std::cout << instance_name << " Packet Start Out: " << std::hex << PacketOut.Start << std::endl;)
	IF_SOFTWARE(std::cout << instance_name << " Packet Finish Out: " << std::hex << PacketOut.Finish << std::endl;)
	IF_SOFTWARE(std::cout << instance_name << " Packet HeaderID Out: " << std::hex << PacketOut.HeaderID << std::endl;)
	IF_SOFTWARE(std::cout << instance_name << " Packet Word Out: " << std::hex << PacketOut.Data << std::endl;)
	// Output data delay (to keep the pipeline full)
	PacketOutReg = PacketIn;
}

template<uint16_t N_Size, uint16_t N_Fields, typename T_Key, uint16_t N_Key, typename T_DataBus, uint16_t N_BusSize, uint16_t N_MaxPktSize, uint16_t N_MaxSuppHeaders, uint8_t N_MaxPktId>
void Header<N_Size, N_Fields, T_Key, N_Key, T_DataBus, N_BusSize, N_MaxPktSize, N_MaxSuppHeaders, N_MaxPktId>
	::HeaderAnalysis(const PacketData<N_BusSize, N_MaxSuppHeaders, N_MaxPktId>& PacketIn, PHVData<N_Size, N_MaxSuppHeaders, N_MaxPktId>& PHV, PacketData<N_BusSize, N_MaxSuppHeaders, N_MaxPktId>& PacketOut)
{
#pragma HLS LATENCY min=1 max=1
#pragma HLS DEPENDENCE variable=HeaderIdle WAR false
#pragma HLS DEPENDENCE variable=NextHeaderValid WAR false
#pragma HLS DEPENDENCE variable=HeaderDone WAR false
#pragma HLS DEPENDENCE variable=NextHeader WAR false
#pragma HLS PIPELINE II=1

	// New packet detection
	if (PacketIn.Start && PacketIn.HeaderID == instance_id) {
		IF_SOFTWARE(std::cout << "Received a new packet" << std::endl;)
		HeaderIdle = false;
	}

	IF_SOFTWARE(std::cout << instance_name << " Packet Start In: " << std::hex << PacketIn.Start << std::endl;)
	IF_SOFTWARE(std::cout << instance_name << " Packet Finish In: " << std::hex << PacketIn.Finish << std::endl;)
	IF_SOFTWARE(std::cout << instance_name << " Packet HeaderID In: " << std::hex << PacketIn.HeaderID << std::endl;)
	IF_SOFTWARE(std::cout << instance_name << " Packet Word In: " << std::hex << PacketIn.Data << std::endl;)

	if (!HeaderIdle) {

		// Header extration
		ExtractFields(PacketIn, receivedWords, HeaderDone, PHV, HeaderDonePulse);

		// State transition evaluation
		StateTransition(PacketIn, receivedBits, NextHeaderValid, NextHeader);

		// Adjusting output data
		PipelineAdjust(PacketIn, PacketOutReg, PacketOut, NextHeader, HeaderDonePulse);

		// Execution control
		if (PacketIn.Finish /*|| HeaderException*/) {
			IF_SOFTWARE(std::cout << "Packet has finished" << std::endl;)
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
	} else {
#ifdef ENABLE_STATE_BYPASS
		PacketOut = PacketIn;
#endif
	}

}
#endif //_PKT_HEADER_HPP_
