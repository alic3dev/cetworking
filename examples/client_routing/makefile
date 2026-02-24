name=client_routing

directory_output=output
directory_include=include
directory_objects=objects
directory_sources=sources

directory_clic3=../../../clic3
directory_clic3_include=${directory_clic3}/include
directory_clic3_library=${directory_clic3}/library

file_output=${directory_output}/${name}
file_clic3_library=${directory_clic3_library}/clic3.o

files_sources=${wildcard ${directory_sources}/*.c}
files_objects=${patsubst ${directory_sources}/%.c,${directory_objects}/%.o,${files_sources}}

cc=clang
c_flags=-I${directory_include} -I${directory_clic3_include} -O3

${file_output}: ${files_objects} ${directory_output}
	${cc} ${c_flags} ${files_objects} ${file_clic3_library} -o ${file_output}

${directory_objects}/%.o: ${directory_sources}/%.c ${directory_objects}
	${cc} ${c_flags} -c $< -o $@

directories: ${directory_objects} ${directory_output}

${directory_objects}:
	mkdir -p ${directory_objects}

${directory_output}:
	mkdir -p ${directory_output}

clean: clean_objects clean_output

clean_objects:
	-rm -r ${directory_objects}

clean_output:
	-rm -r ${directory_output}
