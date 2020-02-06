# define compiler and path of libs
CPPFLAGS = --std=c++11 -Wall -I $(DIR_INC)
CXXFLAGS = -g -O2
LDLIBS = -L $(DIR_LD) -Wl,-rpath,$(DIR_LD) -lboost_program_options -lcnpy
# define variable path
DIR_SRC = src
DIR_INC = include
DIR_BIN = bin
DIR_OBJ = obj
DIR_DEP = dep
DIR_LD  = lib
DIRS = $(DIR_BIN) $(DIR_DEP) $(DIR_OBJ)
vpath %.cpp $(DIR_SRC)
vpath %.h 	$(DIR_INC)
vpath %.hpp $(DIR_INC)
vpath %.d 	$(DIR_DEP)
vpath %.o 	$(DIR_OBJ)
# include all source in DIR_SRC
SRCS := $(notdir $(wildcard $(DIR_SRC)/*.cpp))
DEPS = $(SRCS:.cpp=.d)
DEPS := $(addprefix $(DIR_DEP)/, $(DEPS))
OBJS := $(SRCS:.cpp=.o)
OBJS := $(addprefix $(DIR_OBJ)/, $(OBJS))
BINS := $(DIR_BIN)/cal-mi $(DIR_BIN)/cal-lfp $(DIR_BIN)/cal-lfp-simple $(DIR_BIN)/cal-sta $(DIR_BIN)/cal-cc

.PHONY : all
$all : $(DIRS) $(BINS)
	@echo '>> ' $(BINS) ' are done'

$(DIR_BIN)/cal-lfp : $(DIR_OBJ)/lfp.o $(DIR_OBJ)/main_lfp.o
	$(CXX) -o $@ $^ $(LDLIBS)

$(DIR_BIN)/cal-mi : $(DIR_OBJ)/mi_uniform.o $(DIR_OBJ)/main.o
	$(CXX) -o $@ $^ $(LDLIBS)

$(DIR_BIN)/cal-lfp-simple : $(DIR_OBJ)/lfp.o $(DIR_OBJ)/main_lfp_simple.o
	$(CXX) -o $@ $^ $(LDLIBS)

$(DIR_BIN)/cal-sta : $(DIR_OBJ)/sta.o
	$(CXX) -o $@ $^ $(LDLIBS)

$(DIR_BIN)/cal-cc : $(DIR_OBJ)/main_cc.o
	$(CXX) -o $@ $^ $(LDLIBS)

ifeq ($(wildcard $(DIR_DEP)),)
DEP_DIR_DEP := $(DIR_DEP)
endif

$(DIRS) : 
	mkdir -p $@

# Generate prerequisites
$(DIR_DEP)/%.d : $(DEP_DIR_DEP) %.cpp
	@echo "Making $@ ..."
	@set -e; rm -f $@; \
		$(CC) -MM $(CPPFLAGS) $(filter %.cpp, $^) > $@.$$$$; \
		sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
		rm -f $@.$$$$

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPS)
endif

$(DIR_OBJ)/%.o : %.cpp
	$(CXX) -o $@ -c $(CXXFLAGS) $(CPPFLAGS) $^

.PHONY : clean
clean:
	rm -rf $(DIR_OBJ) $(DIR_BIN) $(DIR_DEP)
