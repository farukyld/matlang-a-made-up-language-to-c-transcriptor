# https://www.gnu.org/software/make/manual/make.html
# https://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/

# make file is designed for the following file tree:

# ./
# |	headers/ # a folder containing some headers designed by me.
# |	entrypoints/ # a folder containing .c files with int main(int,char**){} definitions.
# |	sources/ # a folder containing .c files that are commonly used by main files.
# |	makefile # this file.




# this makefile has rules 

# to create  the folders 
# ./sources/obj/
# ./entrypoints/obj/

# to compile relocatables
# from ./sources/*.c to ./sources/obj/*.o
# from ./entrypoints/*.c to ./entrypoints/obj/*.o

# and link relocatables in ./sources/obj/*.o with each one of ./entrypoints/obj/*.o 
# into an executable with same name with the associated main file.

 # common source folder
SRC_DIR=./sources/

# header folder
HDR_DIR=./headers/

# common relocatable folder
OBJ_DIR=$(SRC_DIR)obj/

# folder with main files
ENTRY_POINT_SRC_DIR=./entrypoints/

# folder with main files' relocatables
ENTRY_POINT_OBJ_DIR=$(ENTRY_POINT_SRC_DIR)obj/

# to place the executables
BUILT_DIR=./built/

# ./entrypoints/obj/ ./sources/obj/
ADDITIONAL_FOLDERS=$(ENTRY_POINT_OBJ_DIR) $(OBJ_DIR) $(BUILT_DIR)

# paths to c files with int main(int,char**){} definitions.
ENTRY_POINT_SOURCES=$(wildcard $(ENTRY_POINT_SRC_DIR)*.c)

# paths to common c files
SOURCES=$(wildcard $(SRC_DIR)*.c)

# paths to headers
HEADERS=$(wildcard $(HDR_DIR)*.h)

 # paths to common relocatable files
OBJECTS=$(patsubst $(SRC_DIR)%.c, $(OBJ_DIR)%.o, $(SOURCES))

# paths to relocatables with int main(int,char**){} definitions.
ENTRY_POINT_OBJECTS=$(patsubst $(ENTRY_POINT_SRC_DIR)%.c, $(ENTRY_POINT_OBJ_DIR)%.o, $(ENTRY_POINT_SOURCES))

# prerequisites to create executable files.
PREQS_for_EXECUTABLES=folders $(OBJECTS)


IDIR =$(HDR_DIR)
CC=gcc
CFLAGS=$(patsubst %,-I%,$(IDIR))
DEPS=$(HEADERS)

LIBS=-lm


# create necessary folders
.PHONY: folders
folders:
	@(mkdir $(ADDITIONAL_FOLDERS) 1>/dev/null 2>/dev/null && echo creating $(ADDITIONAL_FOLDERS) \
	|| echo folders $(ADDITIONAL_FOLDERS) already created)


# compiles relocatable requested as ./soruces/obj/*.o
$(OBJ_DIR)%.o: $(SRC_DIR)%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# compiles relocatable requested as ./entrypoints/obj/*.o
$(ENTRY_POINT_OBJ_DIR)%.o: $(ENTRY_POINT_SRC_DIR)%.c
	$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)



.PHONY: clean
clean:
	rm -rf $(ADDITIONAL_FOLDERS)

# link one of main files from $(ENTRY_POINT_OBJ_DIR) with ./sources/obj/*.o files 
# into an executable with same name with the main file

# main was my main file names. 
# I added it here because I want for shell to advice it when I type make and hit tab.

main: %: $(PREQS_for_EXECUTABLES) $(ENTRY_POINT_OBJ_DIR)%.o
	$(CC) -o $(BUILT_DIR)$@ $(wordlist 2, $(words $^), $^) $(CFLAGS) $(LIBS)
