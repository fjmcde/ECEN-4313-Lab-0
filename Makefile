###################
#    Variables    #
###################

# Compiler setup
CC      = g++
CFLAGS  = -Wall -Wextra -g

# Source files
SRC_DIR     = src
SRC_FILES   = $(wildcard $(SRC_DIR)/*.cpp)

# Object files
OBJ_DIR     = obj
OBJ_FILES   = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

# Output executable name
EXECUTABLE = mysort

# Make makefile cross platform for my own sanity
# NOTE: I want to develop and debug test on Windows in VSCode.
#       I'll then compile on Linux Jupyter VM and push/unit test there.
#       Is this more work? Yes... but I hate vim and/or nano and prefer
#       full-blown IDEs over text-editors.
ifeq ($(OS), Windows_NT)
    # Windows configuration
    RM                          = del /Q /F
    RM_EXECUTABLE_EXTENSION     = .exe
    RM_OBJ_FILES                = $(subst /, \, $(OBJ_FILES))
    RMDIR                       = rmdir /S /Q
    MKDIR                       = mkdir
else
    # Unix configuration
    RM                          = rm -f
    RM_EXECUTABLE_EXTENSION     = ""
    RM_OBJ_FILES                = $(OBJ_FILES)
    RMDIR                       = rm -rf
    MKDIR                       = mkdir
endif

#################
#    Targets    #
#################

# Executable target
# Generate .exe
$(EXECUTABLE): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^

# Object directory target
# Having object files everywhere makes me crazy; so
# put them all in the same place
$(OBJ_DIR):
	$(MKDIR) $(OBJ_DIR)

# Object file(s) target
# Generagte object files from source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# Cleanup object files, directories, and executable
clean:
	$(RM) $(RM_OBJ_FILES) $(EXECUTABLE)$(RM_EXECUTABLE_EXTENSION)
	$(RMDIR) $(OBJ_DIR)

# Clean is not a real build target
.PHONY: clean
