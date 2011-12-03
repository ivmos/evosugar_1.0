/*****************************************************************************\
|* Copyright (C) 2009 Irontec SL (*)                                         *|
|* Part of the Evosugar project                                              *|
|* Licensed under the GNU General Public License (*)                         *|
|*                                                                           *|
|* (*) Complete information is provided at license.txt                       *|
|*                                                                           *|
\*****************************************************************************/

#include "lib/soapH.h"

void scrm_get_default_modules(struct select_USCOREfields *) ;


struct select_USCOREfields *scrm_type_casting_field_to_select(const struct field_USCORElist *, struct select_USCOREfields *) ;

//SOAP_FMAC5 int SOAP_FMAC6 soap_call_ns1__relate_USCOREnote_USCOREto_USCOREmodule(struct soap *soap, const char *soap_endpoint, const char *soap_action, char *session, char *note_USCOREid, char *module_USCOREname, char *module_USCOREid, struct ns1__relate_USCOREnote_USCOREto_USCOREmoduleResponse *_param_9);
void relate_note_to_module(const char *,const  char *, const  char *, char *, const char *);

void scrm_rel_email(const char *, const char *, const char *, const char *, const char *);

//SOAP_FMAC5 int SOAP_FMAC6 soap_call_ns1__get_USCOREcontact_USCORErelationships(struct soap *soap, const char *soap_endpoint, const char *soap_action, char *user_USCOREname, char *password, char *id, struct ns1__get_USCOREcontact_USCORErelationshipsResponse *_param_30);
struct contact_USCOREdetail_USCOREarray *scrm_get_contact_relationships(const char * ,char *, char *, char *) ;

//SOAP_FMAC5 int SOAP_FMAC6 soap_call_ns1__get_USCOREmodule_USCOREfields(struct soap *soap, const char *soap_endpoint, const char *soap_action, char *session, char *module_USCOREname, struct ns1__get_USCOREmodule_USCOREfieldsResponse *_param_12);
struct field_USCORElist *scrm_get_module_fields(const char *, const char *, char *) ;

int mustmatch(struct soap *soap, const char *tag);


//SOAP_FMAC5 int SOAP_FMAC6 soap_call_ns1__set_USCOREnote_USCOREattachment(struct soap *soap, const char *soap_endpoint, const char *soap_action, char *session, struct ns1__note_USCOREattachment *note, struct ns1__set_USCOREnote_USCOREattachmentResponse *_param_7);
char *set_note_attachment(const char *, const char *, const struct ns1__note_USCOREattachment *) ;

//SOAP_FMAC5 int SOAP_FMAC6 soap_call_ns1__set_USCOREentry(struct soap *soap, const char *soap_endpoint, const char *soap_action, char *session, char *module_USCOREname, struct name_USCOREvalue_USCORElist *name_USCOREvalue_USCORElist, struct ns1__set_USCOREentryResponse *_param_5);
char *scrm_set_entry(const char *, const char *, const struct name_USCOREvalue_USCORElist *, const char *) ;


//SOAP_FMAC5 int SOAP_FMAC6 soap_call_ns1__get_USCOREentry_USCORElist(struct soap *soap, const char *soap_endpoint, const char *soap_action, char *session, char *module_USCOREname, char *query, char *order_USCOREby, int offset, struct select_USCOREfields *select_USCOREfields, int max_USCOREresults, int deleted, struct ns1__get_USCOREentry_USCORElistResponse *_param_2);
struct ns1__get_USCOREentry_USCORElist_USCOREresult *scrm_get_entry_list(const char *, const char *, const char *, const char *, const char *, const struct select_USCOREfields *) ;



//SOAP_FMAC5 int SOAP_FMAC6 soap_call_ns1__search_USCOREby_USCOREmodule(struct soap *soap, const char *soap_endpoint, const char *soap_action, char *user_USCOREname, char *password, char *search_USCOREstring, struct select_USCOREfields *modules, int offset, int max_USCOREresults, struct ns1__search_USCOREby_USCOREmoduleResponse *_param_19);
struct ns1__get_USCOREentry_USCORElist_USCOREresult *scrm_search_by_module(const char *, const char *, const char *, const char *,const struct select_USCOREfields *) ;


//SOAP_FMAC5 int SOAP_FMAC6 soap_call_ns1__get_USCOREavailable_USCOREmodules(struct soap *soap, const char *soap_endpoint, const char *soap_action, char *session, struct ns1__get_USCOREavailable_USCOREmodulesResponse *_param_13);


char *scrm_login(char *, char *, char *) ;


//SOAP_FMAC5 int SOAP_FMAC6 soap_call_ns1__track_USCOREemail(struct soap *soap, const char *soap_endpoint, const char *soap_action, char *user_USCOREname, char *password, char *parent_USCOREid, char *contact_USCOREids, char *date_USCOREsent, char *email_USCOREsubject, char *email_USCOREbody, char **return_);
char **scrm_track_email(const char *, char *, char *, char *, char *, char *, char *, char *);
