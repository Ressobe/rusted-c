CXX = g++
CXXFLAGS = -std=c++17 -Wall -lpq -lpqxx
SRCDIR = .
OBJDIR = obj
BINDIR = bin
ASTDIR = $(SRCDIR)/ast
LEXERDIR = $(SRCDIR)/lexer
PARSERDIR = $(SRCDIR)/parser
DATABASEDIR = $(SRCDIR)/database
ENVDIR = $(SRCDIR)/runtime/environment
INTERPRETERDIR = $(SRCDIR)/runtime/interpreter
VALUESDIR = $(SRCDIR)/runtime/values
STANDARDLIBDIR = $(SRCDIR)/runtime/standard-library

# Lista plików źródłowych
SOURCES = $(wildcard $(SRCDIR)/*.cpp $(ASTDIR)/*.cpp $(LEXERDIR)/*.cpp $(PARSERDIR)/*.cpp $(ENVDIR)/*.cpp $(INTERPRETERDIR)/*.cpp $(VALUESDIR)/*.cpp $(STANDARDLIBDIR)/*.cpp $(DATABASEDIR)/*.cpp)

# Lista plików obiektowych
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))

# Cel główny
all: $(BINDIR)/rustedc

# Cel końcowy
$(BINDIR)/rustedc: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Reguła dla plików obiektowych
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Reguła dla plików obiektowych z podkatalogów
$(OBJDIR)/%.o: $(ASTDIR)/%.cpp $(LEXERDIR)/%.cpp $(PARSERDIR)/%.cpp $(ENVDIR)/%.cpp $(INTERPRETERDIR)/%.cpp $(VALUESDIR)/%.cpp $(STANDARDLIBDIR)/%.cpp $(DATABASEDIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Czyszczenie plików tymczasowych
clean:
	rm -rf $(OBJDIR) $(BINDIR)

# Oznaczenie celu 'all' oraz 'clean' jako phony, żeby Makefile nie szukał plików o takich nazwach
.PHONY: all clean
