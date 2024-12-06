FILENAME = main
BUILD = build

all: run 

run: $(FILENAME).cpp $(BUILD)
	$(CXX) $(CXXFLAGS) $< -o $(BUILD)/$(FILENAME)
	$(BUILD)/$(FILENAME) files/test.bin files/diff_20.bin files/diff_30.bin files/diff_70.bin
 
$(BUILD): 
	@mkdir  -p $(BUILD)



