TARGET_EXEC = ezbfcpp
CXX = g++

MAKE = make --no-print-directory

BUILD_DIR ?= ./build
SRC_DIRS ?= ./src

PARSER = $(SRC_DIRS)/ast

SRCS := $(addsuffix .cpp,$(sort $(basename $(shell find $(SRC_DIRS) -name *.cpp -or -name *.yy -or -name *.ll))))
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP

$(TARGET_EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

%.cpp %.hpp: %.yy
	@$(MAKE) -C $(PARSER)

%.cpp %.hpp: %.ll
	@$(MAKE) -C $(PARSER)

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	@$(MKDIR_P) $(dir $@)
	@$(MAKE) -C $(PARSER)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)
	$(RM) $(TARGET_EXEC)
	@$(MAKE) -C $(PARSER) $(MAKECMDGOALS)

-include $(DEPS)

MKDIR_P ?= mkdir -p