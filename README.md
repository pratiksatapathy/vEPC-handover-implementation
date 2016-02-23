# Virtualized Evolved Packet Core (v-EPC) 

v-EPC is a simple virtualized form of Long Term Evolution Evolved Packet Core (LTE - EPC). It simulates the working of a typical EPC for User Equipment session creation, termination, handover and paging. 
Being motivated by the fastly growing concept of Network Functions Virtualization (NFV), v-EPC is built to help researchers see the benefits of employing NFV in the EPC. 

## EPC Entities Virtualized:

1. Mobility Management Entity (MME)
2. Home Subscriber Server (HSS)
3. Serving GateWay (S-GW)
4. Packet Data Network GateWay (P-GW)

## LTE Simulator: Used to test v-EPC

1. Radio Access Network (RAN) - User Equipment (UE) + Evolved NodeB (eNodeB)
2. Sink Node - Dummy node to receive / send data traffic

## Requirements

1. Six interconnected VMs running Linux OS on a server
2. Software tools: openssl

## Usage

1. Run EPC-executables each on a separate VM
2. Use LTE simulator **ran** to test EPC
3. Use sink node **sink** to receive data traffic (Dump node)

Note: User Equipment connections, data traffic transfer can be configured using **ran** simulator

## Contact

Sadagopan N S <br />
Email: nss@cse.iitb.ac.in <br />
Webpage: www.cse.iitb.ac.in/~nss <br />
