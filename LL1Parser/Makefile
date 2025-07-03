# Makefile para el Parser LL(1)

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -I../
SRCDIR = .
OBJDIR = obj
BINDIR = bin

# Archivos fuente
SOURCES = ll1_grammar.cpp ll1_parser.cpp hulk_grammar.cpp simple_hulk_grammar.cpp test_ll1.cpp
OBJECTS = $(SOURCES:%.cpp=$(OBJDIR)/%.o)

# Ejecutable de prueba
TARGET = $(BINDIR)/test_ll1

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CXX) $(OBJECTS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

test: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(OBJDIR) $(BINDIR)

# Dependencias específicas
$(OBJDIR)/ll1_grammar.o: ll1_grammar.cpp ll1_grammar.hpp
$(OBJDIR)/ll1_parser.o: ll1_parser.cpp ll1_parser.hpp ll1_grammar.hpp
$(OBJDIR)/hulk_grammar.o: hulk_grammar.cpp ll1_parser.hpp
$(OBJDIR)/test_ll1.o: test_ll1.cpp ll1_grammar.hpp ll1_parser.hpp
