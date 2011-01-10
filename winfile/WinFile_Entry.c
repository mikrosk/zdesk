#include "general.h"


void add_selected_entry( TAB *tab, Entry *entry)
{
	Entry *ptr = tab->first_selected;
	
	if ( ptr == NULL)
		tab->first_selected = entry;
	else
	{
		while( ptr->next_selected)
			ptr = ptr->next_selected;
			
		ptr->next_selected = entry;
	}
}


void remove_selected_entry( TAB *tab, Entry *entry)
{
	Entry* ptr = tab->first_selected;
	
	if ( ptr == entry) 
		tab->first_selected = ptr->next_selected;
	else 
	{
		while ( ptr->next_selected) 
		{	
			if ( ptr->next_selected == entry)
				break;
			else
				ptr = ptr->next_selected;
		}	
	
		if ( ptr->next_selected == entry)
			ptr->next_selected = ptr->next_selected->next_selected;
	}
	
	entry->next_selected = NULL;
}


inline boolean check_selected_entry( TAB *tab, Entry *entry)
{
	Entry* ptr = tab->first_selected;

	if ( ptr == NULL)
		return FALSE;

	if ( ptr == entry)
		return TRUE;

	while ( ptr->next_selected)
	{
		if ( ptr->next_selected == entry)
			return TRUE;
		else
			ptr = ptr->next_selected;		 
	}
	return FALSE;
}


void remove_all_selected_entry( TAB *tab)
{
	while( tab->first_selected)
		tab->first_selected = tab->first_selected->next_selected;
}


int16 return_entry_nbr( TAB *tab, Entry *entry)
{
	int16 i;
	
	if ( entry == NULL)
		return ( -1);
		
	for ( i = 0 ; i < tab->nbr_icons; i++)	 	 
		if ( entry == &tab->entry[i])
			break;
	
	return i;		
}
