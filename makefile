CC := g++
SRCDIR := src
BUILDDIR := build
TARGET := bin/libI2C.so

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g -std=c++14 -fPIC
LIB := -lsemaeapi -L/opt/Sema/lib `python-config --cflags --ldflags`
INC := -Iinclude -I/opt/Sema/include

$(TARGET): $(OBJECTS)
#	@echo "Linking..."
	$(CC) $(CFLAGS) $(INC) -I/opt/pybind11/include `python-config --cflags --ldflags` -c -o $(BUILDDIR)/libI2C.o libsrc/libI2C.cpp 
	@echo "$(CC) -shared $^ $(BUILDDIR)/libI2C.o -o $(TARGET) $(LIB)"; $(CC) -shared $^ $(BUILDDIR)/libI2C.o -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo "$(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo "Cleaning...";
	@echo "$(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)
