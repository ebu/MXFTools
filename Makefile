all : 
	c++ -std=c++11 -o demo-linux64 *.cpp `pkg-config --libs --cflags gtkmm-3.0 gdkmm-3.0 xerces-c libMXF-1.0 libMXF++-1.0 bmx-0.1 ebu-mxfsdk-1.0` -W -Wall
	
clean :
	rm *~
	rm demo-linux64
