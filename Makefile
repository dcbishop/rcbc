PREFIX = /usr/local

CC = $(shell which cc gcc colorgcc | tail -n 1)
CFLAGS = $(shell pkg-config --cflags mxml) -g
LIBS = -lmxml -lGL -lGLU -lglut -lIL
OBJS = rcbc.o rcbc_data.o rcbc_render_gl.o rcbc_render_textinfo.o rcbc_xml_minixml.o console.o rcbc_xml_minixml_visualscene.o rcbc_xml_minixml_geometries.o rcbc_xml_minixml_textures.o rcbc_data_hookup.o List.o

#LIBS = -lmxml -lm
#DEBUGFLAGS=-g -D_SKIPGL -D_DEBUG

all: rcbcview librcbc.a loadcheck

.o.c:
	echo "MAKING OBJECT!!!!!!"
	exit
	$(CC) $(CFLAGS) $(DEBUGFLAGS) -c $<

rcbcview: rcbcview.c ${OBJS} 
	${CC} $< ${CFLAGS} ${LIBS} ${OBJS} ${DEBUGFLAGS} -o $@ -Wall

loadcheck: loadcheck.c
	${CC} $< ${CFLAGS} ${LIBS} ${OBJS} ${DEBUGFLAGS} -o $@ -Wall

librcbc.a: rcbc.o ${OBJS}
	ar rcs $@ $< ${OBJS}

rcbc_render_gl.o: rcbc_render_gl.c rcbc_render_gl.h
	${CC} ${CFLAGS} ${DEBUGFLAGS} -c $<

install: librcbc.a rcbcview
	cp librcbc.a ${PREFIX}/lib
	cp rcbcview ${PREFIX}/bin
	cp rcbc.h ${PREFIX}/include
	cp rcbc_data.h ${PREFIX}/include
	cp rcbc_data_hookup.h ${PREFIX}/include
	cp List.h ${PREFIX}/include
	cp ooc.h ${PREFIX}/include

uninstall:
	rm -f ${PREFIX}/lib/librcbc.a
	rm -f ${PREFIX}/bin/rcbcview
	rm -f ${PREFIX}/include/rcbc.h
	rm -f ${PREFIX}/include/rcbc_data.h
	rm -f ${PREFIX}/include/rcbc_data_hookup.h
	rm -f ${PREFIX}/include/List.h
	rm -f ${PREFIX}/include/ooc.h

clean:
	rm -f rcbcview rcbcview.exe loadcheck *~ *.o *.a core *.so *.os .scon*
