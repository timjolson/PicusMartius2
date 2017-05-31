#include "Toolbox.h"
#include "EEPROMCustom.h"

#define ALLTOOLS_i int i =0; i< list.size(); i++

TOOL blankTool = TOOL( "NONE", "PN", 1, swivelImpact, 0.750, "GHIJK" );

void Toolbox::Print(TOOL tool){
    Serial.print("**\nTool is: ");
    Serial.println(tool.name);
    Serial.print("Part number is: ");
    Serial.println(tool.pn);
    Serial.print("Drawer is: ");
    Serial.println(tool.drawer);
    Serial.print("Type is: ");
    char buff[40];
    switch(tool.type){
        case openEnd:
            strcpy(buff,"Open End Wrench");
        break;
        case wrench:
            strcpy(buff,"Open End Wrench");
        break;
        case ratchetingWrench:
            strcpy(buff,"Ratcheting Wrench");
        break;
        case socketWrench:
            strcpy(buff,"Socket Wrench");
        break;
        case screwDriverPhillips:
            strcpy(buff," + Screw Driver");
        break;
        case screwDriverFlat:
            strcpy(buff," | Screw Driver");
        break;
        case screwDriverStar:
            strcpy(buff," * Screw Driver");
        break;
        case socket:
            strcpy(buff,"Socket");
        break;
        case swivelImpact:
            strcpy(buff,"Swivel Impact Socket");
        break;
        case swivelSocket:
            strcpy(buff,"Swivel Socket");
        break;
        default:
            strcpy(buff,"Unknown Type");
    }
    Serial.println(buff);
    Serial.print("Size is: ");
    Serial.println(tool.size);
    Serial.print("ID is: ");
    Serial.println(tool.ID);
    Serial.println();
}

void Toolbox::Print(){
    if (list.size()==0)
        Serial.println("\n   THERE ARE NO TOOLS!   ");
    else
        for (ALLTOOLS_i)
            Print(list.at(i));
}


void Toolbox::Open(byte dn)
{
    if ((dn >0)&&(dn<nDrawers))
        drawer[dn].GoToMax();
    else
        Serial.println("Drawer out of range");
}
void Toolbox::Close(byte dn)
{
    if ((dn >0)&&(dn<nDrawers))
        drawer[dn].GoToMin();
    else
        Serial.println("Drawer out of range");
}

bool Toolbox::Moving()
{
    for (int i=0;i<nDrawers;i++)
    {
        if (drawer[i].Moving())
            return 1;
    }
    return 0;
}

bool Toolbox::FlagMonitor()
{
    for (int i=0;i<nDrawers;i++)
    {
        if (drawer[i].FlagMonitor())
            return 1;
    }
    return 0;
}

void Toolbox::Init()
{    
    ReadNtools();
    
    // loop through nTools, add to list
    for (int i=0;i<((EEPROM.length()-EEPROMstart())/sizeof(TOOL)), i < nTools;i++)
    {
        TOOL tool;
        EEPROMinit(&tool);
        if ((strcmp(tool.name,blankTool.name)!=0)&&(tool.name[0] != 0))
        {
            list.push_back(tool);
            Serial.println("loaded:: ");
            //Serial.println(list.back().name);
            Print(list.back());
        }
        else i--;
    }
    
}

void Toolbox::Watch()
{
    moving = Moving();
    //TODO: add error watching (like step loss)
    FlagMonitor();
}

void Toolbox::Add(TOOL tool)
{
    for (ALLTOOLS_i)
    {
        
        if (strcmp(list.at(i).ID, tool.ID)==0)
        {
            Serial.println("ID entered previously");
            i = list.size();
        }
        
        else if (i == (list.size()-1))
        {
            _Add(tool);
            Serial.println("last tool searched");
            if (EEPROMwrite(list.back()))
            {
                Serial.print("Added:: ");
                Serial.println(list.back().name);
                //Print(list.back());
                WriteNtools(list.size());
                i = list.size();
            }
            else
            {
                Serial.println("EEPROM write error");
            }
        }
    }
    if (list.size()==0)
    {
        _Add(tool);
        if (EEPROMwrite(list.back()))
        {
            Serial.print("First Tool:: ");
            Serial.println(list.back().name);
            //Print(list.back());
            //Serial.println(list.size());
            WriteNtools(list.size());
        }
        else
        {
            Serial.println("..EEPROM write first tool error");
        }
    }
}

void Toolbox::Remove(char _ID[IDbytes])
{
    for (ALLTOOLS_i)
    {
        if (strcmp(list.at(i).ID, _ID)==0){
            Serial.print("Removed:: ");
            Serial.println(list.at(i).name);
            TOOL holder = list.at(i);
            list.at(i) = blankTool;
            list.at(i).loc = holder.loc;
            EEPROMwrite(list.at(i));
            list.erase(list.begin()+i);
        }
    }
    WriteNtools(list.size());
}

void Toolbox::Clear()
{
    
    for (int i=0; i< EEPROM.length(); i++)
    {
        EEPROM.update(i, 0);
    }
    
    list.clear();
    list.reserve(20);
    EEPROMrestart();
    WriteNtools(0);
}

void Toolbox::ReadNtools()
{
    nTools = EEPROM.read(0);
    nTools = nTools << 8;
    nTools |= EEPROM.read(1);
    char buff[30];
    sprintf(buff, "read %d tools", nTools);
    Serial.println(buff);
}

void Toolbox::WriteNtools(uint16_t _nTools)
{
    int check = _nTools >> 8;
    EEPROM.update(0, check ); //write major byte
    check = (_nTools & 0xFF);
    EEPROM.update(1, check );//write minor byte
    char buff[30];
    sprintf(buff, "wrote %d tool(s)", check);
    Serial.println(buff);
}

void Toolbox::_Add(TOOL tool)
{
    list.push_back(tool);
    //Serial.print("list back:: ");
    //Serial.println(list.back().name);
    EEPROMinit(&(list.back()));
    TOOL holder = list.back();
    list.back() = tool;
    list.back().loc = holder.loc;
}

Toolbox::Toolbox()
{
    list.reserve(20);
}