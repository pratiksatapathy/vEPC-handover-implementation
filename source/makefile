#---------------Final Target----------------#

all:	utils.o gtp.o s1ap.o diameter.o packet.o network.o sctp_server.o sctp_client.o udp_server.o udp_client.o

#-------------------------------------------#


#-----------------Variables-----------------#

G++ = g++ -std=c++0x -std=c++11 -std=gnu++0x -ggdb

#-------------------------------------------#


#---------Prerequisites and Recipes---------#



#-------------------------------------------#


#---------------Make Statements-------------#

utils.o:	utils.cpp
	$(G++) -c utils.cpp

gtp.o:	gtp.cpp utils.h
	$(G++) -c gtp.cpp

s1ap.o:	s1ap.cpp utils.h
	$(G++) -c s1ap.cpp

diameter.o:	diameter.cpp utils.h
	$(G++) -c diameter.cpp

packet.o:	packet.cpp utils.h gtp.h s1ap.h diameter.h
	$(G++) -c packet.cpp

network.o:	network.cpp utils.h
	$(G++) -c network.cpp

sctp_server.o:	sctp_server.cpp utils.h gtp.h s1ap.h diameter.h packet.h network.h
	$(G++) -c sctp_server.cpp	

sctp_client.o:	sctp_client.cpp utils.h gtp.h s1ap.h diameter.h packet.h network.h
	$(G++) -c sctp_client.cpp

udp_server.o:	udp_server.cpp utils.h gtp.h s1ap.h diameter.h packet.h network.h
	$(G++) -c udp_server.cpp

udp_client.o:	udp_client.cpp utils.h gtp.h s1ap.h diameter.h packet.h network.h
	$(G++) -c udp_client.cpp

clean:
	rm -f *~ *.o *.out

#-------------------------------------------#

#----------Special Commands-----------------#

# make -k (To keep going on even after encountering errors in making a former target)

#-------------------------------------------#
