# Makefile para el Parser LL(1)

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -I../
SRCDIR = .
OBJDIR = obj
BINDIR = bin

# Archivos fuente de la librería del parser (sin los tests ni gramáticas específicas)
PARSER_LIB_SOURCES = ll1_grammar.cpp ll1_parser.cpp
PARSER_LIB_OBJECTS = $(PARSER_LIB_SOURCES:%.cpp=$(OBJDIR)/%.o)

# Archivos fuente de las gramáticas
GRAMMAR_SOURCES = simple_hulk_grammar.cpp intermediate_hulk_grammar.cpp full_hulk_grammar.cpp full_hulk_grammar_v2.cpp full_hulk_grammar_v3.cpp
GRAMMAR_OBJECTS = $(GRAMMAR_SOURCES:%.cpp=$(OBJDIR)/%.o)

# Ejecutables de prueba y sus dependencias específicas
TARGET_MAIN = $(BINDIR)/test_ll1
TARGET_LET = $(BINDIR)/test_let
TARGET_LET_SUCCESS = $(BINDIR)/test_let_success
TARGET_OPERATORS = $(BINDIR)/test_operators
TARGET_FULL_V2 = $(BINDIR)/test_full_v2
TARGET_FULL_V3 = $(BINDIR)/test_full_v3

TARGETS = $(TARGET_MAIN) $(TARGET_LET) $(TARGET_LET_SUCCESS) $(TARGET_OPERATORS) $(TARGET_FULL_V2) $(TARGET_FULL_V3)

.PHONY: all clean

all: $(TARGETS)

# Reglas de compilación para cada test
$(TARGET_MAIN): $(OBJDIR)/test_ll1.o $(PARSER_LIB_OBJECTS) $(GRAMMAR_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TARGET_LET): $(OBJDIR)/test_let.o $(PARSER_LIB_OBJECTS) $(OBJDIR)/intermediate_hulk_grammar.o
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TARGET_LET_SUCCESS): $(OBJDIR)/test_let_success.o $(PARSER_LIB_OBJECTS) $(OBJDIR)/intermediate_hulk_grammar.o
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TARGET_OPERATORS): $(OBJDIR)/test_operators.o $(PARSER_LIB_OBJECTS) $(OBJDIR)/intermediate_hulk_grammar.o
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TARGET_FULL_V2): $(OBJDIR)/test_full_v2.o $(PARSER_LIB_OBJECTS) $(OBJDIR)/full_hulk_grammar_v2.o $(OBJDIR)/intermediate_hulk_grammar.o
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TARGET_FULL_V3): $(OBJDIR)/test_full_v3.o $(PARSER_LIB_OBJECTS) $(OBJDIR)/full_hulk_grammar_v3.o
	$(CXX) $(CXXFLAGS) -o $@ $^


# Regla genérica para compilar archivos fuente a objetos
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

test: $(TARGET_MAIN)
	./$(TARGET_MAIN)

test-let: $(TARGET_LET)
	./$(TARGET_LET)

test-let-success: $(TARGET_LET_SUCCESS)
	./$(TARGET_LET_SUCCESS)

test-operators: $(TARGET_OPERATORS)
	./$(TARGET_OPERATORS)

test-full-v2: $(TARGET_FULL_V2)
	./$(TARGET_FULL_V2)

test-full-v3: $(TARGET_FULL_V3)
	./$(TARGET_FULL_V3)

clean:
	rm -rf $(OBJDIR) $(BINDIR)

# Recrear directorios después de limpiar
setup:
	mkdir -p $(OBJDIR)
	mkdir -p $(BINDIR)
