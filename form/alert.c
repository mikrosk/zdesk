#include "general.h"

/*==================================================================================*
 * show_alert:																		*
 *		Open an AES alert box.														*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		txt		-> Optional text.													*
 * 		error	-> error's identifier.												*
 * 		app		-> Application's descriptor.										*
 *----------------------------------------------------------------------------------*
 * returns: 																		*
 *      --																			*
 *==================================================================================*/

void show_alert( int8 *txt, int32 error, APPVar *app)
{
	char *alert;
	
	switch( error)
  	{
		/* GEMDOS */
		case -EBUSY:
			mt_rsrc_gaddr( R_STRING, E_BUSY, &alert, app->aes_global);
			break;

		case -EUKCMD:
			mt_rsrc_gaddr( R_STRING, E_UKCMD, &alert, app->aes_global);
			break;

		case -ECRC:
			mt_rsrc_gaddr( R_STRING, E_CRC, &alert, app->aes_global);
			break;

		case -EBADR:
			mt_rsrc_gaddr( R_STRING, E_BADR, &alert, app->aes_global);			
			break;

		case -ENOMEM:
			mt_rsrc_gaddr( R_STRING, E_NOMEM, &alert, app->aes_global);				
			break;

			
		/* Custom */

		case EWINDOW:
			mt_rsrc_gaddr( R_STRING, E_WINDOW, &alert, app->aes_global);				
			break;

		case ERSC:
 			alert = "[1][zdesk.rsc not found!][Quit]";
      		break;

		case EEDDI:
 			alert = "[1][zDesk needs a VDI with EDDI][Quit]";
      		break;

		case EMINT:
 			alert = "[1][zDesk works only with MiNT][Quit]";
      		break;

		case ECODEC:
			mt_rsrc_gaddr( R_STRING, E_CODEC, &alert, app->aes_global);				
      		break;

		/* Default */

		default:
			mt_rsrc_gaddr( R_STRING, E_ERROR, &alert, app->aes_global);					
			break;				
	}

	mt_form_alert( 1, alert, app->aes_global);
} 






