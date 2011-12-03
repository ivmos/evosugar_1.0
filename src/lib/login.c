#include "sugarsoapBinding.nsmap"
#include <stdio.h>

int main()
{

    struct ns1__user_USCOREauth auth;
    struct soap *soap = soap_new();
    struct ns1__loginResponse response;
    char **arr;
    
    auth.user_USCOREname = "admin";  
    auth.password = "1baa";
    auth.version = "0.1";
    //int SOAP_FMAC6 soap_call_ns1__login(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct ns1__user_USCOREauth *user_USCOREauth, char *application_USCOREname, struct ns1__loginResponse *_param_18);
    //if (soap_call_ns1__test(soap, NULL, NULL, "PatoPato", &arr) == SOAP_OK)
	// "https://hq.irontec.com/iCRM/soap.php"
    if (soap_call_ns1__login(soap, "http://mail2crm.irontec.com/soap.php", NULL, &auth, NULL, &response) == SOAP_OK) {
	printf("Veamos %s \n",response.return_->id);
	printf("Hola");
    } else {
	printf("Falla");
	soap_print_fault(soap,stderr);

    }
    return 0;
}

