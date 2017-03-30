#include "utils.h"

size_t writeFunction(void *buffer, size_t size, size_t nmemb, string *s) {
	/*
	 * This function converts JSON response to a string.
	 */
	size_t newLength = size * nmemb;
	size_t oldLength = s->size();
	try	{
		s->resize(oldLength + newLength);
	} catch (bad_alloc &e) {
		return 0;
	}

	copy((char*)buffer, (char*)buffer + newLength, s->begin() + oldLength);
	return size * nmemb;
}

vector<string> GCP::getPrinterOptionsFromJSON(string s){
	vector<string> printer_details;
	size_t start, end;

	start = s.find("vendor_capability");
	s = s.substr(start);
	start = s.find("id");
	while(start != string::npos){
		start += 6;
		end = s.find("\"", start);
		string option_id = s.substr(start, end-start);
		s = s.substr(end);
		printer_details.push_back(option_id);
		start = s.find("id\"");
	}
	printer_details.pop_back();
	return printer_details;
}

vector<string> GCP::getPrinterOptions(string printer_id, string access_token){

	string url = "https://www.google.com/cloudprint/printer?access_token="+access_token+"&printerid="+printer_id;

	string s = GCP::makeCURLRequest(url);
	return GCP::getPrinterOptionsFromJSON(s);
}

void GCP::submitPrintJob(string printer_id, string title, string ticket, string content){

}

string GCP::makeCURLRequest(string url){
	CURL *c;
	CURLcode opt, res;
	string response;
	string s;


	curl_global_init(CURL_GLOBAL_ALL);
	c = curl_easy_init();

	if (c) {
		string postfields;
		string code;
		struct curl_slist *chunk = NULL;

		// Header.
//		string authorization = "Authorization: Bearer auth="+access_token;
		string app_name = "X-CloudPrint-Proxy: Google Cloud Print GSoC";

		// Append header.
//		chunk = curl_slist_append(chunk, authorization.c_str());
		chunk = curl_slist_append(chunk, app_name.c_str());

		// set header.
		opt = curl_easy_setopt(c, CURLOPT_HTTPHEADER, chunk);

		// set URL.
		opt = curl_easy_setopt(c, CURLOPT_URL, url.c_str());
		if (opt != CURLE_OK)
			cout << "Failed to set URL: " << curl_easy_strerror(opt) << endl;

		// Verify SSL Connection
		opt = curl_easy_setopt(c, CURLOPT_SSL_VERIFYPEER, 0L);
		if (opt != CURLE_OK)
			cout << "Failed to set SSL_VERIFYPEER: " << curl_easy_strerror(opt) << endl;

		opt = curl_easy_setopt(c, CURLOPT_SSL_VERIFYHOST, 0L);
		if (opt != CURLE_OK)
			cout << "Failed to set SSL_VERIFYHOST: " << curl_easy_strerror(opt) << endl;

		opt = curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, writeFunction);
		if (opt != CURLE_OK)
			cout << "Failed to set WRITEFUNCTION: " << curl_easy_strerror(opt) << endl;

		opt = curl_easy_setopt(c, CURLOPT_WRITEDATA, &s);
		if (opt != CURLE_OK)
			cout << "Failed to set SSL_WRITEDATA: " << curl_easy_strerror(opt) << endl;

		res = curl_easy_perform(c);
		if (res != CURLE_OK)
			cout << "Unable to send the request: " << curl_easy_strerror(res) << endl;

		curl_easy_cleanup(c);
	}
	curl_global_cleanup();
	return s;
}

string GCP::getPrinters(string access_token) {

	/* This string has /search as endpoint (proxy interface) which should
	 * list of all printers as JSON.
	 * Please see : https://developers.google.com/cloud-print/docs/proxyinterfaces
	 * */
	string url = "https://www.google.com/cloudprint/search?access_token="+access_token;

	return GCP::makeCURLRequest(url);
}

void GCP::parseJSON(string s) {
	GCP::error = "";
	GCP::error_description = "";

	size_t start, end;

	start = s.find("\"access_token\": \"");
	if (start != string::npos) {
		start += strlen("\"access_token\": \"");
		end = s.find("\"", start);
		GCP::access_token = s.substr(start, end - start);
	}

	start = s.find("\"token_type\": \"");
	if (start != string::npos) {
		start += strlen("\"token_type\": \"");
		end = s.find("\"", start);
		GCP::token_type = s.substr(start, end - start);
	}

	start = s.find("\"expires_in\": ");
	if (start != string::npos) {
		start += strlen("\"expires_in\": ");
		end = s.find(",", start);
		if (end == string::npos)
			end = s.find("\n", start);
		GCP::expires_in = s.substr(start, end - start);
	}

	start = s.find("\"refresh_token\": \"");
	if (start != string::npos) {
		start += strlen("\"refresh_token\": \"");
		end = s.find("\"", start);
		GCP::refresh_token = s.substr(start, end - start);
	}

	start = s.find("\"error\": \"");
	if (start != string::npos) {
		start += strlen("\"error\": \"");
		end = s.find("\"", start);
		GCP::error = s.substr(start, end - start);
	}

	start = s.find("\"error_description\": \"");
	if (start != string::npos) {
		start += strlen("\"error_description\": \"");
		end = s.find("\"", start);
		GCP::error_description = s.substr(start, end - start);
	}

	return;
}

vector< pair<string, string> > GCP::getPrintersFromJSON(string s){
	/*
	* Extract a list of printer id and names from received JSON and return it as a vector of pairs.
	*/

	vector< pair<string, string> > printers;
	size_t start, end;

	start = s.find("displayName");
	while(start != string::npos){
		start += 15;
		end = s.find("\"", start);
		string printer_name = s.substr(start, end-start);
		s = s.substr(end);
		start = s.find("id");
		start += 6;
		end = s.find("\"", start);
		string printer_id = s.substr(start, end-start);
		printers.push_back(make_pair(printer_id, printer_name));
		s = s.substr(end);
		start = s.find("displayName");
	}
	return printers;
}

string GCP::authorize() {
	/*
	 * Authorize app to manage user's printers.
	 */
	CURL *c;
	CURLcode opt, res;
	string response;
	string s;

	curl_global_init(CURL_GLOBAL_ALL);
	c = curl_easy_init();

	if (c) {
		string postfields;
		string code;

		cout << "Please enter this URL in your browser:" << endl << endl;

		// set the authorization url with required parameters.
		string auth_url = GCP::auth_url + \
			"?scope=" + curl_easy_escape(c, GCP::scope.c_str(), GCP::scope.length()) + \
			"&response_type=code" + \
			"&redirect_uri=" + curl_easy_escape(c, GCP::redirect_uri.c_str(), GCP::redirect_uri.length()) + \
			"&client_id=" + GCP::client_id + \
			"&access_type=offline";

		cout << auth_url << endl << endl;

		cout << "Paste the access token here: ";
		cin >> code;

		opt = curl_easy_setopt(c, CURLOPT_URL, GCP::api_url.c_str());
		if (opt != CURLE_OK)
			cout << "Failed to set URL: " << curl_easy_strerror(opt) << endl;

		// set the required postfields.
		postfields = "code=" + code + \
				"&client_id=" + GCP::client_id + \
				"&client_secret=" + GCP::client_secret + \
				"&grant_type=authorization_code" + \
				"&redirect_uri=" + curl_easy_escape(c, GCP::redirect_uri.c_str(), GCP::redirect_uri.length());

		opt = curl_easy_setopt(c, CURLOPT_POSTFIELDS, postfields.c_str());
		if (opt != CURLE_OK)
			cout << "Failed to set POSTFIELDS: " << curl_easy_strerror(opt) << endl;

		// Verify SSL Connection
		opt = curl_easy_setopt(c, CURLOPT_SSL_VERIFYPEER, 0L);
		if (opt != CURLE_OK)
			cout << "Failed to set SSL_VERIFYPEER: " << curl_easy_strerror(opt) << endl;

		opt = curl_easy_setopt(c, CURLOPT_SSL_VERIFYHOST, 0L);
		if (opt != CURLE_OK)
			cout << "Failed to set SSL_VERIFYHOST: " << curl_easy_strerror(opt) << endl;

		opt = curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, writeFunction);
		if (opt != CURLE_OK)
			cout << "Failed to set WRITEFUNCTION: " << curl_easy_strerror(opt) << endl;

		opt = curl_easy_setopt(c, CURLOPT_WRITEDATA, &s);
		if (opt != CURLE_OK)
			cout << "Failed to set SSL_WRITEDATA: " << curl_easy_strerror(opt) << endl;

		res = curl_easy_perform(c);
		if (res != CURLE_OK)
			cout << "Unable to send the request: " << curl_easy_strerror(res) << endl;

		curl_easy_cleanup(c);
	}
	curl_global_cleanup();
	return s;
}
