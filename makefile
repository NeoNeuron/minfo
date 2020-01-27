# define compiler and path of libs
CPPFLAGS = --std=c++14 -Wall -I $(DIR_INC)
CXXFLAGS = -g -O2
LDLIBS = -lboost_program_options
# define variable path
DIR_SRC = src
DIR_INC = include
DIR_BIN = bin
DIR_OBJ = obj
DIR_DEP = dep
DIR_TEST= test
DIRS = $(DIR_BIN) $(DIR_DEP) $(DIR_OBJ)
vpath %.cpp $(DIR_SRC)
vpath %.cpp $(DIR_TEST)
vpath %.h 	$(DIR_INC)
vpath %.hpp $(DIR_INC)
vpath %.d 	$(DIR_DEP)
vpath %.o 	$(DIR_OBJ)
vpath %.out $(DIR_BIN)
# include all source in DIR_SRC
SRCS := $(notdir $(wildcard $(DIR_SRC)/*.cpp)) main_lfp_simple.cpp
DEPS = $(SRCS:.cpp=.d)
DEPS := $(addprefix $(DIR_DEP)/, $(DEPS))
#OBJS := $(SRCS:.cpp=.o)
#OBJS := $(addprefix $(DIR_OBJ)/, $(OBJS))
BINS := $(DIR_BIN)/cal-mi $(DIR_BIN)/cal-lfp

.PHONY : all
$all : $(DIRS) $(BINS)
	@mv *.o $(DIR_OBJ)
	@echo '>> ' $(BINS) ' are done'

$(DIR_BIN)/cal-lfp : lfp.o main_lfp.o
	$(CXX) -o $@ $^ $(LDLIBS)

$(DIR_BIN)/cal-mi : mi_uniform.o main.o
	$(CXX) $(CPPFLAGS) -o $@ $^ $(LDLIBS)

$(DIR_BIN)/cal-lfp-simple : $(DIRS) lfp.o main_lfp_simple.o
	$(CXX) $(CPPFLAGS) -o $@ $(filter %.o, $^) $(LDLIBS)
	@mv *.o $(DIR_OBJ)
	@echo '>> ' $(DIR_BIN) '/cal-lfp-simple are done'

#mi.out : mi_uniform.o main_mi.o
#	$(CXX) -o $@ $^

$(DIR_BIN)/cal-sta : sta.o
	$(CXX) $(CPPFLAGS) -o $@ $^ $(LDLIBS)
	@mv *.o $(DIR_OBJ)

$(DIR_BIN)/cal-cc : main_cc.o
	$(CXX) $(CPPFLAGS) -o $@ $^ $(LDLIBS)
	@mv *.o $(DIR_OBJ)

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

#%.o : %.cpp
#	$(CXX) -o $@ -c $(CXXFLAGS) $(CPPFLAGS) $^

.PHONY : clean
clean:
	rm -rf $(DIR_OBJ) $(DIR_BIN) $(DIR_DEP)
