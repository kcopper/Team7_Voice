#include <iostream>
#include <string>
#include "VoiceIt.h"
using namespace std;

int main() {
    VoiceIt myVoiceIt("908e399927154baab125a5c2c9dc36af");
	string createResponse = myVoiceIt.createUser("donavan.2@wright.edu","Pokemon1","Patrick","Donavan");
    string enrollResponse = myVoiceIt.createEnrollment("donavan.2@wright.edu","Pokemon1", "rec_4s.wav","en-US");
    string authResponse = myVoiceIt.authentication("donavan.2@wright.edu","Pokemon1","rec_4s.wav","90","en-US");

    return 0;
}
