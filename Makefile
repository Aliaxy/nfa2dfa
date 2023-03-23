TARGET := main
SRC := main.cpp

CC := $(CXX)
CPPFLAGS := -std=c++17

$(TARGET) : $(SRC)

run :
	./$(TARGET)

.PHONY : clean

clean :
	$(RM) $(TARGET)