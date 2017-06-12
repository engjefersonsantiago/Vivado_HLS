#include "PktFIFO.hpp"
#include "Header.hpp"
#include "parser_header_template.hpp"

#ifndef _TST_FIFO_HPP_
#define _TST_FIFO_HPP_

#if __GNUC_PREREQ(4,7)
using FIFOElementType = FifoElement<ap_int<FIFO_ELEMENT_SIZE>>;
#else
typedef FifoElement<ap_int<FIFO_ELEMENT_SIZE>> FIFOElementType;
#endif

enum op {READ, WRITE};

//#define PKT_BUS_SIZE 32
//#define PKT_BUS_SIZE 64
#define PKT_BUS_SIZE 128
//#define PKT_BUS_SIZE 256
//#define PKT_BUS_SIZE 320
//#define PKT_BUS_SIZE 512

#define ETH_HEADER_SIZE 14
#define IP_HEADER_SIZE 20
#define UDP_HEADER_SIZE 8

#define HEADER_SIZE 14	// 14: ethernet, 20: IP, 8: UDP
//#define HEADER_SIZE 20	// 14: ethernet, 20: IP, 8: UDP
//#define HEADER_SIZE 8		// 14: ethernet, 20: IP, 8: UDP

//bool FIFO_access (FIFOElementType *wrData, bool wrEn, FIFOElementType *rdData, bool rdEn);
bool HeaderAnalysisTop(PacketData<PKT_BUS_SIZE> DataIn, PHVData<HEADER_SIZE>* PHV, PacketData<PKT_BUS_SIZE>* DataOut);

#endif //_TST_FIFO_HPP_
