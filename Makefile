
### FLAGS
CC     = g++
CFLAGS = -Wall -g
LIBS   = -lframe -L. -lboost_thread -lboost_system -lutility -pthread

# FILES
PARSER_SRC = parse_env.cpp lex_env.cpp
PARSER_OBJ = parse_env.o
SOURCES = main.cpp homebot.cpp domain.cpp
OBJECTS = $(SOURCES:.cpp=.o)

# Implicit rules
.cpp.o:; $(CC) -c $(CFLAGS) $<

# BUILD RULES
homebot: $(OBJECTS) $(PARSER_OBJ)
	$(CC) -o homebot $(OBJECTS) $(PARSER_OBJ) $(CFLAGS) $(LIBS)

lex_env.cpp: env.lex
	flex -Penv -olex_env.cpp env.lex
parse_env.cpp: env.y lex_env.cpp
	bison -penv -benv -oparse_env.cpp env.y

clean:
	rm -f *.o homebot lex_env.cpp parse_env.cpp
