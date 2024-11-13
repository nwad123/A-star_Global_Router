# Build the global router

BUILD_DIR ?= $(CURDIR)/build

.PHONY : build
build:
	mkdir -p ${BUILD_DIR}
	cd ${BUILD_DIR}; cmake ..
	cmake --build ${BUILD_DIR} 

.PHONY : release
release:
	mkdir -p ${BUILD_DIR}
	cd ${BUILD_DIR}; cmake -DCMAKE_BUILD_TYPE=Release ..
	cmake --build ${BUILD_DIR} 

.PHONY : clean
clean:
	rm -r ${BUILD_DIR}

.PHONY : bundle
bundle:
	cd ..; tar  --exclude='./.git/' --exclude="./.cache/" --exclude="./goldenRef" --exclude="./benchmarks" --exclude="./build/" -cvzf gr.tar.gz -C ${CURDIR} .
