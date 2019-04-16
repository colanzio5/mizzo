CC=g++ # define the compiler to use
TARGET=mizzo # define the name of the executable
SOURCES=main.cpp mizzo.cpp
CFLAGS=-O3 
LFLAGS=

# define list of objects
OBJSC=$(SOURCES:.c=.o)
OBJS=$(OBJSC:.cpp=.o)

# the target is obtained linking all .o files
all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $(TARGET) -pthread

purge: clean
	rm -f $(TARGET)

clean:
	rm -f *.o