#include <assert>
#include <string>
#include "VoiceIt.h"

using namespace std;

//Unit tests

void test_Obj_VoiceIt(){
	cout<<"Testing VoiceIt object class."<<endl;
	try{
		VoiceIt myVoiceIt = VoiceIt("908e399927154baab125a5c2c9dc36af");
	}
	catch(exception e){
		cout<<"Failed to create VoiceIt Object."<<endl;
	}
	cout<<"Object creation succeeded."<<endl;
	cout<<"Testing object methods."<<endl;
	//Object method tests
	//createUser
	string result = myVoiceIt.createUser("test@email.com", "testing", "Test", "McTester");
	cout<<"User Test created"<<result<<endl;
	//getUser
	result = myVoiceIt.getUser("test@email.com", "testing");
	cout<<"Get User test"<<result<<endl;
	//setUser
	myVoiceIt.setUser("email2@email.com", "new_pass", "Rusty", "Testington");
	//deleteUser
	myVoiceIt.deleteUser();
	//createEnrollment
	myVoiceIt.createEnrollment();
	//createEnrollmentByWavUrl
	myVoiceIt.createEnrollmentByWavUrl();
	//getEnrollment
	myVoiceIt.getEnrollment();
	//deleteEnrollment
	myVoiceIt.deleteEnrollment();
	//authentication
	myVoiceIt.authentication();
	//authenticationByWavUrl
	myvoiceIt.authenticationByWavUrl();
}

