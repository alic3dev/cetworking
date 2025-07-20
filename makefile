directory_base=.

prefix_directory=__directory__
prefix_directory_clean=$(prefix_directory)__clean__

directories=$(dir $(wildcard $(directory_base)/*/makefile))

directories_prefixed=$(addprefix $(prefix_directory)/, $(directories))
directories_prefixed_clean=$(addprefix $(prefix_directory_clean)/, $(directories))

all: $(directories_prefixed)

clean: clean_all
clean_all: $(directories_prefixed_clean)

$(prefix_directory)/%: %
	cd $< && if [[ -z "$$(cat makefile | grep "all:")" ]]; then make; else make all; fi

$(prefix_directory_clean)/%: %
	cd $< && if [[ -z "$$(cat makefile | grep "clean_all:")" ]]; then make clean; else make clean_all; fi
