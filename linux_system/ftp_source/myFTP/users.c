#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "users.h"

userList connectedUser = {NULL, NULL};
userList unconnectedUser = {NULL, NULL};

static void removeFrom(userList* list, user* theUser);
static void addTo(userList* list, user* theUser);


static void addTo(userList* list, user* theUser)
{
	theUser -> next = NULL;

	if(list -> last == NULL)
	{
		// list is empty by now
		list -> first= theUser;
		list -> last = theUser;
		theUser -> previous = NULL;
	}	
	else
	{
		list -> last -> next = theUser;
		theUser -> previous = list -> last;
		list -> last = theUser;
	}
}

user* addNewUser(userList* list)
{
	user* newUser = (user *) malloc( sizeof(user));
	if(!newUser)
		return NULL;

	memset(newUser, 0, sizeof(newUser));
	addTo(list, newUser);
	return newUser;
}

static void removeFrom(userList* list, user* theUser)
{
	user* first;
	user* last;
	first = list -> first;
	last = list -> last;

	if(first == last)
	{
		// "theUser" is the only one
		list -> first = NULL;
		list -> last = NULL;
	}
	else if(theUser == first)
	{
		list -> first = theUser -> next;
		theUser -> next -> previous = NULL;
	}
	else if(theUser == last)
	{
		list -> last = theUser -> previous;
		theUser -> previous -> next = NULL;
	}
	else
	{
		theUser -> previous -> next = theUser -> next;
		theUser -> next -> previous = theUser -> previous;
	}
}

void moveUser(userList* from, userList* to, user* theUser)
{
	removeFrom(from, theUser);
	addTo(to, theUser);
}

user* findUserByName(const char* name, userList* list)
{
	user* current;
	for(current = list -> first; current; current = current -> next)
	{
		if( strcmp(current -> username, name) == 0)
			return current;
	}
	return NULL;
}

user* findUserByControlSocketFileDescriptor(int fileDescriptor, userList* list)
{
	user* current;
	for(current = list -> first; current; current = current -> next)
	{
		if( fileDescriptor == current -> controlSocket)
			return current;
	}
	return NULL;
}

void printUserList(userList* list)
{
	user* current;
	for(current = list -> first; current; current = current -> next)
	{
		printf("%s:%s@%s\n", current -> username, current -> password,
							current -> homeDirectory);
	}
}
