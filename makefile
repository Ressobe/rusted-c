CXX = g++
CXXFLAGS = -std=c++11 -Wall
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

$(BINDIR)/rusted-c: $(OBJDIR)/ast/AST.o $(OBJDIR)/lexer/Lexer.o $(OBJDIR)/parser/Parser.o $(OBJDIR)/runtime/environment/Environment.o $(OBJDIR)/runtime/interpreter/Interpreter.o $(OBJDIR)/runtime/interpreter/expression/EvaluateExpression.o $(OBJDIR)/runtime/interpreter/statement/EvaluateStatement.o $(OBJDIR)/runtime/values/Values.o $(OBJDIR)/main.o
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

$(OBJDIR)/runtime/environment/Environment.o: $(ENVDIR)/Environment.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)/runtime/interpreter/Interpreter.o: $(INTERPRETERDIR)/Interpreter.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)/runtime/interpreter/expression/EvaluateExpression.o: $(INTERPRETERDIR)/expression/EvaluateExpression.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)/runtime/interpreter/statement/EvaluateStatement.o: $(INTERPRETERDIR)/statement/EvaluateStatement.cpp
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

