#---------------Final Target----------------#

all:	hss.out mme.out ransim.out

#-------------------------------------------#


#-----------------Compiler------------------#

G++ = g++ -std=c++0x -std=c++11 -std=gnu++0x -ggdb

#-------------------------------------------#


#---------------Object files----------------#

diameter.o:	diameter.cpp diameter.h utils.h
	$(G++) -c -o diameter.o diameter.cpp

gtp.o:	gtp.cpp gtp.h utils.h
	$(G++) -c -o gtp.o gtp.cpp

hss.o:	diameter.h gtp.h hss.cpp hss.h mysql.h network.h packet.h s1ap.h sctp_server.h sync.h utils.h
	$(G++) -c -o hss.o hss.cpp	

hss_server.o:	diameter.h gtp.h hss.h hss_server.cpp hss_server.h mysql.h network.h packet.h s1ap.h sctp_server.h sync.h utils.h
	$(G++) -c -o hss_server.o hss_server.cpp	

mme.o:	diameter.h gtp.h mme.cpp mme.h network.h packet.h s1ap.h security.h sctp_client.h sctp_server.h sync.h telecom.h udp_client.h utils.h
	$(G++) -c -o mme.o mme.cpp

mme_server.o:	diameter.h gtp.h mme.h mme_server.cpp mme_server.h network.h packet.h s1ap.h security.h sctp_client.h sctp_server.h sync.h telecom.h udp_client.h utils.h
	$(G++) -c -o mme_server.o mme_server.cpp	

mysql.o:	mysql.cpp mysql.h utils.h
	$(G++) -c -o mysql.o mysql.cpp

network.o:	network.cpp network.h utils.h 
	$(G++) -c -o network.o network.cpp

packet.o:	diameter.h gtp.h packet.cpp packet.h s1ap.h utils.h
	$(G++) -c -o packet.o packet.cpp

ran.o:	diameter.h gtp.h network.h packet.h ran.cpp ran.h s1ap.h security.h sctp_client.h telecom.h udp_client.h utils.h
	$(G++) -c -o ran.o ran.cpp

ran_simulator.o:	diameter.h gtp.h network.h packet.h ran.h ran_simulator.cpp ran_simulator.h s1ap.h security.h sctp_client.h telecom.h udp_client.h utils.h
	$(G++) -c -o ran_simulator.o ran_simulator.cpp

s1ap.o:	s1ap.cpp s1ap.h utils.h
	$(G++) -c -o s1ap.o s1ap.cpp

security.o:	diameter.h gtp.h packet.h s1ap.h security.cpp security.h utils.h
	$(G++) -c -o security.o security.cpp

sctp_client.o:	diameter.h gtp.h network.h packet.h s1ap.h sctp_client.cpp sctp_client.h utils.h
	$(G++) -c -o sctp_client.o sctp_client.cpp

sctp_server.o:	diameter.h gtp.h network.h packet.h s1ap.h sctp_server.cpp sctp_server.h sync.h utils.h
	$(G++) -c -o sctp_server.o sctp_server.cpp	

sync.o:	sync.cpp sync.h utils.h
	$(G++) -c -o sync.o sync.cpp

telecom.o:	telecom.cpp telecom.h utils.h
	$(G++) -c -o telecom.o telecom.cpp

udp_client.o:	diameter.h gtp.h network.h packet.h s1ap.h udp_client.cpp udp_client.h utils.h
	$(G++) -c -o udp_client.o udp_client.cpp

udp_server.o:	diameter.h gtp.h network.h packet.h s1ap.h udp_server.cpp udp_server.h utils.h
	$(G++) -c -o udp_server.o udp_server.cpp

utils.o:	utils.cpp utils.h
	$(G++) -c -o utils.o utils.cpp

#-------------------------------------------#


#---------Prerequisites and Recipes---------#


HSS_P = diameter.o gtp.o hss.o hss_server.o mysql.o network.o packet.o s1ap.o sctp_server.o sync.o utils.o
HSS_R = $(G++) -o hss.out $(HSS_P) -pthread `mysql_config --cflags --libs`

MME_P = diameter.o gtp.o mme.o mme_server.o network.o packet.o s1ap.o sctp_client.o security.o sctp_server.o sync.o telecom.o udp_client.o utils.o
MME_R = $(G++) -o mme.out $(MME_P) -pthread -lcrypto

RAN_P = diameter.o gtp.o network.o packet.o ran.o ran_simulator.o s1ap.o security.o sctp_client.o telecom.o udp_client.o utils.o
RAN_R = $(G++) -o ransim.out $(RAN_P) -pthread -lcrypto

#-------------------------------------------#


#---------------Executable files------------#

hss.out:	$(HSS_P)
	$(HSS_R)

mme.out:	$(MME_P)
	$(MME_R)

ransim.out:	$(RAN_P)
	$(RAN_R)

#-------------------------------------------#


#------------------Cleaner------------------#

clean:
	rm -f *~ *.o *.out

#-------------------------------------------#


#--------------Special Commands-------------#

# make -k (To keep going on even after encountering errors in making a former target)

#-------------------------------------------#