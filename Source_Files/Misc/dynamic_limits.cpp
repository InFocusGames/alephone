/*
Feb 10, 2000 (Loren Petrich):
	Here is all the behind-the-scenes stuff for getting the dynamic limits.

Feb 14, 2000 (Loren Petrich):
	Changed it to read 'DLim' resource 128

Feb 19, 2000 (Loren Petrich):
	Added local and global monster-buffer upper limits (collision checking)
*/

#include "cseries.h"

#include <string.h>
#include "dynamic_limits.h"

// Reasonable defaults;
// the original ones are in []'s
static short dynamic_limits[NUMBER_OF_DYNAMIC_LIMITS] =
{
	1024,	// [384] Objects (every possible kind)
	 512,	// [220] NPC's
	 128,	// [20] Paths for NPC's to follow (determines how many may be active)
	 128,	// [64] Projectiles
	 128,	// [64] Currently-active effects (blood splatters, explosions, etc.)
	1024,	// [72] Number of objects to render
	  64,	// [16] Local collision buffer (target visibility, NPC-NPC collisions, etc.)
	 256	// [64] Global collision buffer (projectiles with other objects)
};


// Boolean-attribute parser: for switching stuff on and off
class XML_DynLimValueParser: public XML_ElementParser
{

	bool IsPresent;
	short *ValuePtr;

public:
	bool Start();
	bool HandleAttribute(const char *Tag, const char *Value);
	bool AttributesDone();
	
	XML_DynLimValueParser(const char *_Name, short *_ValuePtr):
		XML_ElementParser(_Name), ValuePtr(_ValuePtr) {}
};


bool XML_DynLimValueParser::Start()
{
	IsPresent = false;
	return true;
}

bool XML_DynLimValueParser::HandleAttribute(const char *Tag, const char *Value)
{
	if (strcmp(Tag,"value") == 0)
	{
		if (ReadBoundedNumericalValue(Value,"%hd",*ValuePtr,short(0),short(32767)))
		{
			IsPresent = true;
			return true;
		}
		else return false;
	}
	UnrecognizedTag();
	return false;
}

bool XML_DynLimValueParser::AttributesDone()
{
	// Verify...
	if (!IsPresent)
	{
		AttribsMissing();
		return false;
	}
	return true;
}

static XML_DynLimValueParser
	DynLimParser0("objects",dynamic_limits + _dynamic_limit_objects),
	DynLimParser1("monsters",dynamic_limits + _dynamic_limit_monsters),
	DynLimParser2("paths",dynamic_limits + _dynamic_limit_paths),
	DynLimParser3("projectiles",dynamic_limits + _dynamic_limit_projectiles),
	DynLimParser4("effects",dynamic_limits + _dynamic_limit_effects),
	DynLimParser5("rendered",dynamic_limits + _dynamic_limit_rendered),
	DynLimParser6("local_collision",dynamic_limits + _dynamic_limit_local_collision),
	DynLimParser7("global_collision",dynamic_limits + _dynamic_limit_global_collision);

static XML_ElementParser DynamicLimitsParser("dynamic_limits");

// XML-parser support
XML_ElementParser *DynamicLimits_GetParser()
{
	DynamicLimitsParser.AddChild(&DynLimParser0);
	DynamicLimitsParser.AddChild(&DynLimParser1);
	DynamicLimitsParser.AddChild(&DynLimParser2);
	DynamicLimitsParser.AddChild(&DynLimParser3);
	DynamicLimitsParser.AddChild(&DynLimParser4);
	DynamicLimitsParser.AddChild(&DynLimParser5);
	DynamicLimitsParser.AddChild(&DynLimParser6);
	DynamicLimitsParser.AddChild(&DynLimParser7);
	
	return &DynamicLimitsParser;
}



// Accessor
short get_dynamic_limit(int which) {
	return dynamic_limits[which];
}
