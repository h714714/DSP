# The following two variable will be commandline determined by TA
# For testing, you could uncomment them.
#SRIPATH = ../../srilm-1.5.10
SRIPATH = /home/r03942039/srilm-1.5.10 all
MACHINE_TYPE = i686-m64
LM = bigram.lm

CXX = g++
CXXFLAGS = -O3 -I$(SRIPATH)/include -w
#CXXFLAGS = -g -I$(SRIPATH)/include -w
vpath lib%.a $(SRIPATH)/lib/$(MACHINE_TYPE)

TARGET1 = mydisambig
TARGET2 = mapping
SRC1 = mydisambig.cpp
SRC2 = mapping.cpp
OBJ1 = $(SRC1:.cpp=.o)
OBJ2 = $(SRC2:.cpp=.o)
TO = ZhuYin-Big5.map
FROM = Big5-ZhuYin.map
.PHONY: all clean map run

all: $(TARGET1) $(TARGET2)

$(TARGET1): $(OBJ1) -loolm -ldstruct -lmisc
	$(CXX) $(LDFLAGS) -o $@ $^

$(TARGET2): $(OBJ2) -loolm -ldstruct -lmisc
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<
run:
	@ #TODO How to run your code toward different txt?
	@for i in $(shell seq 1 10) ; do \
		echo "Running $$i.txt"; \
		./mydisambig -text testdata/$$i.txt -map $(TO) -lm $(LM) -order 2 > result2/$$i.txt; \
	done;
map:
	@ #TODO How to map?
	@echo "Mapping!"
	@$(CXX) -o mapping -O3 -Wall mapping.cpp
	@./mapping $(FROM) $(TO)
	@#matlab mapping.m $(FROM) $(TO)
	@#python mapping.py $(FROM) $(TO)
	@#sh mapping.sh $(FROM) $(TO)
	@#perl mapping.pl Big5-ZhuYin.map ZhuYin-Big5.map
clean:
	$(RM) $(OBJ1) $(OBJ2) $(TARGET1) $(TARGET2) $(TO)
