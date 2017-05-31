#ifndef ROVEthernet_h
#define ROVEthernet_h

void InitEthernet();    //initialize the ethernet

bool CheckEthernet();   //see if packet is available

const char* ReceiveEthernet();  //read available packet

void SendEthernet(const char*); //send packet

#endif