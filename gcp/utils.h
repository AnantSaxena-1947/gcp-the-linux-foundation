#ifndef UTILS_H_
#define UTILS_H_

#include <bits/stdc++.h>
#include <curl/curl.h>

using namespace std;

class GCP {
public:
	string authorize();
	string getPrinters(string);
	vector< pair<string, string> > getPrintersFromJSON(string);
	vector<string> getPrinterOptions(string, string);
	vector<string> getPrinterOptionsFromJSON(string);
	void submitPrintJob(string, string, string, string);
	void parseJSON(string);
	string makeCURLRequest(string);


	string access_token;
	string token_type;
	string expires_in;
	string refresh_token;
	string error;
	string error_description;

private:
	const string auth_url = "https://accounts.google.com/o/oauth2/v2/auth";
	const string api_url = "https://www.googleapis.com/oauth2/v4/token";
	const string client_id = "606895597987-8httq6upblqb4e306p9antbkh7nrmaa0.apps.googleusercontent.com";
	const string client_secret = "csnoeG8HNMgFHt5glZrIvsZW";
	const string scope = "https://www.googleapis.com/auth/cloudprint";
	const string redirect_uri = "urn:ietf:wg:oauth:2.0:oob";
};

size_t writeFunction(void *, size_t, size_t, string *);

#endif /* UTILS_H_ */
