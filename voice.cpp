#include <iostream>
#include "VoiceIt.h"
using namespace std;

int main() {
    VoiceIt myVoiceIt("908e399927154baab125a5c2c9dc36af");
	//test change.
	String reponce = myVoiceIt.createUser("donavan.2@wright.edu","Pokemon1","Patrick","Donavan","513-484-5459");
    String reponce = myVoiceIt.createEnrollment("donavan.2@wright.edu","Pokemon1","/home/user/username/voicePrint.wav","en-US");
    String reponce = myVoiceIt.authentication("donavan.2@wright.edu","Pokemon1","/home/user/username/voicePrint.wav","90","en-US");

    return 0;
}
