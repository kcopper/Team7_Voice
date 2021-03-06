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
	result = myVoiceIt.createUser("test2@email.com", "testing2", "", "Burgundy");
	cout<<"Create User Test No First. "<<result<<endl;
	result = myVoiceIt.createUser("test2@email.com", "testing2", "Ron", "");
	cout<<"Create User Test No Last. "<<result<<endl;
	//getUser
	result = myVoiceIt.getUser("test@email.com", "testing");
	cout<<"Get User Test Success. "<<result<<endl;
	result = myVoiceIt.getUser("test2@email.com", "testing");
	cout<<"Get User Test User Not Found."<<result<<endl;
	result = myVoiceIt.getUser("email.com", "testing");
	cout<<"Get User Test Invalid Email"<<result<<endl;
	result = myVoiceIt.getUser("test@email.com", "test");
	cout<<"Get User Test Incorrect Password"<<result<<endl;
	//setUser
	result = myVoiceIt.setUser("test@email.com", "testing", "Test", "McTester");
	cout<<"Set User Test Success. "<<result<<endl;
	result = myVoiceIt.setUser("email.com", "testing", "Test", "McTester");
	cout<<"Set User Test Invalid Email. "<<result<<endl;
	result = myVoiceIt.setUser("test@email.com", "test", "Test", "McTester");
	cout<<"Set User Test Incorrect Password. "<<result<<endl;
	result = myVoiceIt.setUser("test2@email.com", "tes", "Ron", "Burgundy");
	cout<<"Set User Test User Not Found. "<<result<<endl;
	//createEnrollment
	result = myVoiceIt.createEnrollment("test@email.com", "testing", "TestPhraseWrong.wav", "en-US");
	cout<<"Create Enrollment Test Wrong Phrase. "<<result<<endl;
	result = myVoiceIt.createEnrollment("email.com", "testing", "TestPhraseEnroll1.wav", "en-US");
	cout<<"Create Enrollment Test Invalid Email. "<<result<<endl;
	result = myVoiceIt.createEnrollment("test@email.com", "test", "TestPhraseEnroll1.wav", "en-US");
	cout<<"Create Enrollment Test Invalid Password. "<<result<<endl;
	result = myVoiceIt.createEnrollment("test2@email.com", "testing2", "TestPhraseEnroll1.wav", "en-US");
	cout<<"Create Enrollment Test User Not Found. "<<result<<endl;
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
	result = myVoiceIt.getEnrollments("email.com", "testing");
	cout<<"Get Enrollment Invalid Email. "<<result<<endl;
	result = myVoiceIt.getEnrollments("test@email.com", "tes");
	cout<<"Get Enrollment Incorrect Password. "<<result<<endl;
	//authentication
	result = myVoiceIt.authentication("test@email.com", "testing", "TestPhraseAuth.wav", "90", "en-US");
	cout<<"Authentication Test Success. Confidence 90 "<<result<<endl;
	result = myVoiceIt.authentication("test@email.com", "testing", "TestPhraseLow.wav", "90", "en-US");
	cout<<"Authentication Test Fail, too low. Confidence 90"<<result<<endl;
	result = myVoiceIt.authentication("test@email.com", "testing", "TestPhraseHigh.wav", "90", "en-US");
	cout<<"Authentication Test Fail, too high. Confidence 90"<<result<<endl;
	result = myVoiceIt.authentication("test@email.com", "testing", "TestPhraseWrong.wav", "90", "en-US");
	cout<<"Authentication Test Fail, Wrong Phrase. Confidence 90"<<result<<endl;
	result = myVoiceIt.authentication("test@email.com", "testing", "TestPhraseAuth.wav", "90", "en-US");
	cout<<"Authentication Test Lockout. Confidence 90 "<<result<<endl;
	//deleteEnrollment
	result = myVoiceIt.deleteEnrollment("email.com", "testing", num);
	cout<<"Delete Enrollment Test Invalid Email. "<<result<<endl;
	result = myVoiceIt.deleteEnrollment("test@email.com", "tes", num);
	cout<<"Delete Enrollment Test Incorrect Password. "<<result<<endl;
	result = myVoiceIt.deleteEnrollment("test@email.com", "testing", "127");
	cout<<"Delete Enrollment Test ID Doesn't Exist. "<<result<<endl;
	result = myVoiceIt.deleteEnrollment("test@email.com", "testing", num);
	cout<<"Delete Enrollment Test Success. "<<result<<endl;
	//deleteUser
	result = myVoiceIt.deleteUser("email.com", "testing");
	cout<<"Delete User Invalid Email. "<<result<<endl;
	result = myVoiceIt.deleteUser("test@email.com", "test");
	cout<<"Delete User Test Incorrect Password. "<<result<<endl;
	result = myVoiceIt.deleteUser("test@email.com", "testing");
	cout<<"Delete User Test Success. "<<result<<endl;
	cout<<"Testing Complete."<<endl;
}

int main(){
	test_Obj_VoiceIt();
	return 0;
}

