#include "PktFIFO.hpp"

#ifndef _TST_FIFO_HPP_
#define _TST_FIFO_HPP_

#if __GNUC_PREREQ(4,7)
		using FIFOElementType = FifoElement<ap_int<FIFO_ELEMENT_SIZE>>;
#else
		typedef FifoElement<ap_int<FIFO_ELEMENT_SIZE>> FIFOElementType;
#endif

enum op {READ, WRITE};

bool FIFO_access (FIFOElementType *wrData, bool wrEn, FIFOElementType *rdData, bool rdEn);

#endif //_TST_FIFO_HPP_
