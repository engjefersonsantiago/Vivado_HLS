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

#define HEADER_SIZE 14	// 14: ethernet, 20: IP, 8: UDP
//#define HEADER_SIZE 20	// 14: ethernet, 20: IP, 8: UDP
//#define HEADER_SIZE 8		// 14: ethernet, 20: IP, 8: UDP

//bool FIFO_access (FIFOElementType *wrData, bool wrEn, FIFOElementType *rdData, bool rdEn);
bool HeaderAnalysisTop (ap_uint<PKT_BUS_SIZE> DataIn, bool HeaderStart, bool HeaderFinish, ap_uint<8*HEADER_SIZE>* PHV, bool* PHVDone, ap_uint<PKT_BUS_SIZE>* DataOut, uint_16* NextHeaderOut, bool* NextHeaderValidOut);

#endif //_TST_FIFO_HPP_
