FILENAME = main
BUILD = build
CXXFLAGS = -std=c++11 -O3 -Wc++11-extensions
INCLUDE = -I /opt/homebrew/Cellar/boost/1.86.0_2/include

all: run 

run: $(FILENAME).cpp $(BUILD)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $< -o $(BUILD)/$(FILENAME)
	$(BUILD)/$(FILENAME) files/test.bin \
	files/diff_20.bin \
	files/diff_30.bin \
	files/diff_70.bin \
	files/diff_80.bin

gen:
	python3 generate_files.py \
	files/test.bin \
	80000 \
	0.2 0.3 0.7 0.8
 
$(BUILD): 
	@mkdir  -p $(BUILD)



