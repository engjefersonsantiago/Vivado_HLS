#include "tstFIFO.hpp"

//bool FIFO_access (FIFOElementType *wrData, bool wrEn,
//					FIFOElementType *rdData, bool rdEn) {
bool HeaderAnalysisTop(PacketData<PKT_BUS_SIZE> DataIn, PHVData<HEADER_SIZE>* PHV, PacketData<PKT_BUS_SIZE>* DataOut)
{
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS PIPELINE II=1
#pragma HLS LATENCY min=1 max=1

//<uint_16 N_Size, uint_16 N_Fields, typename T_Key, uint_16 N_Key, typename T_DataBus, uint_16 N_BusSize, uint_16 N_MaxPktSize>

	ap_uint<PKT_BUS_SIZE> DataIn_0;
	bool HeaderStart_0;
	bool HeaderFinish_0;
	ap_uint<8*14>* PHV_0;
	bool* PHVDone_0;
	ap_uint<PKT_BUS_SIZE>* DataOut_0;
	uint_16* NextHeaderOut_0;
	bool* NextHeaderValidOut_0;

	static PktFIFO<FIFO_SIZE, FIFO_ELEMENT_SIZE, MAX_PKT_SIZE, MIN_PKT_SIZE>
		myFIFO (IF_SOFTWARE("my_fifo",) 0);
	static Header<ETH_HEADER_SIZE, 3, uint_16, 7, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE>
		Ethernet(IF_SOFTWARE("Ethernet",) 0, EthernetLayout);
	static Header<IP_HEADER_SIZE, 13, uint_8, 2, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE>
		IPv4(IF_SOFTWARE("Ipv4",) 11, IPv4Layout);
	static Header<UDP_HEADER_SIZE, 4, uint_8, 1, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE>
		UDP(IF_SOFTWARE("UDP",) 22, UDPLayout);

#if HEADER_SIZE == 14
	Ethernet.HeaderAnalysis(DataIn, PHV, DataOut);
#elif HEADER_SIZE == 8
	UDP.HeaderAnalysis(DataIn, PHV, DataOut);
#else
	IPv4.HeaderAnalysis(DataIn, PHV, DataOut);
#endif

	//Ethernet.HeaderAnalysis(DataIn, HeaderStart, HeaderFinish, PHV_0, PHVDone_0, DataOut_0, NextHeaderOut_0, NextHeaderValidOut_0);
	//IPv4.HeaderAnalysis(&DataOut_0, &PHVDone_0, 0, PHV, PHVDone, DataOut, NextHeaderOut, NextHeaderValidOut);
	//IPv4.HeaderAnalysis(DataIn, HeaderStart, HeaderFinish, PHV, PHVDone, DataOut, NextHeaderOut, NextHeaderValidOut);
	// myFIFO.access(wrData, wrEn, rdData, rdEn);
    return true;
}
