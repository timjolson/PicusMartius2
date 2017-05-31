#include "ROVEthernet.h"
#include "..\Ethernet2\src\Ethernet2.h"
#include "..\Ethernet2\src\EthernetUdp2.h"

#include <Arduino.h>

extern EthernetClass Ethernet;

EthernetUDP Udp; // Create a UDP Object
byte mac[] ={ 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEE}; //Assign mac address
IPAddress ip(192, 168, 1, 100); //Assign the IP Adress
IPAddress remote(192, 168, 1, 2 ); //IP address of Raspberry PI or commanding unit
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //char array to hold our data packet
char buff[UDP_TX_PACKET_MAX_SIZE];  //buffer packets get copied to to send elsewhere
unsigned int port = 5000;   //communicaton port
int packetSize = 0;         //packet size available

void InitEthernet()
{
    Ethernet.begin(mac,ip); //start communication
    Udp.begin(port);        //start UDP connection
};

bool CheckEthernet()
{
    packetSize = Udp.parsePacket(); //get packet size
    return !!packetSize;              //return 1 if anything, 0 otherwise
}

const char* ReceiveEthernet()
{
    if(packetSize>0) { //if packetSize is >0, that means someone has sent a request
    
        Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE); //Read the data request
        //String datReq(packetBuffer); //Convert char array packetBuffer into a string called datReq
    }
    
    strcpy(buff,packetBuffer);  //copy packetBuffer into buff
    memset(packetBuffer,0,sizeof(packetBuffer));    //clear packetBuffer
    return buff;    //return packet
};

void SendEthernet(const char* tx)
{
    Udp.beginPacket(remote, port);  //open connection
    Udp.write(tx);                  //transmit packet
    Udp.endPacket();                //close connection
};