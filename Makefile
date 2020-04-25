CC := g++ #Current Compiler
CFLAGS := -std=c++11 #-Wall #Special options for compiler

BIN		:= bin#Folder that contains the output executables
BUILD	:= build#Folder that contains all the object files - are removed on clean
SRC     := src#Contains the application and the application's source files
TEST	:= test#Contains the unit tests
INCLUDE := include#Contains all the project header files
LIB     := lib
LIBRARIES   := 
TARGET  := clusterer#Name of the executable
TESTTARGET := tests
SRCEXT := cpp

SOURCES := $(shell find $(SRC) -type f -name "*.$(SRCEXT)")
OBJECTS := $(patsubst $(SRC)/%,$(BUILD)/%,$(SOURCES:.$(SRCEXT)=.o))

#Linking Object Files
$(BIN)/$(TARGET): $(OBJECTS)
	@echo "🔗 Linking Object Files..."
	$(CC) $(OBJECTS) -o $(BIN)/$(TARGET) -std=c++11 $(LIBS)
#Compiling Object Files
$(BUILD)/%.o : $(SRC)/%.cpp   
	@echo "🚧 Building (Compiling Object Files)..."
	$(CC) -c $< -o $@ -std=c++11

run:
	clear
	@echo "🚀 Executing..."
	./$(BIN)/$(TARGET) Gradient_Numbers_PPMS -o testoutput -k 10 -bin 1

tests:
	@echo "Compiling for Tests..."
	#$(CC) $(CFLAGS) $(SRC)/Person.cpp -c -o $(BUILD)/Person.o
	$(CC) $(CFLAGS) $(TEST)/UnitTests.cpp -c -o $(BUILD)/UnitTests.o
	@echo "Compilation Complete."
	@echo "Linking Object Files..."
	$(CC) $(CFLAGS) $(BUILD)/Person.o $(BUILD)/UnitTests.o -o $(BIN)/$(TESTTARGET)
	@echo "Linking Complete."

runTests:
	./$(BIN)/$(TESTTARGET)

# deletes all the object code files as well as the previously generated outputs
clean:
	@echo "🧹 Cleaning..."
	@rm -f $(BUILD)/*.o
	@rm -f $(BIN)/$(TARGET)
	@rm -f $(BIN)/$(TESTTARGET)
	@echo "🏁 Cleaning Complete."