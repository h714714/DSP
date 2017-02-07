# The following two variable will be commandline determined by TA
# For testing, you could uncomment them.
SRIPATH ?= /usr/local/srilm-1.5.10
MACHINE_TYPE ?= i686-m64
LM ?= bigram.lm

CXX = g++
CXXFLAGS = -O3 -std=c++11 -I$(SRIPATH)/include -w
vpath lib%.a $(SRIPATH)/lib/$(MACHINE_TYPE)

TARGET = mydisambig
SRC = mydisambig.cpp
OBJ = $(SRC:.cpp=.o)
TO = ZhuYin-Big5.map
FROM = Big5-ZhuYin.map

TESTDATAROOT = testdata
.PHONY: all clean map run test

all: $(TARGET)

$(TARGET): $(OBJ) -loolm -ldstruct -lmisc
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<
run: $(TARGET)
#	#TODO How to run your code toward different txt?
	for i in $(shell seq 1 10) ; do \
		echo "Running $$i.txt"; \
		./mydisambig -text $(TESTDATAROOT)/$$i.txt -map $(TO) -lm $(LM) -order 2 > result2/$$i.txt; \
	done;
#	./mydisambig -text $(TESTDATAROOT)/1.txt -map $(TO) -lm $(LM) -order 2 > result2/1.txt  2>aaaaaa
map:
	@#TODO How to map?
	@echo "Mapping!"
	@#./mapping $(FROM) $(TO)
	@#matlab mapping.m $(FROM) $(TO)
	python mapping.py $(FROM) $(TO)
	@#sh mapping.sh $(FROM) $(TO)
	@#perl mapping.pl Big5-ZhuYin.map ZhuYin-Big5.map

test:
	for i in $(shell seq 1 10) ; do \
		diff result1/$$i.txt result2/$$i.txt; \
	done;
	

clean:
	$(RM) $(OBJ) $(TARGET)

