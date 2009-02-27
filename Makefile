all: rcbcview

rcbcview: main.c rcbc.o rcbc_render_gl.o rcbc_render_textinfo.o rcbc_xml_tinyxml.o console.o 
	gcc $< -lGL -lGLU -lglut rcbc.o rcbc_render_gl.o rcbc_render_textinfo.o rcbc_xml_tinyxml.o console.o -o $@ -Wall

clean:
	rm -f rcbcview *~ *.o core
