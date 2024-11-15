# Build the global router

# Command for continuous build: (TO AUTOMATE)
# fswatch -0 -r -l 5 -e "build." . | xargs -0 -n 1 -I {} "make"

BUILD_DIR ?= $(CURDIR)/build

.PHONY : build
build:
	@echo "🟢 Updating CMake build files..."
	@mkdir -p ${BUILD_DIR}
	@cd ${BUILD_DIR}; cmake ..
	@echo "🟢 Building project..."
	@cmake --build ${BUILD_DIR} 
	@echo
	@date +"🟢 %T - Build finished"

.PHONY : clean
clean:
	rm -r ${BUILD_DIR}

.PHONY : bundle
bundle:
	cd ..; tar  --exclude='./.git/' --exclude="./.cache/" --exclude="./goldenRef" --exclude="./benchmarks" --exclude="./build/" -cvzf gr.tar.gz -C ${CURDIR} .
