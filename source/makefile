#---------------Final Target----------------#

all:	ran.out mme.out

#all:	utils.o gtp.o s1ap.o diameter.o packet.o network.o sctp_server.o sctp_client.o udp_server.o udp_client.o

#-------------------------------------------#


#-----------------Variables-----------------#

G++ = g++ -std=c++0x -std=c++11 -std=gnu++0x -ggdb

#-------------------------------------------#


#---------Prerequisites and Recipes---------#



#-------------------------------------------#


#---------------Make Statements-------------#

#---------------Object files----------------#

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

mme.o:	mme.cpp utils.h gtp.h s1ap.h diameter.h packet.h network.h sctp_server.h sctp_client.h udp_client.h
	$(G++) -c mme.cpp

mme_server.o:	mme_server.cpp utils.h gtp.h s1ap.h diameter.h packet.h network.h sctp_server.h sctp_client.h udp_client.h mme.h
	$(G++) -c mme_server.cpp	

ran.o:	ran.cpp utils.h gtp.h s1ap.h diameter.h packet.h network.h sctp_client.h udp_client.h
	$(G++) -c ran.cpp

ran_simulator.o:	ran_simulator.cpp	utils.h gtp.h s1ap.h diameter.h packet.h network.h sctp_client.h udp_client.h ran.h
	$(G++) -c ran_simulator.cpp

#-------------------------------------------#

#--------------Executable files-------------#

ran.out:	utils.o gtp.o s1ap.o diameter.o packet.o network.o sctp_client.o udp_client.o ran.o ran_simulator.o
	$(G++) -o ran.out utils.o gtp.o s1ap.o diameter.o packet.o network.o sctp_client.o udp_client.o ran.o ran_simulator.o -pthread

mme.out:	utils.o gtp.o s1ap.o diameter.o packet.o network.o sctp_server.o sctp_client.o udp_client.o mme.o mme_server.o
	$(G++) -o mme.out utils.o gtp.o s1ap.o diameter.o packet.o network.o sctp_server.o sctp_client.o udp_client.o mme.o mme_server.o -pthread

#-------------------------------------------#

clean:
	rm -f *~ *.o *.out

#-------------------------------------------#

#----------Special Commands-----------------#

# make -k (To keep going on even after encountering errors in making a former target)

#-------------------------------------------#
