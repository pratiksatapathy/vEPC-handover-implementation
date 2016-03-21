#---------------Final Target----------------#

all:	ransim.out mme.out

#-------------------------------------------#


#-----------------Compiler------------------#

G++ = g++ -std=c++0x -std=c++11 -std=gnu++0x -ggdb

#-------------------------------------------#


#---------------Object files----------------#

utils.o:	utils.h
	$(G++) -c -o utils.o utils.cpp

gtp.o:	gtp.h utils.h
	$(G++) -c -o gtp.o gtp.cpp

s1ap.o:	s1ap.h utils.h
	$(G++) -c -o s1ap.o s1ap.cpp

diameter.o:	diameter.h utils.h
	$(G++) -c -o diameter.o diameter.cpp

packet.o:	packet.h utils.h gtp.h s1ap.h diameter.h
	$(G++) -c -o packet.o packet.cpp

network.o:	network.h utils.h
	$(G++) -c -o network.o network.cpp

sctp_server.o:	sctp_server.h utils.h gtp.h s1ap.h diameter.h packet.h network.h
	$(G++) -c -o sctp_server.o sctp_server.cpp	

sctp_client.o:	sctp_client.h utils.h gtp.h s1ap.h diameter.h packet.h network.h
	$(G++) -c -o sctp_client.o sctp_client.cpp

udp_server.o:	udp_server.h utils.h gtp.h s1ap.h diameter.h packet.h network.h
	$(G++) -c -o udp_server.o udp_server.cpp

udp_client.o:	udp_client.h utils.h gtp.h s1ap.h diameter.h packet.h network.h
	$(G++) -c -o udp_client.o udp_client.cpp

telecom.o:	telecom.h utils.h
	$(G++) -c -o telecom.o telecom.cpp

mme.o:	mme.h utils.h gtp.h s1ap.h diameter.h packet.h network.h sctp_server.h sctp_client.h udp_client.h telecom.h
	$(G++) -c -o mme.o mme.cpp

mme_server.o:	mme_server.h utils.h gtp.h s1ap.h diameter.h packet.h network.h sctp_server.h sctp_client.h udp_client.h telecom.h mme.h
	$(G++) -c -o mme_server.o mme_server.cpp	

ran.o:	ran.h utils.h gtp.h s1ap.h diameter.h packet.h network.h sctp_client.h udp_client.h telecom.h
	$(G++) -c -o ran.o ran.cpp

ran_simulator.o:	ran_simulator.h utils.h gtp.h s1ap.h diameter.h packet.h network.h sctp_client.h udp_client.h telecom.h ran.h
	$(G++) -c -o ran_simulator.o ran_simulator.cpp

#-------------------------------------------#


#---------Prerequisites and Recipes---------#

RAN_P = utils.o gtp.o s1ap.o diameter.o packet.o network.o sctp_client.o udp_client.o telecom.o ran.o ran_simulator.o
RAN_R = $(G++) -o ransim.out $(RAN_P) -pthread

MME_P = utils.o gtp.o s1ap.o diameter.o packet.o network.o sctp_server.o sctp_client.o udp_client.o telecom.o mme.o mme_server.o
MME_R = $(G++) -o mme.out $(MME_P) -pthread

#-------------------------------------------#


#---------------Executable files------------#

ransim.out:	$(RAN_P)
	$(RAN_R)

mme.out:	$(MME_P)
	$(MME_R)

#-------------------------------------------#


#------------------Cleaner------------------#

clean:
	rm -f *~ *.o *.out

#-------------------------------------------#


#--------------Special Commands-------------#

# make -k (To keep going on even after encountering errors in making a former target)

#-------------------------------------------#