all: rcbcview

rcbcview: main.c rcbc.o rcbc_render_gl.o rcbc_render_textinfo.o 
	gcc $< -lGL -lGLU -lglut rcbc.o rcbc_render_gl.o rcbc_render_textinfo.o -o $@ -Wall

clean:
	rm -f rcbcview *~ *.o core
