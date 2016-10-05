SOURCES=deduplicate.cpp
OBJS=$(patsubst %.cpp, %.o, $(SOURCES))
CPPFLAGS=-ggdb3 -Wall -Werror -pedantic -std=gnu++98

deduplicate: $(OBJS)
        g++ $(CPPFLAGS) -o deduplicate $(OBJS)
%.o: %.cpp linkedlist.h file.h hashset.h
        g++ $(CPPFLAGS) -c $<

clean:
        rm deduplicate *~ *.o


