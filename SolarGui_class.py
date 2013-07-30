import sys
name=raw_input("Name files:")
nameupper=name.upper()

#make h file
ofile = open("SolarGui/"+name+".h","w")
ofile.write("#ifndef "+nameupper+"_H\n")
ofile.write("#define "+nameupper+"_H\n\n")
ofile.write("namespace SolarSystem {\n\n")
ofile.write("\tclass "+name+" {\n\n")
ofile.write("\tpublic:\n\n")
ofile.write("\t\t"+name+"();\n\n")
ofile.write("\t};\n\n")
ofile.write("};\n\n")
ofile.write("#endif")
ofile.close()
#make cppfile
ofile = open("SolarGui/"+name+".cpp","w")
ofile.write("#include <stdafx.h>\n")
ofile.write("#include <"+name+".h>\n\n")
ofile.write("///////////////////////\n")
ofile.write("using namespace SolarSystem;\n")
ofile.write("///////////////////////\n\n")
ofile.close()