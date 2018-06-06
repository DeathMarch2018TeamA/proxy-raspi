CC		= g++
CFLAGS		= -O2
BINDIR		= ../bin
TARGET		= $(BINDIR)/proxy
SOURCES		= $(wildcard *.cpp)
INCLUDE		= -I ../include -I ../lib/
OBJDIR		= ../obj
OBJECTS		= $(addprefix $(OBJDIR)/, $(SOURCES:.cpp=.o))

$(TARGET):	$(OBJECTS)
		@if [ ! -d $(BINDIR) ]; then\
			mkdir $(BINDIR);\
		fi
		$(CC) $(CFLAGS) -o $@ $^

$(OBJDIR)/%.o:	%.cpp
		@if [ ! -d $(OBJDIR) ]; then\
			mkdir $(OBJDIR);\
		fi
		@[ -d $(OBJDIR) ]
		$(CC) $(CFLAGS) $(INCLUDE) -o $@ -c $<

clean:
		rm -rf $(OBJDIR)/*.o $(BINDIR)/*.o
