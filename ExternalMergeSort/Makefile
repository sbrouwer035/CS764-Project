CPPOPT=-g -Og -D_DEBUG
# -O2 -Os -Ofast
# -fprofile-generate -fprofile-use
CPPFLAGS=$(CPPOPT) -Wall -ansi -pedantic #-g
# -Wparentheses -Wno-unused-parameter -Wformat-security
# -fno-rtti -std=c++11 -std=c++98

# documents and scripts
DOCS=Tasks.txt
SCRS=

# headers and code sources
HDRS=	defs.h Constant.h Verification.h NRecord.h NTournamentTreeNode.h NTournamentTree.h NDevice.h
SRCS=	defs.cpp Assert.cpp Test.cpp \
		Verification.cpp NRecord.cpp NTournamentTreeNode.cpp NTournamentTree.cpp NDevice.cpp 

# compilation targets
OBJS=	defs.o Assert.o Test.o \
		Verification.o NRecord.o NTournamentTreeNode.o NTournamentTree.o NDevice.o 

# RCS assists
REV=-q -f
MSG=no message

# default target
#
ExternalSort.exe : Makefile $(OBJS)
	g++ $(CPPFLAGS) -o ExternalSort.exe $(OBJS)

trace : ExternalSort.exe Makefile
	@date > trace
	./ExternalSort.exe >> trace
	@size -t ExternalSort.exe $(OBJS) | sort -r >> trace

$(OBJS) : Makefile defs.h

list : Makefile
	echo Makefile $(HDRS) $(SRCS) $(DOCS) $(SCRS) > list
count : list
	@wc `cat list`

ci :
	ci $(REV) -m"$(MSG)" $(HDRS) $(SRCS) $(DOCS) $(SCRS)
	ci -l $(REV) -m"$(MSG)" Makefile
co :
	co $(REV) -l $(HDRS) $(SRCS) $(DOCS) $(SCRS)

clean :
	@rm -f $(OBJS) ExternalSort.exe ExternalSort.exe.stackdump trace input_table output_table trace0.txt

