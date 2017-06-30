###############################################################################
# Packet Parser generator: code generation for a packet parser                #
# Jeferson Santiago da Silva (eng.jefersonsantiago@gmail.com)                 #
###############################################################################

import json
import string
import sys
import math
import copy
import re

HEADER_TYPES_STR = "header_types"
HEADER_INST_STR = "headers"
PARSER_INST_STR = "parsers"

def recur_search (exp_dict):
    expr_str = ""
    if exp_dict["type"] == "expression":
        expr_str = expr_str + "(" + (str(recur_search(exp_dict["value"]["left"])) + str(exp_dict["value"]["op"]) + str(recur_search(exp_dict["value"]["right"]))) + ")" 
    else:
        if exp_dict["type"] != "local":
            expr_str = expr_str + str(exp_dict["value"])
        else:
            expr_str = expr_str + "field[" + str(exp_dict["value"]) + "]"    #expr_val is the variable name used in the header layout struct

    return expr_str

def json_parser(json_file):
	with open(json_file) as data_file:
		data = json.load(data_file)

	#Retrieving relevant json arrays from json descritpion
	for json_array in data:
		if json_array == HEADER_TYPES_STR:
			header_types = data[json_array]
		if json_array == HEADER_INST_STR:
			headers_inst = data[json_array]
		if json_array == PARSER_INST_STR:
			parser_inst = data[json_array]

	#Header declaration analisys
	headers_list = []
	for headers in header_types:
		header_dict = {}
		header_dict["num_fields"] = len(headers["fields"])
		header_dict["fields"] = [None]*header_dict["num_fields"]
                field_offset = 0
                for field in range(header_dict["num_fields"]):
                    header_dict["fields"][field] = [None]*3
	            header_dict["fields"][field][0] = headers["fields"][field][0]
	            header_dict["fields"][field][1] = field_offset
                    if field == header_dict["num_fields"] - 1 and headers["max_length"] != None:
                        header_dict["fields"][field][2] = 8*headers["max_length"] - field_offset
                        field_offset = 8*headers["max_length"]
                        header_dict["var_size_header"] = True
                    else:
                        header_dict["fields"][field][2] = headers["fields"][field][1]
                        field_offset+=int(headers["fields"][field][1])
                        header_dict["var_size_header"] = False

                header_dict["header_length_lookup_array"] = []
                header_dict["header_length_field"] = ""
                header_dict["header_length_position"] = 0
		header_dict["header_length_size"] = 1
		header_dict["header_length_expression"] = ""
                expr = headers["length_exp"] 
                exp_str = ""
                exp_field = 0
                if expr != None:
                    if expr["type"] == "expression":     
                        if expr["value"]["op"] == "-":   # only interested in parsing inside the '-' expression
                            exp_str = recur_search(expr["value"]["left"]) 
                            exp_re = r'field\[(\w+)\]'
                            header_field_exp = re.findall(exp_re, exp_str)
                            header_dict["header_length_field"] = str(header_dict["fields"][int(header_field_exp[0])][0]) 
                            header_dict["header_length_expression"] = re.sub(exp_re, header_dict["header_length_field"], exp_str)
                            header_dict["header_length_position"] = header_dict["fields"][int(header_field_exp[0])][1] 
                            header_dict["header_length_size"] = header_dict["fields"][int(header_field_exp[0])][2] 
                    print "Found a variable size header: " + headers["name"] + ". Header length expression: " + header_dict["header_length_expression"]
                header_dict["parser_state"] = ""
		header_dict["parser_state_id"] = 0
		header_dict["next_state"] = []
		header_dict["next_state_id"] = []
		header_dict["previous_state"] = []
		header_dict["previous_state_id"] = []
		header_dict["previous_state_both"] = []
		header_dict["header_size"] = field_offset/8
		header_dict["header_size_bits"] = field_offset
		header_dict["num_fields"] = len(headers["fields"])
		header_dict["extr_mask"] = ((1 << header_dict["header_size_bits"]) - 1)
		header_dict["transition_keys"] = []
		header_dict["key_name"] = ""
		header_dict["key_position"] = 0
		header_dict["key_size"] = 1			# Workaround for final states
		header_dict["key_type"] = "ap_uint<1>"		# Workaround for final states
		header_dict["key_number"] = 0

		header_dict["last_header"] = True
		header_dict["first_header"] = False
		header_dict["header_type_name"] = headers["name"]
		header_dict["header_name"] = "" #from instantiation
		header_dict["header_id"] = headers["id"]
		header_dict["metadata"] = False

                if header_dict["var_size_header"]:
                    exp_str = header_dict["header_length_expression"] 
                    fx = re.sub(header_dict["header_length_field"], "x", exp_str) 
                    f = lambda x: eval(fx)
                    for i in range(header_dict["header_size_bits"]):
                        res = f(i)
                        if res >  header_dict["header_size_bits"]:
                            break
                        else:
                            header_dict["header_length_lookup_array"].append(res)
                    print(header_dict["header_length_lookup_array"])
                else:
                    header_dict["header_length_lookup_array"].append(1)
                header_dict["header_length_larray_size"] = len(header_dict["header_length_lookup_array"])
                last_len_elem = header_dict["header_length_lookup_array"][header_dict["header_length_larray_size"] - 1] 
                print (last_len_elem)
                len_bit_size = 0 
                if last_len_elem % 2:
                    if last_len_elem == 1:
                        len_bit_size = 1
                    else:
                        len_bit_size = int(math.ceil(math.log(last_len_elem, 2)))
                else:
                    len_bit_size = int(math.ceil(math.log(last_len_elem + 1, 2)))
                header_dict["len_bit_size"] = len_bit_size

                headers_list.append(header_dict)

	#Header instantiation analisys
	for header_i in headers_inst:
		for headers_t in headers_list:
			if headers_t["header_type_name"] == header_i["header_type"]:
				print "Found an instantiation for the header type " + header_i["header_type"] \
						+ " with the name " + header_i["name"]
				headers_t["header_name"] = header_i["name"]
				if headers_t["header_id"] != header_i["id"]:
					headers_t["header_id"] = header_i["id"]
					print "Header identifier different in the declaration and instantiation." \
							+ " Using the instantiation value"
				if header_i["metadata"]:
					print "Header " + header_i["name"] + " is a metadata instance. Removing from header list"
					headers_t["metadata"] = True
					headers_list.remove(headers_t)
				break

	#Header parser analisys
	for parser_i in parser_inst:
		parser_ini_state = parser_i["init_state"]
		parser_name = parser_i["name"]
		parser_id = parser_i["id"]
		for states in parser_i["parse_states"]:
			# Linking parser state and header name
			for ops in states["parser_ops"]:
				for ops_params in ops["parameters"]:
					for headers_t in headers_list:
						if ops_params["value"] == headers_t["header_name"]:
							headers_t["parser_state"] = states["name"]
							headers_t["parser_state_id"] = states["id"]
							print "Linking header " + headers_t["header_name"] + " to parser state " \
									+ headers_t["parser_state"] + " with ID " + str(headers_t["parser_state_id"])
			# Retriving transition key
			for keys in states["transition_key"]:
				for headers_t in headers_list:
					if headers_t["header_name"] == keys["value"][0]:
						for field in headers_t["fields"]:
							if field[0] == keys["value"][1]:
								headers_t["last_header"] = False
								print "Found match key in " + states["name"] + ". Key is "\
										+ headers_t["header_name"] + "." + field[0]
								headers_t["key_position"] = field[1]
								headers_t["key_size"] = field[2]
								headers_t["key_type"] = "ap_uint<" + str(headers_t["key_size"]) + ">"

								# Evaluate state transitions
								for transitions in states["transitions"]:
									transition_tuple = [None]*5
									if transitions["type"] == "default":
										continue
									else:
									    print "Found state transition from "\
									    + headers_t["header_name"] + " to " + transitions["next_state"]

									    transition_tuple[0] = transitions["value"]
									    transition_tuple[1] = transitions["mask"] if transitions["mask"]!=None else str((1 << headers_t["key_size"]) - 1)
									    transition_tuple[2] = transitions["next_state"]
									    transition_tuple[3] = 0		# Here goes the ID
									    transition_tuple[4] = ""	# Header name
									    headers_t["transition_keys"].append(transition_tuple)

	# Final adjusts to retrieve max header number and pipeline laytout
	transition_id = []
	header_sizes_id = []
	header_size = 0
	for headers_t in headers_list:
		headers_t["key_number"] = len(headers_t["transition_keys"])
		transition_id.append(headers_t["parser_state_id"])
		header_sizes_id.append(headers_t["header_size"])
		header_size+=headers_t["header_size"]
		for headers_tt in headers_list:
			for state in headers_t["transition_keys"]:
				if headers_tt["parser_state"] == state[2]:
					headers_t["next_state"].append(headers_tt["header_name"])
					headers_t["next_state_id"].append(headers_tt["parser_state_id"])
					state[3] = headers_tt["parser_state_id"]
					state[4] = headers_tt["header_name"]
					# Necessary to identify nodes with more than one source (proper mux insertion)
					headers_tt["previous_state"].append(headers_t["header_name"])
					headers_tt["previous_state_id"].append(headers_t["parser_state_id"])
					previous_state_tuple = [None]*2
					previous_state_tuple[0] = headers_t["header_name"]
					previous_state_tuple[1] = headers_t["parser_state_id"]
					headers_tt["previous_state_both"].append(previous_state_tuple)

	input_parser_state = min(transition_id)
	max_supp_headers = max(transition_id)
	max_header_size = max(header_sizes_id)
	header_num = len(header_sizes_id)
	header_size_avg = int(math.ceil(header_size/header_num))
	return headers_list, input_parser_state, max_supp_headers, max_header_size, header_num, header_size_avg

##############################################################
# Write the header layout types
##############################################################
def write_headers_template(headers_list, max_supp_headers, max_header_size, header_num):
	print "Generating Header Layouts..."
	with open("parser_header_template.hpp", "w") as parser_header_template:
		parser_header_template.write("/******************************************************************************" + "\n")
		parser_header_template.write("* parser_header_template: Self-generated template structs for the parser      *" + "\n")
		parser_header_template.write("* Jeferson Santiago da Silva (eng.jefersonsantiago@gmail.com)                 *" + "\n")
		parser_header_template.write("******************************************************************************/" + "\n")
		parser_header_template.write("\n#include <iostream>" + "\n")
		parser_header_template.write("#include <math.h>" + "\n")
		parser_header_template.write("#include <string>" + "\n")
		parser_header_template.write("#include <array>" + "\n")

		parser_header_template.write("\n#include \"pktBasics.hpp\"" + "\n")

		parser_header_template.write("\n#ifndef _PARSER_HEADER_TEMP_HPP_" + "\n")
		parser_header_template.write("#define _PARSER_HEADER_TEMP_HPP_" + "\n")

		# Defines
		parser_header_template.write("\n#define HEADER_NUM " + str(header_num) + "\n")
		parser_header_template.write("\n#define MAX_SUPP_HEADERS " + str(max_supp_headers) + "\n")
		parser_header_template.write("\n#define MAX_HEADER_SIZE " + str(max_header_size) + "\n")
		parser_header_template.write("\n#define MAX_HEADER_SIZE_BITS bytes2Bits(MAX_HEADER_SIZE)\n")

		for headers_t in headers_list:
			header_name = headers_t["header_name"]
			header_name_cap = string.upper(header_name)
			parser_header_template.write("\n#define " + header_name_cap + "_HEADER_SIZE " + str(headers_t["header_size"])  + "\n")
			parser_header_template.write("\n#define " + header_name_cap + "_HEADER_SIZE_BITS bytes2Bits(" + header_name_cap + "_HEADER_SIZE)" + "\n")
			parser_header_template.write("\n#define " + header_name_cap + "_NUM_FIELDS " + str(headers_t["num_fields"]) + "\n")

                        header_layout = "HeaderFormat"
                        if headers_t["var_size_header"]:
                            header_layout = header_name + "HeaderFormat"
                            # Redefining header template for variable sized headers
                            parser_header_template.write("template<uint16_t N_Size, uint16_t N_Fields, typename T_Key, uint16_t N_Key, uint16_t N_MaxSuppHeaders, uint16_t N_HeaderLenArrSize, uint16_t N_HeaderLenELemBits> " + "\n") 
                            parser_header_template.write("struct " + header_layout + " : public HeaderFormat<N_Size, N_Fields, T_Key, N_Key, N_MaxSuppHeaders, N_HeaderLenArrSize, N_HeaderLenELemBits> { " + "\n") 
	                    #parser_header_template.write("\ttemplate <typename... T>  " + "\n")  
	                    #parser_header_template.write("\t" + header_layout + "(T... args) : HeaderFormat<N_Size, N_Fields, T_Key, N_Key, N_MaxSuppHeaders>(args...){} " + "\n")   

                            
                            parser_header_template.write("\t" + header_layout + " (ap_uint<bytes2Bits(N_Size)> PHVMask, " + "\n")  
	                    parser_header_template.write("\t\tstd::array<FieldFormat<N_Size>, N_Fields> Fields, " + "\n")  
	                    parser_header_template.write("\t\tstd::array<KeyFormat<T_Key, N_MaxSuppHeaders>, N_Key> Key, " + "\n")  
	                    parser_header_template.write("\t\tstd::pair<ap_uint<numbits(bytes2Bits(N_Size))>, ap_uint<numbits(bytes2Bits(N_Size))>> KeyLocation, " + "\n")  
	                    parser_header_template.write("\t\tbool LastHeader," + "\n") 					
	                    parser_header_template.write("\t\tIF_SOFTWARE(std::string HeaderName,)" + "\n")   
	                    parser_header_template.write("\t\tbool varSizeHeader," + "\n") 				
                            parser_header_template.write("\t\tstd::pair<ap_uint<numbits(bytes2Bits(N_Size))>, ap_uint<numbits(bytes2Bits(N_Size))>> HeaderLengthInd, std::array<ap_uint<N_HeaderLenELemBits>, N_HeaderLenArrSize> ArrLenLookup) : " + "\n")  
                            parser_header_template.write("\tHeaderFormat<N_Size, N_Fields, T_Key, N_Key, N_MaxSuppHeaders, N_HeaderLenArrSize, N_HeaderLenELemBits> ({PHVMask, " + "\n")  
	                    parser_header_template.write("\t\tFields, " + "\n")  
	                    parser_header_template.write("\t\tKey, " + "\n")  
	                    parser_header_template.write("\t\tKeyLocation, " + "\n")  
	                    parser_header_template.write("\t\tLastHeader," + "\n") 					
	                    parser_header_template.write("\t\tIF_SOFTWARE(HeaderName,)" + "\n")   
	                    parser_header_template.write("\t\tvarSizeHeader," + "\n") 				
                            parser_header_template.write("\t\tHeaderLengthInd," + "\n")      
                            parser_header_template.write("\t\tArrLenLookup}) {}" + "\n")      
                            
                            parser_header_template.write("\n\tvoid getHeaderSize(ap_uint<numbits(bytes2Bits(N_Size))>& size, "\
                                                            "const ap_uint<" + str(headers_t["header_length_size"]) + ">& " \
                                                            + headers_t["header_length_field"] + ") {" + "\n") 
                            parser_header_template.write("\t\tsize = " + str(headers_t["header_length_expression"]) + ";" + "\n") 
                            parser_header_template.write("\t}" + "\n")
                            parser_header_template.write("};" + "\n")

			# Header layout declaration
			if headers_t["last_header"]:
			    headers_t["key_number"] = 1
			parser_header_template.write("\nconst " + header_layout + "<" + str(headers_t["header_size"]) + ", "		\
											+ str(headers_t["num_fields"]) + ", " + headers_t["key_type"]	\
											+ ", " + str(headers_t["key_number"]) + ", MAX_SUPP_HEADERS "	\
											+ ", " + str(headers_t["header_length_larray_size"]) \
                                                                                        + ", " + str(headers_t["len_bit_size"]) 
                                                                                        + "> " + headers_t["header_type_name"] + "\n")
			parser_header_template.write( "{" + "\n")

			# Extract mask
			parser_header_template.write(  "\t(ap_uint<" + str(headers_t["header_size_bits"])   \
						+ ">(\"" + str(headers_t["extr_mask"]) + "\"))," + "\n")

			# Header Fileds
			#field_str = "\t{\n\t\t{"
                        field_str = "\tstd::array<FieldFormat< " + str(headers_t["header_size"]) + ">," + str(headers_t["num_fields"]) + ">{\n\t\t{"

                        for field in range(headers_t["num_fields"]):
				field_str =  field_str + "\n\t\t\t{" + str(headers_t["fields"][field][1]) + ", "	\
					+ str(headers_t["fields"][field][2]) + " IF_SOFTWARE(, \""		\
					+ str(headers_t["fields"][field][0]) + "\")}"
				if field == headers_t["num_fields"] - 1:
					field_str =  field_str + "\n\t\t},\n\t},"
				else:
					field_str =  field_str + ","
			parser_header_template.write( field_str	 + "\n")

			# Match Keys
			#key_str = "\t{\n\t\t{"
			key_str = "\tstd::array<KeyFormat<" + headers_t["key_type"] + ", " + str(max_supp_headers) + ">," + str(headers_t["key_number"]) + ">{\n\t\t{"
			it = 0
			num_keys = len(headers_t["transition_keys"])
			if headers_t["last_header"]:
				key_str =  key_str + "\n\t\t\t{1, 1, 0 IF_SOFTWARE(, \"Last Header\")}\n\t\t},\n\t},"
			else:
				for trans in headers_t["transition_keys"]:
					key_str =  key_str + "\n\t\t\t{" + str(trans[0]) + ", "					\
								+ str(trans[1]) + ", " + str(trans[3]) + " IF_SOFTWARE(, \""	\
								+ str(trans[4]) + "\")}"
					if it == num_keys - 1:
						key_str =  key_str + "\n\t\t},\n\t},"
					else:
						key_str =  key_str + ","
					it+=1
			parser_header_template.write(key_str + "\n")

			# Key position
			parser_header_template.write( "\tstd::pair<ap_uint<" + str(int.bit_length(headers_t["header_size_bits"]))   \
					+ ">, ap_uint<" + str(int.bit_length(headers_t["header_size_bits"]))				\
					+ ">>{" + str(headers_t["key_position"]) + "," + str(headers_t["key_size"]) + "}," + "\n")

			# Last Header
			parser_header_template.write("\t(" + string.lower(str(headers_t["last_header"])) + ")," + "\n")

			#Header Layout name
			parser_header_template.write("\tIF_SOFTWARE(\"" + headers_t["header_type_name"] + "\",)" + "\n")

			# Variable Size Flag
			parser_header_template.write("\t(" + string.lower(str(headers_t["var_size_header"])) + ")," + "\n")

			# Variable Size indicator position
			parser_header_template.write( "\tstd::pair<ap_uint<" + str(int.bit_length(headers_t["header_size_bits"]))   \
			    		+ ">, ap_uint<" + str(int.bit_length(headers_t["header_size_bits"]))				\
			    		+ ">>{" + str(headers_t["header_length_position"]) + "," + str(headers_t["header_length_size"]) + "}," + "\n")

                        # Initialize pipeline adjust for variable sized headers
			#parser_header_template.write( "\tstd::array<ap_uint<" + str(headers_t["len_bit_size"]) + ">, " + str(headers_t["header_length_larray_size"]) + ">({")
			parser_header_template.write("\t{{")
                        it = 0
                        for i in headers_t["header_length_lookup_array"]:
			    it+=1
                            parser_header_template.write(str(i))
                            if it < headers_t["header_length_larray_size"]:
			        parser_header_template.write(",")
			#parser_header_template.write("})" + "\n")
			parser_header_template.write("}}" + "\n")

                        # End of declaration
			parser_header_template.write("};" + "\n")


		parser_header_template.write("\n#endif //_PARSER_HEADER_TEMP_HPP_"  + "\n")

##############################################################
# Write the parser pipeline
##############################################################
def write_parse_pipeline(headers_list, bus_size, max_pkt_id_size, input_parser_state, max_supp_headers):
	##############
	# Parser.hpp #
	##############
	with open("Parser.hpp", "w") as Parser:
		Parser.write("/******************************************************************************" + "\n")
		Parser.write("* Packet Parser: header extraction and supported protocols graph              *" + "\n")
		Parser.write("* Jeferson Santiago da Silva (eng.jefersonsantiago@gmail.com)                 *" + "\n")
		Parser.write("******************************************************************************/" + "\n")

		Parser.write("\n#include \"Header.hpp\"" + "\n")
		Parser.write("\n#include \"parser_header_template.hpp\"" + "\n")

		Parser.write("\n#ifndef _PARSER_HEADER_HPP_" + "\n")
		Parser.write("\n#define _PARSER_HEADER_HPP_" + "\n")


		Parser.write("\n#define PKT_BUS_SIZE " + str(bus_size) + "\n")
		Parser.write("\n#define MAX_PKT_ID_SIZE " + str(max_pkt_id_size) + "\n")

		Parser.write("\nvoid HeaderAnalysisTop(const PacketData<PKT_BUS_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>& PacketIn")
		for headers_t in headers_list:
			header_name = headers_t["header_name"]
			header_name_cap = string.upper(header_name)
			Parser.write(",\n\tPHVData<" + header_name_cap + "_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>& " + header_name + "_PHV")
		Parser.write(",\n\tPacketData<PKT_BUS_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>& PacketOut);" + "\n")

		Parser.write("\n#endif //_PARSER_HEADER_HPP_" + "\n")

	##############
	# Parser.cpp #
	##############
	print "Generating Parser Pipeline..."
	with open("Parser.cpp", "w") as Parser:
		Parser.write("/******************************************************************************" + "\n")
		Parser.write("* Packet Parser: header extraction and supported protocols graph              *" + "\n")
		Parser.write("* Jeferson Santiago da Silva (eng.jefersonsantiago@gmail.com)                 *" + "\n")
		Parser.write("******************************************************************************/" + "\n")

		Parser.write("\n#include \"Parser.hpp\"" + "\n")

		Parser.write("\nvoid HeaderAnalysisTop(const PacketData<PKT_BUS_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>& PacketIn")
		for headers_t in headers_list:
			header_name = headers_t["header_name"]
			header_name_cap = string.upper(header_name)
			Parser.write(",\n\tPHVData<" + header_name_cap + "_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>& " + header_name + "_PHV")

		Parser.write(",\n\tPacketData<PKT_BUS_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>& PacketOut)" + "\n{\n")

		Parser.write("#pragma HLS INTERFACE ap_ctrl_none port=return" + "\n")
		Parser.write("#pragma HLS PIPELINE II=1" + "\n")

		Parser.write("\n\t// Wires" + "\n")
		Parser.write("\tstd::array<PacketData<PKT_BUS_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, MAX_SUPP_HEADERS + 1> tmpPacketOut;" + "\n")
		Parser.write("#pragma HLS ARRAY_PARTITION variable=tmpPacketOut dim=1" + "\n")
		Parser.write("\tstd::array<PacketData<PKT_BUS_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, MAX_SUPP_HEADERS + 1> tmpPacketIn;" + "\n")
		Parser.write("#pragma HLS ARRAY_PARTITION variable=tmpPacketIn dim=1" + "\n")

		Parser.write("\n\t// Stateful objects" + "\n")
		for headers_t in headers_list:
			header_name = headers_t["header_name"]
			header_name_cap = string.upper(header_name)
                        header_layout = "HeaderFormat"
                        if headers_t["var_size_header"]:
                            header_layout = header_name + "HeaderFormat"

                        Parser.write("\n#pragma HLS INTERFACE ap_ovld port=" + header_name + "_PHV")

			Parser.write("\n\tstatic Header<" + header_name_cap + "_HEADER_SIZE, " + header_name_cap + "_NUM_FIELDS, "\
							+ headers_t["key_type"] + ", " + str(headers_t["key_number"])\
							+ ", ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE, "\
                                                        + header_layout + "<" + header_name_cap + "_HEADER_SIZE, " + header_name_cap + "_NUM_FIELDS, "\
							+ headers_t["key_type"] + ", " + str(headers_t["key_number"]) + ", MAX_SUPP_HEADERS," + str(headers_t["header_length_larray_size"]) \
                                                        + ", " + str(headers_t["len_bit_size"]) + ">, " + str(headers_t["header_length_larray_size"]) + "> "\
							+ header_name + "(IF_SOFTWARE(\"" + header_name + "\",) " + str(headers_t["parser_state_id"]) + ", "\
							+ headers_t["header_type_name"] + ");" + "\n")

			Parser.write("\n\tPHVData<" + header_name_cap + "_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> wire_" + header_name + "_PHV;" + "\n")
			Parser.write("\n\tstatic std::array<PHVData<" + header_name_cap + "_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, MAX_SUPP_HEADERS + 1> tmp_" + header_name + "_PHV;" + "\n")
			Parser.write("#pragma HLS ARRAY_PARTITION variable=tmp_" + header_name + "_PHV dim=1" + "\n")
			Parser.write("#pragma HLS DEPENDENCE variable=tmp_" + header_name + "_PHV false" + "\n")

		for headers_t in headers_list:
			header_name = headers_t["header_name"]
			header_name_cap = string.upper(header_name)
			header_state_id = headers_t["parser_state_id"]

			if headers_t["parser_state_id"] == input_parser_state:
                            Parser.write("\n\ttmpPacketIn[" + str(input_parser_state) + "] = PacketIn;" + "\n")
			else:
			    if len(headers_t["previous_state_id"]) == 1:
			        Parser.write("\n\ttmpPacketIn[" + str(header_state_id) + "] = tmpPacketOut[" + str(headers_t["previous_state_id"][0]) +"];" + "\n")
			    else:
                                multi_level_parent = False
                                str_bypass = ""
                                reg_str_bypass = ""
                                bypass_id = 0
                                reg_bypass_id = 0
                                tt = []
                                t = []
				for previous_states in headers_t["previous_state_both"]:
				    for headers_tt in headers_list:
                                        if headers_tt["previous_state_both"] != headers_t["previous_state_both"]:
				            for previous_states_tt in headers_tt["previous_state_both"]:
                                                if previous_states_tt == previous_states:
                                                    for next_state_tt in headers_tt["next_state"]:
                                                        if next_state_tt == headers_t["header_name"]:
                                                            multi_level_parent = True
				                            tt.append("Found nodes in different levels with same parent: " + headers_t["header_name"] + " and " + headers_tt["header_name"] + \
                                                                    ". Using " + headers_tt["header_name"] + " as a bypass node")
                                                            t.append("\n\ttmpPacketIn[" + str(header_state_id) + "] = tmpPacketOut[" + str(headers_tt["parser_state_id"]) +"];" + "\n")
 
                                    if not multi_level_parent:
				        Parser.write("\n\tif (tmp_" + str(previous_states[0]) + "_PHV[" + str(previous_states[1]) +"].Valid)" + "\n")
				        Parser.write("\n\t\ttmpPacketIn[" + str(header_state_id) + "] = tmpPacketOut[" + str(previous_states[1]) +"];" + "\n")
                                if multi_level_parent:
                                    print(tt[len(tt) - 1])
                                    Parser.write(t[len(t)-1])

			Parser.write("\n\t" + header_name + ".HeaderAnalysis(tmpPacketIn[" + str(header_state_id) + "], " \
						"tmp_" + header_name + "_PHV[" + str(header_state_id) + "], tmpPacketOut[" + str(header_state_id) + "]);" + "\n")

			Parser.write("\n\tfor (auto i = " + str(header_state_id) + "; i < MAX_SUPP_HEADERS; ++i)"\
							"\n\t\ttmp_" + header_name + "_PHV[i+1] = tmp_" + header_name + "_PHV[i];" + "\n")

			Parser.write("\n\t" + header_name + "_PHV = tmp_" + header_name + "_PHV[MAX_SUPP_HEADERS];" + "\n")

                for headers_t in headers_list:
		    if headers_t["last_header"]:
		        header_name = headers_t["header_name"]
		        header_state_id = headers_t["parser_state_id"]
		        Parser.write("\n\tif (tmp_" + str(header_name) + "_PHV[" + str(header_state_id) +"].Valid)" + "\n")
		        Parser.write("\n\t\tPacketOut = tmpPacketOut[" + str(header_state_id) +"];" + "\n")


#		for headers_t in headers_list:
#			header_name = headers_t["header_name"]
#			header_name_cap = string.upper(header_name)
#			header_state_id = headers_t["parser_state_id"]
#
#			if headers_t["parser_state_id"] == input_parser_state:
#				Parser.write("\n\ttmpPacketIn[" + str(input_parser_state) + "] = PacketIn;" + "\n")
#			else:
#				Parser.write("\n\ttmpPacketIn[" + str(header_state_id) + "] = tmpPacketOut[" + str(header_state_id - 1) +"];" + "\n")
#			#Parser.write("\n\t" + header_name + ".HeaderAnalysis(tmpPacketIn[" + str(header_state_id) + "], " \
#			#			+ header_name + "_PHV, tmpPacketOut[" + str(header_state_id) + "]);" + "\n")
#
#                        ## aqui
#
#			Parser.write("\n\t" + header_name + ".HeaderAnalysis(tmpPacketIn[" + str(header_state_id) + "], " \
#						"tmp_" + header_name + "_PHV[" + str(header_state_id) + "], tmpPacketOut[" + str(header_state_id) + "]);" + "\n")
#
#			Parser.write("\n\tfor (auto i = " + str(header_state_id) + "; i < MAX_SUPP_HEADERS; ++i)"\
#							"\n\t\ttmp_" + header_name + "_PHV[i+1] = tmp_" + header_name + "_PHV[i];" + "\n")
#
#
#			Parser.write("\n\t" + header_name + "_PHV = tmp_" + header_name + "_PHV[MAX_SUPP_HEADERS];" + "\n")
#
#		Parser.write("\n\tPacketOut = tmpPacketOut[" + str(header_state_id) +"];" + "\n")

		Parser.write("\n}\n")

##############################################################
# Main
##############################################################
def main():

	if len(sys.argv) != 4:
		param = len(sys.argv) - 1
		print "Invalid parameter number. Expected 3 parameters, got %d" % param
		print "Usage python generate_header_parser.py <json_file> <bus_size> <max_pkt_id_size>"
		return

	json_file = sys.argv[1]
	bus_size = sys.argv[2]
	max_pkt_id_size = sys.argv[3]

	headers_list, input_parser_state, max_supp_headers, max_header_size, header_num, header_size_avg  = json_parser(json_file)
	write_headers_template(headers_list, max_supp_headers, max_header_size, header_num)
	write_parse_pipeline(headers_list, bus_size, max_pkt_id_size, input_parser_state, max_supp_headers)

	print "Parsing Done"

if __name__ == '__main__':
	main()
