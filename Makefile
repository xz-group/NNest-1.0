
.PHONY: all clean
EXE = main_PR

HAS_READLINE ?= 0

UTILS_OBJS = 
OBJS += $(UTILS_OBJS)
OBJ_FILES = $(patsubst %.o,obj/%.o,$(OBJS))

OBJ_DIR = obj



CACTI_DIR = cacti-p
CACTI_OBJ_DIR = $(CACTI_DIR)/obj_opt

ifndef NTHREADS
  NTHREADS = 8
endif
CACTI_OBJS = Ucache.o bank.o cacti_interface.o decoder.o  parameter.o \
             technology.o arbiter.o  basic_circuit.o  component.o  htree2.o \
						 mat.o router.o uca.o area.o crossbar.o io.o nuca.o subarray.o \
						 wire.o powergating.o
CACTI_SRCS = $(patsubst %.o,%.cc,$(CACTI_OBJS))
CACTI_OBJ_FILES = $(patsubst %.o,$(CACTI_OBJ_DIR)/%.o,$(CACTI_OBJS))



CFLAGS += -g -std=c++0x -Icacti-p
LFLAGS += -lpthread

ifeq ($(HAS_READLINE),1)
  CFLAGS += -DHAS_READLINE
  LFLAGS += -lreadline
endif

all : CFLAGS+=-O3
all : $(EXE)

obj_dir:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(CACTI_OBJ_DIR)

lib : obj_dir $(OBJ_FILES) $(CACTI_OBJ_FILES)
	ar rcs $(OBJ_FILES) $(CACTI_OBJ_FILES)

$(EXE): obj_dir $(CACTI_OBJ_FILES) $(OBJ_FILES)
	$(CXX) $(CFLAGS) -c main_PR.cpp
	$(CXX) -o $(EXE) $(EXE).o $(OBJ_FILES) $(CACTI_OBJ_FILES) $(LFLAGS)

$(OBJ_DIR)/%.o : %.h %.cpp
	$(CXX) $(CFLAGS) -o $@ -c $*.cpp

$(CACTI_OBJ_DIR)/%.o : $(CACTI_DIR)/%.cc
	$(CXX) $(CFLAGS)  -DNTHREADS=$(NTHREADS) -o $@ -c $(CACTI_DIR)/$*.cc

clean:
	rm -f $(OBJ_DIR)/*.o
	rm -f $(CACTI_OBJ_DIR)/*.o
	rm -f main_PR
