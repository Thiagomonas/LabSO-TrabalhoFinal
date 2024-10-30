CC = gcc
CFLAGS = -Wall
OBJFILES = aviao.o main.o
TARGET = jogo

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES)

clean:
	rm -f $(OBJFILES) $(TARGET) *~