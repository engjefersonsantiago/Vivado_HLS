#include "PktFIFO.hpp"

#ifndef _TST_FIFO_HPP_
#define _TST_FIFO_HPP_

#if __GNUC_PREREQ(4,7)
using FIFOElementType = FifoElement<ap_int<FIFO_ELEMENT_SIZE>>;
#else
typedef FifoElement<ap_int<FIFO_ELEMENT_SIZE>> FIFOElementType;
#endif

enum op {READ, WRITE};

//#define PKT_BUS_SIZE 32
#define PKT_BUS_SIZE 64
//#define PKT_BUS_SIZE 128 
//#define PKT_BUS_SIZE 256 

//bool FIFO_access (FIFOElementType *wrData, bool wrEn, FIFOElementType *rdData, bool rdEn);
bool HeaderAnalysisTop (ap_uint<PKT_BUS_SIZE> DataIn, bool HeaderStart, bool HeaderFinish, ap_uint<8*14>* PHV, bool* PHVDone);

#endif //_TST_FIFO_HPP_
