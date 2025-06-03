CXX = g++
CXXFLAGS = -std=c++17 -pthread

TARGET = ftpBrute 

SRCS = main.cpp \
       FTPClient/FTPClient.cpp \
       Manager/Manager.cpp

INCLUDES = -IFTPClient -IManager

all:
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SRCS) -o $(TARGET)
clean:
	rm -f $(TARGET)

.PHONY: all clean
