#include "tstFIFO.hpp"

//bool FIFO_access (FIFOElementType *wrData, bool wrEn,
//					FIFOElementType *rdData, bool rdEn) {
bool HeaderAnalysisTop(PacketData<PKT_BUS_SIZE, 32, 16> DataIn, PHVData<HEADER_SIZE, 32, 16>* PHV, PacketData<PKT_BUS_SIZE, 32, 16>* DataOut)
{
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS PIPELINE II=1
#pragma HLS LATENCY min=1 max=1

//<uint_16 N_Size, uint_16 N_Fields, typename T_Key, uint_16 N_Key, typename T_DataBus, uint_16 N_BusSize, uint_16 N_MaxPktSize>

	//static PktFIFO<FIFO_SIZE, FIFO_ELEMENT_SIZE, MAX_PKT_SIZE, MIN_PKT_SIZE>
	//	myFIFO (IF_SOFTWARE("my_fifo",) 0);
	static Header<ETH_HEADER_SIZE, 3, uint16_t, 7, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE, 32, 16>
		Ethernet(IF_SOFTWARE("Ethernet",) 0, EthernetLayout);
	//static Header<IP_HEADER_SIZE, 13, uint8_t, 2, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE, 32, 16>
	//	IPv4(IF_SOFTWARE("Ipv4",) 11, IPv4Layout);
	//static Header<UDP_HEADER_SIZE, 4, uint8_t, 1, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE, 32, 16>
	//	UDP(IF_SOFTWARE("UDP",) 22, UDPLayout);

#if HEADER_SIZE == 14
	Ethernet.HeaderAnalysis(DataIn, PHV, DataOut);
#elif HEADER_SIZE == 8
	UDP.HeaderAnalysis(DataIn, PHV, DataOut);
#else
	IPv4.HeaderAnalysis(DataIn, PHV, DataOut);
#endif

    return true;
}
