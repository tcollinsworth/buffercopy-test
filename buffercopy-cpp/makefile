CC = g++
# -DDEBUG=true turns on debug output
# -g is debug flag
# -O -O1 -O2 -O3 are progressively more aggressive optimization flags
# http://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html
#INC1      = /usr/src/linux-headers-5.0.0-37/arch/x86/include
#INC2      = /usr/src/linux-headers-5.0.0-37/include/
#INC3      = /usr/src/linux-headers-5.0.0-37/arch/x86/include/asm
#INCDIRS   = #-I${INC1} -I${INC2} -I${INC3}
CFLAGS=-c -O3 -Wall ${INCDIRS} #-fwhole-program -flto #-funroll-loops # -shared-intel
#LDFLAGS='-pthread'
SOURCES=buffercopy.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=buffercopy

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@


