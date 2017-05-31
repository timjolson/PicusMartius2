#ifndef Menu_H
#define Menu_H
#include <Arduino.h>
#include "Screen.h"		//runs lcd
#include "Controller.h" //runs inputs
#include "Radio.h"		//runs receiving/transmitting

#define SIZEOFPROMPT 8		//there are (2) prompts on first line of lcd
#define SIZEOFDISPLAY 16	//there is (1) display on second line of lcd
#define MAXOPTIONS 8		//max number of options on a single page

/*
MenuClass creates a hierarchy/tree of menu 'pages', each containing a list of 'options',
which each have a 'child' page and/or a command number to run

Hierarchy is:
root
	page0 (page1, page2, page3 parent)
		option1 (first option in page0's list)
			page1 (page4's parent, page0's child)
				option1 (first option in page1's list)
					page4 (page1's child)
				option2 (second option in page1's list)
					command (run a command)
		option2 (second option in page0's list)
			page2 (page0's child)
				//set child = acknowledge page
				//display text on specific page without a 
				//target page or taking any actions
		option3 (third option in page0's list)
			page3 (page0's child)
				command (run a command)
			
root should be acknowledge page so it does nothing.
acknowledge page will display feedback upon option selection and run the option's command.
accept page will display feedback upon option selection, run the option's command, AND RETURN TO THE PARENT PAGE.

command number is set when AddOption is used. corresponds to case number in DoCommand().
*/

typedef struct pages pages;	//allows us to make 'pages' inside 'options' before struct pages is fully defined

struct options{		//option to display on a page
	char display[SIZEOFDISPLAY];	//text to display for the option
	int cmd;		//command that is run upon selection (if page is last in branch)
	pages* child;	//page to go to if option is selected
	
	//constructor, starts as blank row
	options(){strcpy(display,"                ");};
};

struct _oplists{		//list of options to display on page
	int numops;			//number of options in list (updates as added)
	options op[MAXOPTIONS];	//array of options structs to cycle through
	
	//Add on option to the list
	//pass "title of option" and &page_to_go_to
	void AddOption(const char* disp, pages* _child){
		strcpy(op[numops].display,disp);	//title of option
		op[numops].cmd=0;			//command to run is irrelevant
		op[numops].child=_child;	//page to go to upon selection
		numops++;					//update number of options in list
	};
	//Add on option to the list
	//pass "title of option", command number, and &page_to_go_to
	void AddOption(const char* disp, int int1, pages* _child){
		strcpy(op[numops].display,disp);	//title of option
		op[numops].cmd=int1;		//command to run is specified
		op[numops].child=_child;	//page to go to upon selection
		numops++;					//update number of options in list
	};
	
	//constructor, page starts with 0 options
	_oplists(){numops=0;};
};

/*
a menu page containing a list of options, having a parent, 
and possibly having a special use (ack or accept)
*/
struct pages{
	char prompt1[SIZEOFPROMPT];	//text to display in first row
	char prompt2[SIZEOFPROMPT];	//text to display mid-way through first row
	_oplists _list;	//list of options to select from
	pages* parent;	//page this one came from
	int numops;		//number of options in page
	byte specialPage;//page is blank and has special function
					//(0) is normal, (1) is acknowledge, (2) is accept
	
	void SetAsAck(){specialPage =1;};	//sets page as acknowledge
	void SetAsAcc(){specialPage =2;};	//sets page as accept
	void Setup(const char* _prompt1, const char* _prompt2){
		//create basics of page, pass in the text to display on first line of screen
		strcpy(prompt1,_prompt1);
		strcpy(prompt2,_prompt2);
		numops=0;	//no options on page yet
	};
	void Setup(const char* _prompt1, const char* _prompt2, pages* _parent){
		//create basics of page, pass in the text to display on first line of screen,
		//and &page this one came from (parent page)
		strncpy(prompt1,_prompt1,8);
		strncpy(prompt2,_prompt2,8);
		parent=_parent;	//parent page passed in
		numops=0;	//no options on page yet
	};
	void AddOption(const char* disp, pages* _child){
		//add option to list in page
		strcpy(_list.op[numops].display,disp);	//title of option
		_list.op[numops].cmd = 0;				//command to run is irrelevant
		_list.op[numops].child = _child;		//page to go to
		_list.op[numops].child->parent = this;	//sets this page as parent of the new child
		numops++;	//update number of options on page
	};
	void AddOption(const char* disp, int _cmd, pages* _child){
		//add option to list in page
		strcpy(_list.op[numops].display,disp);	//title of option
		_list.op[numops].cmd = _cmd;			//command to run
		_list.op[numops].child = _child;		//page to go to
		_list.op[numops].child->parent = this;	//sets this page as parent of the new child
		numops++;	//update number of options on page
	};
	void AddOption(_oplists _op){
		//add a pre-made list of options to the page
		_list=_op;			//pull in list
		numops=_op.numops;	//update number of options on page
		
		//TODO: allow adding list after existing options
		
		for (int i=0; i<numops;i++)//this page is parent of all new children
			_list.op[i].child->parent = this;
	};
	
	//constructor, puts default text in so we can see if a page is not set up
	pages(){
		strcpy(prompt1,"prompt1");
		strcpy(prompt2,"prompt2");
		numops=0;		//no options on page yet
		specialPage=0;	//regular page
		};
};

class MenuClass {
	//general info
	pages* homepage;	//home page
	pages* ackpage;		//acknowledge page
	pages* acceptpage;	//accept page
	pages* currentPage;	//the page currently being browsed
	byte displayOption; //keep track of currently displayed option
	
	//info for temporarily displaying things
	pages* changedPage;	//page modified when displaying things temporarily
	bool displayChanged;//are we temporarily displaying something?
	bool promptChanged; //are we temporarily prompting something?
	char lastDisplay[SIZEOFDISPLAY];	//what to show in second row
	char lastPrompt1[SIZEOFPROMPT];	//first thing to show in first row
	char lastPrompt2[SIZEOFPROMPT];	//second thing to show in first row
	bool gotoParent;	//are we to go to the parent page after temporary display?
	bool cd;
	
	//special stuff
	short countDown;	//when counting down on the display, what is the count?
	char toSend[5];		//text to transmit when running a command
	
	//timing things (millis)
	uint16_t updateInterval;//time between menu updating
	uint16_t displayTime;	//time to temporarily display second row
	uint32_t lastChange;	//time temporary menu text was changed
	uint32_t lastUpdate;	//time menu last tried to update
	uint32_t countDownStart;//time we started a countdown
	
public:
	//temporary screen changes
	void DisplayFor(const char* _text,uint16_t _millis);	//show text on lcd second row for time in millis
	void PromptFor(const char* _text,uint16_t _millis);	//show text on lcd first row for time in millis
	short CountDown();			//runs a countdown on screen
	void CountDown(uint16_t _startseconds);	//STARTS countdown from input seconds
	
	//set acknowledge and accept pages
	void SetHome(struct pages* _home);//set a page to be home page of menu
	void GoHome();		//goes to home page
	void SetAck(struct pages* _ack){ackpage=_ack;};//set page to act as acknowledge for the menu
	void ShowAck();		//show acknowledge feedback
	void SetAccept(struct pages* _acc){acceptpage=_acc;};//set page to act as accept for the menu
	void ShowAccept();	//show accept feedback
	
	//menu navigation
	void GoToPage(struct pages* _gotopage);	//go to a specific page
	void Prev();		//moves to previous option in list
	void Next();		//moves to next option in list
	void Enter();		//selects displayed option
	void Back();		//backs up to previous(parent) page
	
	//screen changes
	bool Update();		//watches for updates to the menu
	void PrintPage();	//prints page text and first option text
	void PrintOption();	//prints current option text
	
	//Get screen text
	char* GetPrompt1(){return currentPage->prompt1;};//returns text for first row
	char* GetPrompt2(){return currentPage->prompt2;};//returns text for first row
	char* GetDisplay(){return currentPage->_list.op[displayOption].display;};//returns text for second row
	
	//where custom command execution goes (called upon selection of an option with ack or accept as child page)
	void DoCommand(int _command);//performs commands specified by an menu page option
	
	//setting up the menu
	void CreatePresetLists();	//creates pre-made lists common to multiple pages
	void Init();				//sets up pages, lists, options, etc.  also starts lcd screen
	
	//constructor, sets updateInterval to 20 millis, starts things at 0
	MenuClass(){displayChanged = promptChanged = 0; lastUpdate = lastChange = 0;
		updateInterval = 20;countDown = -1;gotoParent=0;cd=0;};
};


#endif