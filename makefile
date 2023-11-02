CXX = g++
CXXFLAGS = -std=c++11 -Wall

all: bin/rusted-c

bin/rusted-c: obj/ast/AST.o obj/lexer/Lexer.o obj/parser/Parser.o obj/runtime/environment/Environment.o obj/runtime/interpreter/Interpreter.o obj/runtime/values/Values.o obj/main.o
	$(CXX) $(CXXFLAGS) -o $@ $^

obj/ast/AST.o: ast/AST.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

obj/lexer/Lexer.o: lexer/Lexer.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

obj/parser/Parser.o: parser/Parser.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

obj/runtime/environment/Environment.o: runtime/environment/Environment.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

obj/runtime/interpreter/Interpreter.o: runtime/interpreter/Interpreter.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

obj/runtime/values/Values.o: runtime/values/Values.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

obj/main.o: main.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf obj bin/*

.PHONY: all clean
