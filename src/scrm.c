/*****************************************************************************\
  |* Copyright (C) 2009 Irontec SL (*)                                         *|
  |* Part of the Evosugar project                                              *|
  |* Licensed under the GNU General Public License (*)                         *|
  |*                                                                           *|
  |* (*) Complete information is provided at license.txt                       *|
  |*                                                                           *|
  \*****************************************************************************/

/**
 *
 *  Wrapping over gsoap generated stuff.
 *
 *
 **/

/**
 *
 * TODO All this functions should have a soap object parameter instead
 * of its own stack object. Current design is buggy.
 * Then, soap_end and soap_free should be done from evosugar.c
 */

#include "scrm.h"

#ifndef	FALSE
#define	FALSE	(0)
#endif
#define G_LOG_DOMAIN    "com.irontec.scrm"
#define G_LOG_LEVEL_USER_SHIFT    (8)
#define DEBUG_SCRM_ON        FALSE   //try to debug first in evosugar.c please
#if DEBUG_SCRM_ON == TRUE
#define BREAKPOINT      printf("***** breakpoint %s: %d\n", __FILE__, __LINE__)
#else
#define BREAKPOINT
#endif

SOAP_FMAC1 struct soap *SOAP_FMAC2 __wrap_soap_new(void) {
   return soap_new2(SOAP_C_UTFSTRING, SOAP_C_UTFSTRING);
}

SOAP_FMAC1 void SOAP_FMAC2 __wrap_soap_init(struct soap* soap) {
   return soap_init2(soap, SOAP_C_UTFSTRING, SOAP_C_UTFSTRING);
}

void scrm_get_default_modules(struct select_USCOREfields *arr) {
   arr->__size = 5;
   arr->__ptr = malloc((arr->__size + 1) * sizeof (*arr->__ptr));
   arr->__ptr[0] = (char *) malloc((8+1) * sizeof (char));
   strcpy(arr->__ptr[0], "Accounts");
   arr->__ptr[1] = (char *) malloc((8+1) * sizeof (char));
   strcpy(arr->__ptr[1], "Contacts");
   arr->__ptr[2] = (char *) malloc((13+1) * sizeof (char));
   strcpy(arr->__ptr[2], "Opportunities");
   arr->__ptr[3] = (char *) malloc((7+1) * sizeof (char));
   strcpy(arr->__ptr[3], "Project");
   arr->__ptr[4] = (char *) malloc((5+1) * sizeof (char));
   strcpy(arr->__ptr[4], "Leads");
   arr->__ptr[arr->__size] = NULL;
}

struct select_USCOREfields *scrm_type_casting_field_to_select(const struct field_USCORElist *p_list_field, struct select_USCOREfields *p_list_select) {

   p_list_select->__size = p_list_field->__size;

   BREAKPOINT;
   p_list_select->__ptr = malloc((p_list_select->__size + 1) * sizeof (*p_list_select->__ptr));
   int i;
   for (i = 0; i < p_list_select->__size; i++) {
      BREAKPOINT;

      p_list_select->__ptr[i] = //(char *) malloc( (2+1) * sizeof(char)); strcpy(new_list->__ptr[i], p_list->__ptr[i]->name);
      p_list_field->__ptr[i]->name;
   }
   p_list_select->__ptr[p_list_select->__size] = NULL;
   BREAKPOINT;
   return p_list_select;

}




//SOAP_FMAC5 int SOAP_FMAC6 soap_call_ns1__relate_USCOREnote_USCOREto_USCOREmodule(struct soap *soap, const char *soap_endpoint, const char *soap_action, char *session, char *note_USCOREid, char *module_USCOREname, char *module_USCOREid, struct ns1__relate_USCOREnote_USCOREto_USCOREmoduleResponse *_param_9);

void relate_note_to_module(const char *p_endpoint, const char *p_session, const char *p_note_id, char *p_module_name, const char *p_module_id) {
   struct soap *soap = soap_new();
   struct ns1__relate_USCOREnote_USCOREto_USCOREmoduleResponse response;


   BREAKPOINT;
   soap_ssl_init();
   //if (soap_ssl_client_context(soap,(SOAP_SSL_SKIP_HOST_CHECK|SOAP_SSL_NO_AUTHENTICATION),NULL,NULL,NULL,
   //SOAP_SSL_ALLOW_EXPIRED_CERTIFICATE
   if (soap_ssl_client_context(soap,(SOAP_SSLv3_TLSv1|SOAP_SSL_SKIP_HOST_CHECK|SOAP_SSL_NO_AUTHENTICATION),NULL,NULL,NULL,
            NULL,NULL)) {
      soap_print_fault(&soap, stderr);
      return NULL;
   }



   if (soap_call_ns1__relate_USCOREnote_USCOREto_USCOREmodule(soap, p_endpoint, NULL, p_session, p_note_id, p_module_name, p_module_id, &response) == SOAP_OK) {
      BREAKPOINT;

   } else {
      BREAKPOINT;
      soap_print_fault(soap, stderr);
      soap_print_fault_location(soap, stderr);
   }

}



//SOAP_FMAC5 int SOAP_FMAC6 soap_call_ns1__set_USCORErelationship(struct soap *soap, const char *soap_endpoint, const char *soap_action, char *session, struct ns1__set_USCORErelationship_USCOREvalue *set_USCORErelationship_USCOREvalue, struct ns1__set_USCORErelationshipResponse *_param_16);
//SOAP_FMAC5 int SOAP_FMAC6 soap_call_ns1__set_USCORErelationships(struct soap *soap, const char *soap_endpoint, const char *soap_action, char *session, struct set_USCORErelationship_USCORElist *set_USCORErelationship_USCORElist, struct ns1__set_USCORErelationshipsResponse *_param_17);

void scrm_rel_email(const char *p_endpoint, const char *p_session, const char *p_email_id, const char *p_modulename, const char *p_element_id) {
   struct soap *soap = soap_new();


   BREAKPOINT;
   soap_ssl_init();
   //if (soap_ssl_client_context(soap,(SOAP_SSL_SKIP_HOST_CHECK|SOAP_SSL_NO_AUTHENTICATION),NULL,NULL,NULL,
   //SOAP_SSL_ALLOW_EXPIRED_CERTIFICATE
   if (soap_ssl_client_context(soap,(SOAP_SSLv3_TLSv1|SOAP_SSL_SKIP_HOST_CHECK|SOAP_SSL_NO_AUTHENTICATION),NULL,NULL,NULL,
            NULL,NULL)) {
      soap_print_fault(&soap, stderr);
      return NULL;
   }




   //soap_call_ns1__set_USCORErelationship(struct soap *soap, const char *soap_endpoint, const char *soap_action, char *session, struct ns1__set_USCORErelationship_USCOREvalue *set_USCORErelationship_USCOREvalue, struct ns1__set_USCORErelationshipResponse *_param_16);
   struct ns1__set_USCORErelationshipResponse response;
   BREAKPOINT;
   struct ns1__set_USCORErelationship_USCOREvalue rel;

   rel.module1 = p_modulename;
   rel.module1_USCOREid = p_element_id;
   rel.module2 = "Emails";
   rel.module2_USCOREid = p_email_id;

   if (soap_call_ns1__set_USCORErelationship(soap, p_endpoint, NULL, p_session, &rel, &response) == SOAP_OK) {
      BREAKPOINT;

   } else {
      BREAKPOINT;
      soap_print_fault(soap, stderr);
      soap_print_fault_location(soap, stderr);
   }

}




//SOAP_FMAC5 int SOAP_FMAC6 soap_call_ns1__get_USCOREcontact_USCORErelationships(struct soap *soap, const char *soap_endpoint, const char *soap_action, char *user_USCOREname, char *password, char *id, struct ns1__get_USCOREcontact_USCORErelationshipsResponse *_param_30);

struct contact_USCOREdetail_USCOREarray *scrm_get_contact_relationships(const char *p_endpoint, char *p_username, char *p_password, char *p_contact_id) {
   BREAKPOINT;
   struct ns1__get_USCOREcontact_USCORErelationshipsResponse response;
   struct soap *soap = soap_new();

   BREAKPOINT;
   soap_ssl_init();
   //if (soap_ssl_client_context(soap,(SOAP_SSL_SKIP_HOST_CHECK|SOAP_SSL_NO_AUTHENTICATION),NULL,NULL,NULL,
   //SOAP_SSL_ALLOW_EXPIRED_CERTIFICATE
   if (soap_ssl_client_context(soap,(SOAP_SSLv3_TLSv1|SOAP_SSL_SKIP_HOST_CHECK|SOAP_SSL_NO_AUTHENTICATION),NULL,NULL,NULL,
            NULL,NULL)) {
      soap_print_fault(&soap, stderr);
      return NULL;
   }



   BREAKPOINT;

   if (soap_call_ns1__get_USCOREcontact_USCORErelationships(soap, p_endpoint, NULL, p_username, p_password, p_contact_id, &response) == SOAP_OK) {
      BREAKPOINT;
      return response.return_;
   } else {
      BREAKPOINT;
      soap_print_fault(soap, stderr);
      soap_print_fault_location(soap, stderr);
      return NULL;
   }


}

//SOAP_FMAC5 int SOAP_FMAC6 soap_call_ns1__get_USCOREmodule_USCOREfields(struct soap *soap, const char *soap_endpoint, const char *soap_action, char *session, char *module_USCOREname, struct ns1__get_USCOREmodule_USCOREfieldsResponse *_param_12);

struct field_USCORElist *scrm_get_module_fields(const char *p_endpoint, const char *p_session, char *p_module_name) {
   struct soap *soap = soap_new();
   struct ns1__get_USCOREmodule_USCOREfieldsResponse response;

   BREAKPOINT;
   soap_ssl_init();
   //if (soap_ssl_client_context(soap,(SOAP_SSL_SKIP_HOST_CHECK|SOAP_SSL_NO_AUTHENTICATION),NULL,NULL,NULL,
   //SOAP_SSL_ALLOW_EXPIRED_CERTIFICATE
   if (soap_ssl_client_context(soap,(SOAP_SSLv3_TLSv1|SOAP_SSL_SKIP_HOST_CHECK|SOAP_SSL_NO_AUTHENTICATION),NULL,NULL,NULL,
            NULL,NULL)) {
      soap_print_fault(&soap, stderr);
      return NULL;
   }




   if (soap_call_ns1__get_USCOREmodule_USCOREfields(soap, p_endpoint, NULL, p_session, p_module_name, &response) == SOAP_OK) {
      BREAKPOINT;
      return response.return_->module_USCOREfields;
   } else {
      BREAKPOINT;
      soap_print_fault(soap, stderr);
      soap_print_fault_location(soap, stderr);
      return NULL;
   }
}

int mustmatch(struct soap *soap, const char *tag) {
   if (soap_match_tag(soap, tag, "ns:login*")) // all tags in namespace "ns" that start with "login" are optional
      return SOAP_OK;
   return SOAP_TAG_MISMATCH; // every other tag must be understood (handled)
}



//SOAP_FMAC5 int SOAP_FMAC6 soap_call_ns1__set_USCOREnote_USCOREattachment(struct soap *soap, const char *soap_endpoint, const char *soap_action, char *session, struct ns1__note_USCOREattachment *note, struct ns1__set_USCOREnote_USCOREattachmentResponse *_param_7);

char *set_note_attachment(const char *p_endpoint, const char *p_session, const struct ns1__note_USCOREattachment *p_note) {
   struct soap *soap = soap_new();
   soap->fignore = mustmatch;


   BREAKPOINT;
   soap_ssl_init();
   //if (soap_ssl_client_context(soap,(SOAP_SSL_SKIP_HOST_CHECK|SOAP_SSL_NO_AUTHENTICATION),NULL,NULL,NULL,
   //SOAP_SSL_ALLOW_EXPIRED_CERTIFICATE
   if (soap_ssl_client_context(soap,(SOAP_SSLv3_TLSv1|SOAP_SSL_SKIP_HOST_CHECK|SOAP_SSL_NO_AUTHENTICATION),NULL,NULL,NULL,
            NULL,NULL)) {
      soap_print_fault(&soap, stderr);
      return NULL;
   }





   struct ns1__set_USCOREnote_USCOREattachmentResponse response;
   if (soap_call_ns1__set_USCOREnote_USCOREattachment(soap, p_endpoint, NULL, p_session, p_note, &response) == SOAP_OK) {
      BREAKPOINT;
      return response.return_->id;
   } else {
      BREAKPOINT;
      soap_print_fault(soap, stderr);
      return NULL;
   }
}



//SOAP_FMAC5 int SOAP_FMAC6 soap_call_ns1__set_USCOREentry(struct soap *soap, const char *soap_endpoint, const char *soap_action, char *session, char *module_USCOREname, struct name_USCOREvalue_USCORElist *name_USCOREvalue_USCORElist, struct ns1__set_USCOREentryResponse *_param_5);

char *scrm_set_entry(const char *p_endpoint, const char *p_session, const struct name_USCOREvalue_USCORElist *p_value_list, const char *p_module_name) {
   struct soap *soap = soap_new();
   soap->fignore = mustmatch;


   BREAKPOINT;
   soap_ssl_init();
   //if (soap_ssl_client_context(soap,(SOAP_SSL_SKIP_HOST_CHECK|SOAP_SSL_NO_AUTHENTICATION),NULL,NULL,NULL,
   //SOAP_SSL_ALLOW_EXPIRED_CERTIFICATE
   if (soap_ssl_client_context(soap,(SOAP_SSLv3_TLSv1|SOAP_SSL_SKIP_HOST_CHECK|SOAP_SSL_NO_AUTHENTICATION),NULL,NULL,NULL,
            NULL,NULL)) {
      soap_print_fault(&soap, stderr);
      return NULL;
   }




   struct ns1__set_USCOREentryResponse response;
   if (soap_call_ns1__set_USCOREentry(soap, p_endpoint, NULL, p_session, p_module_name, p_value_list, &response) == SOAP_OK) {
      BREAKPOINT;
      //g_print("Error : %s\n", response.return_->error->description);
      BREAKPOINT;
      return response.return_->id;
   } else {
      BREAKPOINT;

      soap_print_fault(soap, stderr);
      soap_print_fault_location(soap, stderr);
      return NULL;
   }


}



//SOAP_FMAC5 int SOAP_FMAC6 soap_call_ns1__get_USCOREentry_USCORElist(struct soap *soap, const char *soap_endpoint, const char *soap_action, char *session, char *module_USCOREname, char *query, char *order_USCOREby, int offset, struct select_USCOREfields *select_USCOREfields, int max_USCOREresults, int deleted, struct ns1__get_USCOREentry_USCORElistResponse *_param_2);

struct ns1__get_USCOREentry_USCORElist_USCOREresult *scrm_get_entry_list(const char *p_endpoint, const char *p_session, const char *p_module_name, const char *p_query, const char *p_order_by, const struct select_USCOREfields *p_select_fields) {
   struct soap *soap = soap_new();
   //soap_set_imode(soap, SOAP_XML_STRICT);
   soap->fignore = mustmatch;
   struct ns1__get_USCOREentry_USCORElistResponse response;


   BREAKPOINT;
   soap_ssl_init();
   //if (soap_ssl_client_context(soap,(SOAP_SSL_SKIP_HOST_CHECK|SOAP_SSL_NO_AUTHENTICATION),NULL,NULL,NULL,
   //SOAP_SSL_ALLOW_EXPIRED_CERTIFICATE
   if (soap_ssl_client_context(soap,(SOAP_SSLv3_TLSv1|SOAP_SSL_SKIP_HOST_CHECK|SOAP_SSL_NO_AUTHENTICATION),NULL,NULL,NULL,
            NULL,NULL)) {
      soap_print_fault(&soap, stderr);
      return NULL;
   }




   //if (soap_call_ns1__get_USCOREentry_USCORElist(soap,p_endpoint, NULL, p_session, p_module_name, p_query, p_order_by, 0, p_select_fields, 20, FALSE, &response) == SOAP_OK) {
   if (soap_call_ns1__get_USCOREentry_USCORElist(soap, p_endpoint, NULL, p_session, p_module_name, p_query, p_order_by, 0, p_select_fields, 20, FALSE, &response) == SOAP_OK) {
      BREAKPOINT;

      return response.return_;
   } else {
      BREAKPOINT;
      soap_print_fault(soap, stderr);
      soap_print_fault_location(soap, stderr);

      return NULL;
   }

}



//SOAP_FMAC5 int SOAP_FMAC6 soap_call_ns1__search_USCOREby_USCOREmodule(struct soap *soap, const char *soap_endpoint, const char *soap_action, char *user_USCOREname, char *password, char *search_USCOREstring, struct select_USCOREfields *modules, int offset, int max_USCOREresults, struct ns1__search_USCOREby_USCOREmoduleResponse *_param_19);

struct ns1__get_USCOREentry_USCORElist_USCOREresult *scrm_search_by_module(const char *p_endpoint, const char *p_username, const char *p_password, const char *p_search, const struct select_USCOREfields *p_modules) {
   struct soap *soap = soap_new();
   //soap_new2(soap,SOAP_C_UTFSTRING);
   BREAKPOINT;


   BREAKPOINT;
   soap_ssl_init();
   //if (soap_ssl_client_context(soap,(SOAP_SSL_SKIP_HOST_CHECK|SOAP_SSL_NO_AUTHENTICATION),NULL,NULL,NULL,
   //SOAP_SSL_ALLOW_EXPIRED_CERTIFICATE
   if (soap_ssl_client_context(soap,(SOAP_SSLv3_TLSv1|SOAP_SSL_SKIP_HOST_CHECK|SOAP_SSL_NO_AUTHENTICATION),NULL,NULL,NULL,
            NULL,NULL)) {
      soap_print_fault(&soap, stderr);
      return NULL;
   }




   struct ns1__search_USCOREby_USCOREmoduleResponse response;

   if (soap_call_ns1__search_USCOREby_USCOREmodule(soap, p_endpoint, NULL, p_username, p_password, p_search, p_modules, 0, 10000, &response) == SOAP_OK) { // TODO deshadcodear
      BREAKPOINT;

      return response.return_;
   } else {
      BREAKPOINT;
      soap_print_fault(soap, stderr);
      soap_print_fault_location(soap, stderr);
      return NULL;
   }



}




char *scrm_login(char *p_username, char *p_md5_password, char *p_endpoint) {

   struct ns1__user_USCOREauth auth;


   BREAKPOINT;
   struct soap *soap = soap_new();
   BREAKPOINT;
   struct ns1__loginResponse response;
   BREAKPOINT;

   auth.user_USCOREname = p_username;
   BREAKPOINT;
   auth.password = p_md5_password;
   BREAKPOINT;
   auth.version = "0.1"; // TODO deshardcodear
   BREAKPOINT;
   soap_ssl_init();
   //if (soap_ssl_client_context(soap,(SOAP_SSL_SKIP_HOST_CHECK|SOAP_SSL_NO_AUTHENTICATION),NULL,NULL,NULL,
   //SOAP_SSL_ALLOW_EXPIRED_CERTIFICATE
   if (soap_ssl_client_context(soap,(SOAP_SSLv3_TLSv1|SOAP_SSL_SKIP_HOST_CHECK|SOAP_SSL_NO_AUTHENTICATION),NULL,NULL,NULL,
            NULL,NULL)) {
      soap_print_fault(&soap, stderr);
      return NULL;
   }

   BREAKPOINT;
   if (soap_call_ns1__login(soap, p_endpoint, NULL, &auth, NULL, &response) == SOAP_OK) { //valgrind
      BREAKPOINT;
      return response.return_->id;
   } else {

      soap_print_fault(soap, stderr);
      soap_print_fault_location(soap, stderr);
      return NULL;
   }


}


//SOAP_FMAC5 int SOAP_FMAC6 soap_call_ns1__track_USCOREemail(struct soap *soap, const char *soap_endpoint, const char *soap_action, char *user_USCOREname, char *password, char *parent_USCOREid, char *contact_USCOREids, char *date_USCOREsent, char *email_USCOREsubject, char *email_USCOREbody, char **return_);
char **scrm_track_email(const char *p_endpoint, char *p_username, char *p_password, char *p_parent_id, char *p_contact_ids, char *p_date_sent, char *p_email_subject, char *p_emailbody) {
   struct soap *soap = soap_new();
   BREAKPOINT;

   char **response;

   BREAKPOINT;
   soap_ssl_init();
   //if (soap_ssl_client_context(soap,(SOAP_SSL_SKIP_HOST_CHECK|SOAP_SSL_NO_AUTHENTICATION),NULL,NULL,NULL,
   //SOAP_SSL_ALLOW_EXPIRED_CERTIFICATE
   if (soap_ssl_client_context(soap,(SOAP_SSLv3_TLSv1|SOAP_SSL_SKIP_HOST_CHECK|SOAP_SSL_NO_AUTHENTICATION),NULL,NULL,NULL,
            NULL,NULL)) {
      soap_print_fault(&soap, stderr);
      return NULL;
   }




   if (soap_call_ns1__track_USCOREemail(soap, p_endpoint, NULL, p_username,
            p_password, p_parent_id, p_contact_ids, p_date_sent, p_email_subject,
            p_emailbody, response) == SOAP_OK) {

      BREAKPOINT;

      return response;

   } else {

      soap_print_fault(soap, stderr);
      soap_print_fault_location(soap, stderr);
      return NULL;

   }

}
