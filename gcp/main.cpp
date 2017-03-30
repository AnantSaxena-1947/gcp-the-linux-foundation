#include "utils.h"
#include <unistd.h>

using namespace std;

int main(int argc, char **argv) {
	GCP gcp;
	vector< pair<string, string> > printers;
	vector<string> printer_options;
	int option;

	// Authorize app and parse JSON response and set class variables ("access_token" etc...).
	gcp.parseJSON(gcp.authorize());

	if (gcp.error.length() > 0)
		cout << "Authentication Error: " << gcp.error_description << endl;
	else
		cout << "Auth Access Token   : " << gcp.access_token << endl;

	// Get a list of printers of this Authorized User.
	printers = gcp.getPrintersFromJSON(gcp.getPrinters(gcp.access_token));

	cout << "Select : \n1) Display list of printers\n2) Show options for a printer\n3) Submit print job\n";
	cin >> option;
	int i;
	switch(option){
	case 1:
		cout << "List of Google Cloud Printers:" << endl;
		i = 1;
		for(vector< pair<string, string> >::iterator it = printers.begin();it!=printers.end();++it, ++i)
			cout << "(" << i << ")" << it->second << endl;
		break;

	case 2:
		cout << "Select a printer from the following Google Cloud Printers:" << endl;
		i = 1;
		int choice;
		for(vector< pair<string, string> >::iterator it = printers.begin();it!=printers.end();++it, ++i)
			cout << "(" << i << ")" << it->second << endl;
		cin >> choice;
		i = 1;
		cout << "Relevant printer options are:" << endl;
		printer_options = gcp.getPrinterOptions(printers[choice-1].first, gcp.access_token);
		for(vector<string>::iterator it = printer_options.begin();it!=printer_options.end();++it, ++i)
			cout << "(" << i << ")" << *it << endl;
		break;
	case 3:
		// Do something to submit a print job
		break;
	default:
		cout << "Choose a valid option" << endl;
		break;
	}

	return 0;
}
