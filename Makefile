FILENAME = main
BUILD = build
CXXFLAGS = -std=c++11 -O3 -Wc++11-extensions
INCLUDE = -I /opt/homebrew/Cellar/boost/1.86.0_2/include

all: run 

run: $(FILENAME).cpp $(BUILD)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $< -o $(BUILD)/$(FILENAME)
	$(BUILD)/$(FILENAME) files/test.bin \
	files/diff_10.bin \
	files/diff_20.bin \
	files/diff_30.bin \
	files/diff_70.bin \
	files/diff_80.bin

gen:
	python3 generate_files.py \
	files/test.bin \
	50000 \
	0.1 0.2 0.3 0.7 0.8
 

gen_small:
	python3 generate_files.py \
	files/test.bin \
	1000000 \
	0.45 0.49 0.50 0.51 0.55

run_small: $(FILENAME).cpp $(BUILD)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $< -o $(BUILD)/$(FILENAME)
	$(BUILD)/$(FILENAME) files/test.bin \
	files/diff_45.bin \
	files/diff_49.bin \
	files/diff_50.bin \
	files/diff_51.bin \
	files/diff_55.bin >> count

$(BUILD): 
	@mkdir  -p $(BUILD)



