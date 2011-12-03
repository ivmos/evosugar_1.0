#include "sugarsoapBinding.nsmap"
#include <stdio.h>

int main()
{
    struct soap *soap = soap_new();
    char **arr;
    if (soap_call_ns1__test(soap, NULL, NULL, "PatoPato", &arr) == SOAP_OK)
	printf("Veamos %s \n",arr);
    else
	soap_print_fault(soap,stderr);
    return 0;
}

