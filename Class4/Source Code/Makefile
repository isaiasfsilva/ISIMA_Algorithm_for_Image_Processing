#
#
#[2017] Isaias Faria Silva
#All Rights Reserved.
# 
#NOTICE: All information contained here in this file is the 
#        property of Isaias Faria Silva. 
#        If you want to use this, please put the contact 
#        rights information.
# 
#PURPOSE:Practical activity of Algorithm for images
#		Clermont-Auvergne University - Computer Science Institute
#		ZZ3 - 2017 december 
#
#DESCRIPTION:
#		This is Makefile file of this project
#
#Last update: 14 january 2018
#*/

#On purge la liste des suffixes utilis� pour les r�les implicites
.SUFFIXES:

#On ajoute simplements les extensions dont l'on a besoin
.SUFFIXES:.cpp .o

#Nom de l'executable
EXEC=tp4

#Liste des fichiers sources separes par des espaces
SOURCES=main.cpp

#Liste des fichiers objets
OBJETS=$(SOURCES:%.cpp=%.o)

#Compilateur et options de compilation
CCPP=g++
CFLAGS=-Wall -ansi -pedantic -ffast-math -I /usr/X11R6/include -I ../../libs/clmg


LFLAGS= -L . -L /usr/X11R6/lib  -lpthread -lX11 -lXext -Dcimg_use_xshm  -lm

#R�le explicite de construction de l'ex�utable
$(EXEC):$(OBJETS) Makefile
	$(CCPP) -o  $(EXEC) $(OBJETS) $(LFLAGS)
.cpp.o:
	$(CCPP) $(CFLAGS) -c $< -o $@

clean:
	rm $(OBJETS)
clear:
	rm $(EXEC)
depend:
	sed -e "/^#DEPENDANCIES/,$$ d" Makefile >dependances
	echo "#DEPENDANCIES" >> dependances
	$(CCPP) -MM $(SOURCES) >> dependances
	cat dependances >Makefile
	rm dependances

#DEPENDANCIES
main.o: main.cpp 




