CXX = clang++
# CXXFLAGS = -g -DNDEBUG -std=c++11 -Wall -Wextra -Werror -Wpedantic -pedantic-errors
CXXFLAGS = -g -std=c++11 -Wall -Wextra -Werror -Wpedantic -pedantic-errors
OBJS = main.o test.o interpreter.o tokenizer.o token_list.o token_node.o \
	token.o hash_table.o node_pool.o node.o call_stack.o stack_node.o \
	string_stream.o utils.o

.PHONY : all clean distclean

all : run

run : $(OBJS)
	$(CXX) -o run $(OBJS)

main.o : main.cpp interpreter.h tokenizer.h token_list.h token.h token_type.h \
	string_stream.h node_pool.h hash_table.h hash_table.cpp test.h

test.o : test.cpp test.h hash_table.h token.h token_type.h hash_table.cpp \
	node.h node_pool.h string_stream.h tokenizer.h token_list.h

interpreter.o : interpreter.cpp node.h interpreter.h tokenizer.h token_list.h \
	token.h token_type.h string_stream.h node_pool.h hash_table.h \
	hash_table.cpp call_stack.h stack_node.h

tokenizer.o : tokenizer.cpp tokenizer.h token_list.h token.h token_type.h \
	string_stream.h utils.h
token_list.o : token_list.cpp token_node.h token.h token_type.h token_list.h
token_node.o: token_node.cpp token_node.h token.h token_type.h
token.o : token.cpp token.h token_type.h

hash_table.o : hash_table.cpp hash_table.h token.h token_type.h utils.h

node_pool.o : node_pool.cpp node.h node_pool.h utils.h
node.o : node.cpp node.h node_state.h

call_stack.o : call_stack.cpp call_stack.h stack_node.h
stack_node.o : stack_node.cpp stack_node.h

string_stream.o : string_stream.cpp string_stream.h

utils.o : utils.cpp utils.h

clean :
	$(RM) $(OBJS)

distclean : clean
	$(RM) run
