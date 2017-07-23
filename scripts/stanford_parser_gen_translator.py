import string

def gen_stanford_parser(headers_list):
    print "Generating Stanford Parser Layout..."
    with open("stanford_parser_def", "w") as parser_header_template:
        for headers_t in headers_list:
            header_name = headers_t["header_name"]
            header_name_cap = string.upper(header_name)
            parser_header_template.write("\n" + header_name + "{\n")

            parser_header_template.write("\tfields{\n")

            for field in range(headers_t["num_fields"]):
                parser_header_template.write("\n\t\t" + str(headers_t["fields"][field][0])\
                                                + ":" + str(headers_t["fields"][field][2]) + ": extract,")
            parser_header_template.write("\n\t}\n")

            if len(headers_t["transition_keys"]) > 0:
                parser_header_template.write("\tnext_header = map(" + headers_t["key_name"] + "){\n")
                for trans in headers_t["transition_keys"]:
                    parser_header_template.write("\t\t" + str(trans[0]) + ":" + str(trans[4]) + ",\n")
                parser_header_template.write("\t}\n")

            if headers_t["var_size_header"]:
                parser_header_template.write("\t\tlength = " + str(headers_t["header_length_expression"]) + "\n")
                parser_header_template.write("\t\tmax_length = " + str(headers_t["header_size"]*8) + "\n")

            parser_header_template.write("}\n")
