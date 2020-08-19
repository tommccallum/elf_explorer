#!/usr/bin/python3

import os
import typing

def write_struct(in_filename:str, out_filename:str, name:str, namespace:str ):
    with open(in_filename) as infh:
        contents = infh.readlines()
    
    c_struct = []
    for line in contents:
        line = line.strip()
        what=0
        buffer = ""
        hex_num = False
        item = [] # collect items
        for c in line:
            if c == '\t':
                c = ' '
            if what == 0:
                if c.isalpha() or c == '_' or c.isdigit():
                   buffer += c
                if c == ' ' and buffer != "": 
                    item.append(buffer)
                    buffer = ""
                    what = 1
            elif what == 1:
                if c.isdigit():
                    buffer += c
                elif c == '-' and buffer != "":
                    # this is a range
                    item.append(buffer)
                    buffer = ""
                elif c == ' ' and buffer != "":
                    item.append(buffer)
                    buffer = ""
                elif c == ' ' or c == '-':
                    # ignore
                    1==1
                elif c == 'x' and buffer=="0":
                    hex_num=True
                    buffer += c
                elif hex_num and c in "ABCDEFabcdef":
                    buffer += c.upper()
                elif c.isalpha():
                    what = 2
                    buffer += c
                elif c != ' ':
                    what = 2
                    buffer += c
            elif what == 2:
                buffer += c
        if what == 1: # we had something like 242-256\n
            item.append(buffer)
            buffer = "Description not available"
        item.append(buffer)
        print(item)
        c_struct.append(item)
    
    c_code = "#ifndef {}_HPP\n#define {}_HPP\n\n".format(name.upper(), name.upper())
    c_code += "// This file is automatically generated, do not edit.\n\n"
    c_code += "#include <string>\n\n";
    c_code += "namespace {} {{\n\n".format(namespace)
    c_code += "enum {} {{\n".format(name);
    counter = 0
    reserved_counter=0;
    name_mapping = []
    for row in c_struct:
        comma = ""
        if counter < len(c_struct)-1:
            comma = ","
        if len(row) == 2:
            enum_name = row[0]
            if row[0].lower() == "reserved":
                enum_name = name+"_"+enum_name+"_"+str(reserved_counter)
                reserved_counter += 1
            name_mapping.append([enum_name,row[1]])
            c_code += "    {:<40s}= {:<10s}\n".format(enum_name,row[1]+comma);
        elif len(row) == 3:
            enum_name = row[0]
            if row[0].lower() == "reserved":
                enum_name = name+"_"+enum_name+"_"+str(reserved_counter)
                reserved_counter += 1
            name_mapping.append([enum_name,row[1]])
            c_code += "    {:<40s}= {:<10s} // {}\n".format(enum_name,row[1]+comma,row[2]);
        elif len(row) == 4:
            for a in range(int(row[1]), int(row[2])+1):
                enum_name = row[0]+"_"+str(a)
                if row[0].lower() == "reserved":
                    enum_name = name+"_"+enum_name
                name_mapping.append([enum_name,a])
                c_code += "    {:<40s}= {:<10s} // {}\n".format(enum_name,str(a)+comma,row[3]);
        else:
            raise ValueError("invalid number of columns in row {}".format(row))
        counter += 1
    c_code += "};\n\n"

    # add a function for taking a int64_t value and returning a string of the enum
    fn_code = "std::string {}_to_string(int64_t value)\n{{\n".format(name)
    fn_code += "  switch(value) {\n"
    for row in name_mapping:
        fn_code += "    case {}:\n".format(row[1])
        fn_code += "      return \"{}\";\n".format(row[0])
    fn_code += "    default:\n"
    fn_code += "      return \"unknown\";\n"
    fn_code += "  }\n}\n"

    c_code += fn_code
    c_code += "\n\n}} // end of namespace {}\n\n".format(namespace)
    c_code += "#endif // end of {}_HPP\n".format(name.upper())
    with open(out_filename, "w") as outfh:
        outfh.write(c_code);
    


if __name__ == "__main__":
    write_struct("machines.txt", "machines.hpp", "e_machines","elf");
    write_struct("section_attribute_flags.txt", "section_attribute_flags.hpp", "e_sh_flags","elf");
    write_struct("section_types.txt", "section_types.hpp", "e_section_types", "elf");
    