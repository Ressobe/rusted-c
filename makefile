CXX = g++
CXXFLAGS = -std=c++17 -Wall
SRCDIR = .
OBJDIR = obj
BINDIR = bin
ASTDIR = $(SRCDIR)/ast
LEXERDIR = $(SRCDIR)/lexer
PARSERDIR = $(SRCDIR)/parser
ENVDIR = $(SRCDIR)/runtime/environment
INTERPRETERDIR = $(SRCDIR)/runtime/interpreter
VALUESDIR = $(SRCDIR)/runtime/values

all: $(BINDIR)/rusted-c

$(BINDIR)/rusted-c: $(OBJDIR)/ast/AST.o $(OBJDIR)/lexer/Lexer.o $(OBJDIR)/parser/Parser.o $(OBJDIR)/parser/ParserStmt.o $(OBJDIR)/parser/ParserExpr.o  $(OBJDIR)/main.o
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJDIR)/ast/AST.o: $(ASTDIR)/AST.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)/lexer/Lexer.o: $(LEXERDIR)/Lexer.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)/parser/Parser.o: $(PARSERDIR)/Parser.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)/parser/ParserStmt.o: $(PARSERDIR)/ParserStmt.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)/parser/ParserExpr.o: $(PARSERDIR)/ParserExpr.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<


$(OBJDIR)/main.o: $(SRCDIR)/main.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all clean
