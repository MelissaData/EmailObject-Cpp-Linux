# Melissa Email Object Linux C++

CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++11 -g
LDLIBS = -lmdEmail
LDFLAGS = -L/home/emaiiii/MdLinuxExamples/melissaemailobjectlinuxcpp/MelissaEmailObjectLinuxCpp/Build
RM = rm -f

OUT_DIR = Build

MelissaEmailObjectLinuxCpp: Main.cpp EmailObject.cpp mdEmail.h mdEnums.h | $(OUT_DIR)
	$(CXX) $(CXXFLAGS) Main.cpp EmailObject.cpp $(LDFLAGS) $(LDLIBS) -o $(OUT_DIR)/$@

clean:
	$(RM) *.o $(OUT_DIR)/MelissaEmailObjectLinuxCpp

