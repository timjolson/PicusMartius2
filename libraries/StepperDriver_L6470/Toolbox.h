#ifndef TOOLBOX_H
#define TOOLBOX_H
#include <Arduino.h>
#include <vector>
#include "SmartLynx.h"

const byte nDrawers = 10;
const byte IDbits = 96;
const byte IDbytes = IDbits / 8;

enum _types{
    openEnd,
    wrench,
    ratchetingWrench,
    socketWrench,
    screwDriverPhillips,
    screwDriverFlat,
    screwDriverStar,
    socket,
    swivelImpact,
    swivelSocket,
};

struct TOOL
{
    char name[10];
    char pn[8];
    byte drawer;
    uint16_t type;
    float size;
    char ID[IDbytes];
    uint16_t loc;
    TOOL makeTool(char _name[10], char _pn[8], byte _drawer, uint16_t _type, float _size, char _ID[IDbytes]){
        strcpy(name,_name);
        strcpy(pn,_pn);
        drawer = _drawer;
        type = _type;
        size = _size;
        memset(ID,0,IDbytes);
        strcpy(ID,_ID);
        return *this;
    }
    TOOL (char _name[10], char _pn[8], byte _drawer, uint16_t _type, float _size, char _ID[IDbytes]){
        strcpy(name,_name);
        strcpy(pn,_pn);
        drawer = _drawer;
        type = _type;
        size = _size;
        memset(ID,0,IDbytes);
        strcpy(ID,_ID);
    }
    TOOL (){}
};

class Toolbox
{
        uint16_t nTools;
        
        std::vector<TOOL> list;
        /*
        list.at(#); checks index for validity
        list[#]; does not check
        list.push_back(TOOL);   adds tool to list
        list.reserve(#); reserves memory for # TOOLs
        list.capacity(); how much room
        list.size(); how much inside
        */
        
        void ReadNtools();
        void WriteNtools(uint16_t _nTools);
        
        void _Add(TOOL tool);
        
        bool moving;
    
    public:
        Toolbox();
    
        uint16_t GetNtools(){ return list.size(); };
        uint32_t GetSize(){ return list.size() * sizeof(TOOL); };
        
        void Watch();
        
        //TODO add back drivers
        SmartLynx drawer[nDrawers];
        
        void Init();
    
        void Open(byte drawer);
        void Close(byte drawer);
        
        void Add(TOOL tool);
        void Remove(char ID[IDbytes]);
        void Print(TOOL tool);
        void Print();
        
        bool Moving();
        bool FlagMonitor();
        void Clear();
};


#endif