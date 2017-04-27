#include <assert.h>
#include <cstdlib>
#include <string>
#include "VoiceIt.h"

using namespace std;

//Unit tests

void test_Obj_VoiceIt(){
	cout<<"Testing VoiceIt object class."<<endl;
	try{
		VoiceIt myVoiceIt("908e399927154baab125a5c2c9dc36af");
	}
	catch(exception e){
		cout<<"Failed to create VoiceIt Object."<<endl;
	}
	cout<<"Object creation succeeded."<<endl;
	VoiceIt myVoiceIt("908e399927154baab125a5c2c9dc36af");
	cout<<"Testing object methods."<<endl;
	//Object method tests
	myVoiceIt.deleteUser("test@email.com", "testing");
	//createUser
	string result = myVoiceIt.createUser("test@email.com", "testing", "Test", "McTester");
	cout<<"Create User Test Success. "<<result<<endl;
	result = myVoiceIt.createUser("test", "invalid", "invalid", "invalid");
	cout<<"Create User Test Invalid Email. "<<result<<endl;
	result = myVoiceIt.createUser("test@email.com", "testing", "Test", "McTester");
	cout<<"Create User Test Already Exists. "<<result<<endl;
	result = myVoiceIt.createUser("test2@email.com", "testing2", "", "Testington");
	cout<<"Create User Test No First. "<<result<<endl;
	result = myVoiceIt.createUser("test2@email.com", "testing2", "Rusty", "");
	cout<<"Create User Test No Last. "<<result<<endl;
	//getUser
	result = myVoiceIt.getUser("test@email.com", "testing");
	cout<<"Get User Test Success"<<result<<endl;
	//setUser
	result = myVoiceIt.setUser("test@email.com", "testing", "Test", "McTester");
	cout<<"Set User Test"<<result<<endl;
	//createEnrollment
	cout<<"Authentication requires 3 valid enrollments"<<endl;
	result = myVoiceIt.createEnrollment("test@email.com", "testing", "TestPhraseEnroll1.wav", "en-US");
	cout<<"Create Enrollment1 Test Success. "<<result<<endl;
	result = myVoiceIt.createEnrollment("test@email.com", "testing", "TestPhraseEnroll2.wav", "en-US");
	cout<<"Create Enrollment2 Test Success. "<<result<<endl;
	result = myVoiceIt.createEnrollment("test@email.com", "testing", "TestPhraseEnroll3.wav", "en-US");
	cout<<"Create Enrollment3 Test Success. "<<result<<endl;
	//getEnrollment
	result = myVoiceIt.getEnrollments("test@email.com", "testing");
	cout<<"Get Enrollment Test Success. "<<result<<endl;
	size_t start = result.find("[") + 1;
	size_t end = 7;
	string num = result.substr(start, end);
	//authentication
	result = myVoiceIt.authentication("test@email.com", "testing", "TestPhraseAuth.wav", "90", "en-US");
	cout<<"Authentication Test Success. Confidence 90 "<<result<<endl;
	result = myVoiceIt.authentication("test@email.com", "testing", "TestPhraseLow.wav", "90", "en-US");
	cout<<"Authentication Test Fail, too low. Confidence 90"<<result<<endl;
	result = myVoiceIt.authentication("test@email.com", "testing", "TestPhraseHigh.wav", "90", "en-US");
	cout<<"Authentication Test Fail, too high. Confidence 90"<<result<<endl;
	//deleteEnrollment

	result = myVoiceIt.deleteEnrollment("test@email.com", "testing", num);
	cout<<"Delete Enrollment Test Success. "<<result<<endl;
	//deleteUser
	result = myVoiceIt.deleteUser("test@email.com", "testing");
	cout<<"Delete User Test"<<result<<endl;
}

int main(){
	test_Obj_VoiceIt();
	return 0;
}

