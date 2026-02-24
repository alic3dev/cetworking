name=cetworking

directory_examples=examples
directory_include=include
directory_library=library
directory_objects=objects
directory_sources=sources

file_output=${directory_library}/${name}

files_sources=${shell find ${directory_sources} -name \*.c}
files_objects=${patsubst ${directory_sources}/%.c,${directory_objects}/%.o,${files_sources}}

cc=clang
c_flags=-O3 -I${directory_include}

ld=ld
ld_flags=

strip=strip
strip_flags=-x

${name}: ${file_output}

all: ${file_output} examples

examples: .always
	cd ${directory_examples} && make

${file_output}: ${files_objects} ${directory_library}
	${ld} ${ld_flags} -r ${files_objects} -o ${file_output}
	${strip} ${strip_flags} ${file_output}

${directory_objects}/%.o: ${directory_sources}/%.c
	${shell dirname="${patsubst ${shell pwd}/%,%,${abspath ${dir $@}}}" && if [[ ! -d $${dirname} ]]; then printf "mkdir -p $${dirname}\n" && mkdir -p $${dirname}; fi}
	${cc} ${c_flags} -c $< -o $@

${directory_library}:
	mkdir -p ${directory_library}

${directory_objects}:
	mkdir -p ${directory_objects}

clean_all: clean clean_examples

clean: clean_library clean_objects

clean_examples:
	cd ${directory_examples} && make clean

clean_library:
	-rm -r ${directory_library} 2> /dev/null

clean_objects:
	-rm -r ${directory_objects} 2> /dev/null

.always:
