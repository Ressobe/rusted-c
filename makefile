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
STANDARDLIBDIR = $(SRCDIR)/runtime/standard-library

all: $(BINDIR)/rustedc

$(BINDIR)/rustedc: $(OBJDIR)/ast/AST.o $(OBJDIR)/lexer/Lexer.o $(OBJDIR)/parser/Parser.o $(OBJDIR)/parser/ParserStmt.o $(OBJDIR)/parser/ParserExpr.o  $(OBJDIR)/runtime/environment/Environment.o  $(OBJDIR)/runtime/values/Values.o $(OBJDIR)/runtime/standard-library/BuiltinFunctions.o $(OBJDIR)/runtime/interpreter/Interpreter.o $(OBJDIR)/runtime/interpreter/InterpreterExpr.o $(OBJDIR)/runtime/interpreter/InterpreterStmt.o $(OBJDIR)/main.o
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

$(OBJDIR)/runtime/environment/Environment.o: $(ENVDIR)/Environment.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)/runtime/interpreter/Interpreter.o: $(INTERPRETERDIR)/Interpreter.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)/runtime/interpreter/InterpreterExpr.o: $(INTERPRETERDIR)/InterpreterExpr.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)/runtime/interpreter/InterpreterStmt.o: $(INTERPRETERDIR)/InterpreterStmt.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)/runtime/standard-library/BuiltinFunctions.o: $(SRCDIR)/runtime/standard-library/BuiltinFunctions.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)/runtime/values/Values.o: $(VALUESDIR)/Values.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)/main.o: $(SRCDIR)/main.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all clean
