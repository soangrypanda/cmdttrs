SRCDIR = src
OBJDIR = obj
HEADDIR = headers

vpath %.c $(SRCDIR)
vpath %.o $(OBJDIR)
vpath %.h $(HEADDIR)

SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(patsubst $(SRCDIR)/%.c, %.o, $(SRC))

HEADERS = -I/home/admin/Projects/C/mltge/headers -Iheaders
LIBS = -L/home/admin/Projects/C/mltge -lcurses -lmltge -lm
TEMP = /home/admin/Projects/C/mltge/libmltge.a 
CFLAGS = -g -Wall 


#add library to dependancies

cmdttrs: $(OBJ)
	$(CC) -o $@ $(addprefix $(OBJDIR)/, $^) $(CFLAGS) $(LIBS) 
$(OBJ): %.o: %.c | $(OBJDIR)
	$(CC) -c $< $(CFLAGS) $(HEADERS) -o $(OBJDIR)/$@

$(OBJDIR):
	mkdir $@ 

.PHONY: clean print

clean:
	-rm cmdttrs
	-rm -r obj

print:
	@echo objects: $(OBJ)
	@echo sources: $(SRC)
