# Makefile para Linux e macOS
PROG = Bezier

CC = g++
LINUX_FLAGS = -O3 -lGL -lGLU -lglut -lm -lstdc++
DARWIN_FLAGS = -O3 -framework OpenGL -framework Cocoa -framework GLUT -lm -lstdc++

OBJ_DIR = obj
SRC_DIR = src

FONTES = Ponto.cpp Poligono.cpp Temporizador.cpp ListaDeCoresRGB.cpp InstanciaBZ.cpp Bezier.cpp
OBJ = $(addprefix $(OBJ_DIR)/, $(FONTES:.cpp=.o))

UNAME = `uname`

all: check_obj_dir obj_loop main_obj 
	-@make $(UNAME)

check_obj_dir: 
    ifeq ("$(wildcard $(OBJ_DIR))", "")
		@mkdir obj/
    endif

obj_loop: $(OBJ)	
$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CC) -c -o $@ $<

main_obj:
	$(CC) -c -o $(OBJ_DIR)/AnimacaoComBezier.o AnimacaoComBezier.cpp

Darwin:
	$(CC) $(OBJ) $(OBJ_DIR)/AnimacaoComBezier.o $(DARWIN_FLAGS) -o $(PROG)

Linux:
	$(CC) $(OBJ) $(OBJ_DIR)/AnimacaoComBezier.o $(LINUX_FLAGS) -o $(PROG)

clean:
	-@ rm -rf $(OBJ_DIR) $(PROG)