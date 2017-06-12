############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 2015 Xilinx Inc. All rights reserved.
############################################################
open_project synth
set_top HeaderAnalysisTop
add_files libIP/src/Header.hpp -cflags "-std=c++0x"
add_files libIP/src/MemoryInfer.hpp -cflags "-std=c++0x"
add_files libIP/src/PktFIFO.hpp -cflags "-std=c++0x"
add_files libIP/src/defined_types.h -cflags "-std=c++0x"
add_files libIP/src/parser_header_template.hpp -cflags "-std=c++0x"
add_files libIP/src/pktBasics.hpp -cflags "-std=c++0x"
add_files libIP/src/tstFIFO.cpp -cflags "-std=c++0x"
add_files libIP/src/tstFIFO.hpp -cflags "-std=c++0x"
add_files -tb libIP/src/top_tb.cpp -cflags "-std=c++0x"
open_solution "Zynq"
set_part {xc7z045ffg900-2}
create_clock -period 3.2 -name default
#source "./synth/Zynq/directives.tcl"
csim_design -clean
csynth_design
cosim_design
export_design -format ip_catalog
