#ifndef _VoiceIt__
#define _VoiceIt__
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <stdio.h>
#include <vector>
#include <curl/curl.h>
#include <sys/stat.h>
#include <fcntl.h>
using namespace std;

class SHA256
{
protected:
    typedef unsigned char uint8;
    typedef unsigned int uint32;
    typedef unsigned long long uint64;

    const static uint32 sha256_k[];
    static const unsigned int SHA224_256_BLOCK_SIZE = (512/8);
public:
    void init();
    void update(const unsigned char *message, unsigned int len);
    void final(unsigned char *digest);
    static const unsigned int DIGEST_SIZE = ( 256 / 8);

protected:
    void transform(const unsigned char *message, unsigned int block_nb);
    unsigned int m_tot_len;
    unsigned int m_len;
    unsigned char m_block[2*SHA224_256_BLOCK_SIZE];
    uint32 m_h[8];
};

std::string sha256(std::string input);

#define SHA2_SHFR(x, n)    (x >> n)
#define SHA2_ROTR(x, n)   ((x >> n) | (x << ((sizeof(x) << 3) - n)))
#define SHA2_ROTL(x, n)   ((x << n) | (x >> ((sizeof(x) << 3) - n)))
#define SHA2_CH(x, y, z)  ((x & y) ^ (~x & z))
#define SHA2_MAJ(x, y, z) ((x & y) ^ (x & z) ^ (y & z))
#define SHA256_F1(x) (SHA2_ROTR(x,  2) ^ SHA2_ROTR(x, 13) ^ SHA2_ROTR(x, 22))
#define SHA256_F2(x) (SHA2_ROTR(x,  6) ^ SHA2_ROTR(x, 11) ^ SHA2_ROTR(x, 25))
#define SHA256_F3(x) (SHA2_ROTR(x,  7) ^ SHA2_ROTR(x, 18) ^ SHA2_SHFR(x,  3))
#define SHA256_F4(x) (SHA2_ROTR(x, 17) ^ SHA2_ROTR(x, 19) ^ SHA2_SHFR(x, 10))
#define SHA2_UNPACK32(x, str)                 \
{                                             \
    *((str) + 3) = (uint8) ((x)      );       \
    *((str) + 2) = (uint8) ((x) >>  8);       \
    *((str) + 1) = (uint8) ((x) >> 16);       \
    *((str) + 0) = (uint8) ((x) >> 24);       \
}
#define SHA2_PACK32(str, x)                   \
{                                             \
    *(x) =   ((uint32) *((str) + 3)      )    \
           | ((uint32) *((str) + 2) <<  8)    \
           | ((uint32) *((str) + 1) << 16)    \
           | ((uint32) *((str) + 0) << 24);   \
}

const unsigned int SHA256::sha256_k[64] = //UL = uint32
            {0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
             0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
             0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
             0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
             0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
             0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
             0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
             0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
             0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
             0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
             0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
             0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
             0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
             0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
             0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
             0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

void SHA256::transform(const unsigned char *message, unsigned int block_nb)
{
    uint32 w[64];
    uint32 wv[8];
    uint32 t1, t2;
    const unsigned char *sub_block;
    int i;
    int j;
    for (i = 0; i < (int) block_nb; i++) {
        sub_block = message + (i << 6);
        for (j = 0; j < 16; j++) {
            SHA2_PACK32(&sub_block[j << 2], &w[j]);
        }
        for (j = 16; j < 64; j++) {
            w[j] =  SHA256_F4(w[j -  2]) + w[j -  7] + SHA256_F3(w[j - 15]) + w[j - 16];
        }
        for (j = 0; j < 8; j++) {
            wv[j] = m_h[j];
        }
        for (j = 0; j < 64; j++) {
            t1 = wv[7] + SHA256_F2(wv[4]) + SHA2_CH(wv[4], wv[5], wv[6])
                + sha256_k[j] + w[j];
            t2 = SHA256_F1(wv[0]) + SHA2_MAJ(wv[0], wv[1], wv[2]);
            wv[7] = wv[6];
            wv[6] = wv[5];
            wv[5] = wv[4];
            wv[4] = wv[3] + t1;
            wv[3] = wv[2];
            wv[2] = wv[1];
            wv[1] = wv[0];
            wv[0] = t1 + t2;
        }
        for (j = 0; j < 8; j++) {
            m_h[j] += wv[j];
        }
    }
}

void SHA256::init()
{
    m_h[0] = 0x6a09e667;
    m_h[1] = 0xbb67ae85;
    m_h[2] = 0x3c6ef372;
    m_h[3] = 0xa54ff53a;
    m_h[4] = 0x510e527f;
    m_h[5] = 0x9b05688c;
    m_h[6] = 0x1f83d9ab;
    m_h[7] = 0x5be0cd19;
    m_len = 0;
    m_tot_len = 0;
}

void SHA256::update(const unsigned char *message, unsigned int len)
{
    unsigned int block_nb;
    unsigned int new_len, rem_len, tmp_len;
    const unsigned char *shifted_message;
    tmp_len = SHA224_256_BLOCK_SIZE - m_len;
    rem_len = len < tmp_len ? len : tmp_len;
    memcpy(&m_block[m_len], message, rem_len);
    if (m_len + len < SHA224_256_BLOCK_SIZE) {
        m_len += len;
        return;
    }
    new_len = len - rem_len;
    block_nb = new_len / SHA224_256_BLOCK_SIZE;
    shifted_message = message + rem_len;
    transform(m_block, 1);
    transform(shifted_message, block_nb);
    rem_len = new_len % SHA224_256_BLOCK_SIZE;
    memcpy(m_block, &shifted_message[block_nb << 6], rem_len);
    m_len = rem_len;
    m_tot_len += (block_nb + 1) << 6;
}

void SHA256::final(unsigned char *digest)
{
    unsigned int block_nb;
    unsigned int pm_len;
    unsigned int len_b;
    int i;
    block_nb = (1 + ((SHA224_256_BLOCK_SIZE - 9)
                     < (m_len % SHA224_256_BLOCK_SIZE)));
    len_b = (m_tot_len + m_len) << 3;
    pm_len = block_nb << 6;
    memset(m_block + m_len, 0, pm_len - m_len);
    m_block[m_len] = 0x80;
    SHA2_UNPACK32(len_b, m_block + pm_len - 4);
    transform(m_block, block_nb);
    for (i = 0 ; i < 8; i++) {
        SHA2_UNPACK32(m_h[i], &digest[i << 2]);
    }
}

std::string sha256(std::string input)
{
    unsigned char digest[SHA256::DIGEST_SIZE];
    memset(digest,0,SHA256::DIGEST_SIZE);

    SHA256 ctx = SHA256();
    ctx.init();
    ctx.update( (unsigned char*)input.c_str(), input.length());
    ctx.final(digest);

    char buf[2*SHA256::DIGEST_SIZE+1];
    buf[2*SHA256::DIGEST_SIZE] = 0;
    for (int i = 0; i < SHA256::DIGEST_SIZE; i++)
        sprintf(buf+i*2, "%02x", digest[i]);
    return std::string(buf);
}

static string readBuffer;

class VoiceIt{
private:
	string developerId= "";
	string urlUsers= "https://siv.voiceprintportal.com/sivservice/api/users";
	string urlEnrollments = "https://siv.voiceprintportal.com/sivservice/api/enrollments";
	string urlAuthentication = "https://siv.voiceprintportal.com/sivservice/api/authentications";



public:
	VoiceIt(string devId){
		cout<<"Constructor for VoiceIt Called"<<endl;
		developerId=devId;
		cout<<"Parameters Initialized"<<endl;
	}

static size_t WriteCallback(char *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    readBuffer.append(contents, realsize);
    return realsize;
}



struct WriteThis
 {
  const char *readptr;
  long sizeleft;
};

static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *userp)
{
  struct WriteThis *writeBuffer = (struct WriteThis *)userp;

  if(size*nmemb < 1)
    return 0;

  if(writeBuffer->sizeleft) {
    *(char *)ptr = writeBuffer->readptr[0]; /* copy one single byte */
    writeBuffer->readptr++;                 /* advance pointer */
    writeBuffer->sizeleft--;                /* less data left */
    return 1;                        /* we return 1 byte at a time! */
  }

  return 0;                          /* no more data left to deliver */
}

string getUser(string email,string password)
{
  CURL *curl;
  CURLcode res;
  string emailParam = "VsitEmail: "+email;
  string passParam =  "VsitPassword: "+sha256(password);
  string devIdParam = "VsitDeveloperId: "+developerId;
  string platformIdParam = "PlatformID: 11";
  readBuffer.clear();
  curl = curl_easy_init();
  if(curl) {
    struct curl_slist *chunk = NULL;
    chunk = curl_slist_append(chunk, emailParam.c_str());
    chunk = curl_slist_append(chunk, passParam.c_str());
    chunk = curl_slist_append(chunk, devIdParam.c_str());
    chunk = curl_slist_append(chunk, platformIdParam.c_str());
    curl_easy_setopt(curl, CURLOPT_URL, "https://siv.voiceprintportal.com/sivservice/api/users");
    res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
    /* always cleanup */
    curl_easy_cleanup(curl);
    //cout<<"Response is "<<readBuffer<<" For Sure"<<endl;
    return readBuffer;
    /* free the custom headers */
    curl_slist_free_all(chunk);
  }
}

string setUser(string email,string password,string firstName,string lastName)
{
  CURL *curl;
  CURLcode res;
  string emailParam = "VsitEmail: "+email;
  string passParam =  "VsitPassword: "+sha256(password);
  string devIdParam = "VsitDeveloperId: "+developerId;
  string fNameParam = "VsitFirstName: "+firstName;
  string lNameParam = "VsitLastName: "+lastName;
  string platformIdParam = "PlatformID: 11";
  curl_global_init(CURL_GLOBAL_ALL);
  readBuffer.clear();
  curl = curl_easy_init();
  if(curl) {
    struct curl_slist *chunk = NULL;
    chunk = curl_slist_append(chunk, emailParam.c_str());
    chunk = curl_slist_append(chunk, passParam.c_str());
    chunk = curl_slist_append(chunk, devIdParam.c_str());
    chunk = curl_slist_append(chunk, fNameParam.c_str());
    chunk = curl_slist_append(chunk, lNameParam.c_str());
    chunk = curl_slist_append(chunk, platformIdParam.c_str());
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
    curl_easy_setopt(curl, CURLOPT_URL, "https://siv.voiceprintportal.com/sivservice/api/users");
    res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
    /* always cleanup */
    curl_easy_cleanup(curl);
    //cout<<"Response is "<<readBuffer<<" For Sure"<<endl;
        /* free the custom headers */
    curl_slist_free_all(chunk);
    return readBuffer;
  }
}

string createUser(string email,string password,string firstName,string lastName, string phone1, string phone2, string phone3)
{
  CURL *curl;
  CURLcode res;
  string emailParam = "VsitEmail: "+email;
  string passParam =  "VsitPassword: "+sha256(password);
  string devIdParam = "VsitDeveloperId: "+developerId;
  string fNameParam = "VsitFirstName: "+firstName;
  string lNameParam = "VsitLastName: "+lastName;
  string phone1Param = "VsitPhone1: "+phone1;
  string phone2Param = "VsitPhone2: "+phone2;
  string phone3Param = "VsitPhone3: "+phone3;
  string platformIdParam = "PlatformID: 11";
  curl_global_init(CURL_GLOBAL_ALL);
  readBuffer.clear();
  curl = curl_easy_init();
  if(curl) {
    struct curl_slist *chunk = NULL;
    chunk = curl_slist_append(chunk, emailParam.c_str());
    chunk = curl_slist_append(chunk, passParam.c_str());
    chunk = curl_slist_append(chunk, devIdParam.c_str());
    chunk = curl_slist_append(chunk, fNameParam.c_str());
    chunk = curl_slist_append(chunk, lNameParam.c_str());
    chunk = curl_slist_append(chunk, phone1Param.c_str());
    chunk = curl_slist_append(chunk, phone2Param.c_str());
    chunk = curl_slist_append(chunk, phone3Param.c_str());
    chunk = curl_slist_append(chunk, platformIdParam.c_str());
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curl, CURLOPT_URL, "https://siv.voiceprintportal.com/sivservice/api/users");
    res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
    /* always cleanup */
    curl_easy_cleanup(curl);
    //cout<<"Response is "<<readBuffer<<" For Sure"<<endl;
        /* free the custom headers */
    curl_slist_free_all(chunk);
    return readBuffer;
  }
}
string createUser(string email,string password,string firstName,string lastName)
{
    return createUser(email, password, firstName, lastName, "","","");
}
string deleteUser(string email,string password)
{
  CURL *curl;
  CURLcode res;
  string emailParam = "VsitEmail: "+email;
  string passParam =  "VsitPassword: "+sha256(password);
  string devIdParam = "VsitDeveloperId: "+developerId;
  string platformIdParam = "PlatformID: 11";
  curl_global_init(CURL_GLOBAL_ALL);
  readBuffer.clear();
  curl = curl_easy_init();
  if(curl) {
    struct curl_slist *chunk = NULL;
    chunk = curl_slist_append(chunk, emailParam.c_str());
    chunk = curl_slist_append(chunk, passParam.c_str());
    chunk = curl_slist_append(chunk, devIdParam.c_str());
    chunk = curl_slist_append(chunk, platformIdParam.c_str());
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
    curl_easy_setopt(curl, CURLOPT_URL, "https://siv.voiceprintportal.com/sivservice/api/users");
    res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
    /* always cleanup */
    curl_easy_cleanup(curl);
    //cout<<"Response is "<<readBuffer<<" For Sure"<<endl;
        /* free the custom headers */
    curl_slist_free_all(chunk);
    return readBuffer;
  }
  return "";
}

string createEnrollment(string email,string passwrd, string pathToEnrollmentWav, string contentLanguage){
  CURL *curl;
  CURLcode res;
  struct stat file_info;
  //double speed_upload, total_time;
  FILE *fd;

  string emailParam = "VsitEmail: "+email;
  string passParam =  "VsitPassword: "+sha256(passwrd);
  string devIdParam = "VsitDeveloperId: "+developerId;
  string contLangParam = "ContentLanguage: "+contentLanguage;
  string platformIdParam = "PlatformID: 11";
  string cType = "Content-Type: audio/wav";
  string accept = "Accept: application/json";
  readBuffer.clear();

  fd = fopen(pathToEnrollmentWav.c_str(), "rb"); /* open file to upload */
  if(!fd) {

    return "Error"; /* can't continue */
  }

  /* to get the file size */
  if(fstat(fileno(fd), &file_info) != 0) {

    return "Error"; /* can't continue */
  }

  curl = curl_easy_init();
  if(curl) {
    /* upload to this place */
    struct curl_slist *chunk = NULL;
    chunk = curl_slist_append(chunk, emailParam.c_str());
    chunk = curl_slist_append(chunk, passParam.c_str());
    chunk = curl_slist_append(chunk, devIdParam.c_str());
    chunk = curl_slist_append(chunk, contLangParam.c_str());
    chunk = curl_slist_append(chunk, platformIdParam.c_str());
    chunk = curl_slist_append(chunk, cType.c_str());
    chunk = curl_slist_append(chunk, accept.c_str());
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curl, CURLOPT_URL, "https://siv.voiceprintportal.com/sivservice/api/enrollments");

    /* tell it to "upload" to the URL */
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

    /* set where to read from (on Windows you need to use READFUNCTION too) */
    curl_easy_setopt(curl, CURLOPT_READDATA, fd);

    /* and give the size of the upload (optional) */
    //curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,(curl_off_t)file_info.st_size);
    res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    /* enable verbose for easier tracing */
    //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    res = curl_easy_perform(curl);
    /* Check for errors */
    if(res != CURLE_OK) {
      //fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));

    }
    else {
      /* now extract transfer info */
      //curl_easy_getinfo(curl, CURLINFO_SPEED_UPLOAD, &speed_upload);
      //curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);

      //fprintf(stderr, "Speed: %.3f bytes/sec during %.3f seconds\n",speed_upload, total_time);

    }
    curl_slist_free_all(chunk);
    /* always cleanup */
    curl_easy_cleanup(curl);
    return readBuffer;
  }
  return "";
}

string createEnrollment(string email,string passwrd, string pathToEnrollmentWav){

  return createEnrollment(email,passwrd, pathToEnrollmentWav, "");
}

string createEnrollmentByWavURL(string email,string passwrd, string urlToEnrollmentWav, string contentLanguage){
  CURL *curl;
  CURLcode res;
  //double speed_upload, total_time;

  string emailParam = "VsitEmail: "+email;
  string passParam =  "VsitPassword: "+sha256(passwrd);
  string devIdParam = "VsitDeveloperId: "+developerId;
  string wavURLParam = "VsitwavURL: "+urlToEnrollmentWav;
  string contLangParam = "ContentLanguage: "+contentLanguage;
  string platformIdParam = "PlatformID: 11";
  string cType = "Content-Type: audio/wav";
  string accept = "Accept: application/json";
  readBuffer.clear();
  curl = curl_easy_init();
  if(curl) {
    /* upload to this place */
    struct curl_slist *chunk = NULL;
    chunk = curl_slist_append(chunk, emailParam.c_str());
    chunk = curl_slist_append(chunk, passParam.c_str());
    chunk = curl_slist_append(chunk, devIdParam.c_str());
    chunk = curl_slist_append(chunk, wavURLParam.c_str());
    chunk = curl_slist_append(chunk, contLangParam.c_str());
    chunk = curl_slist_append(chunk, platformIdParam.c_str());
    chunk = curl_slist_append(chunk, cType.c_str());
    chunk = curl_slist_append(chunk, accept.c_str());
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curl, CURLOPT_URL, "https://siv.voiceprintportal.com/sivservice/api/enrollments/bywavurl");


    /* and give the size of the upload (optional) */
    //curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,(curl_off_t)file_info.st_size);
    res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    /* enable verbose for easier tracing */
    //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    res = curl_easy_perform(curl);
    /* Check for errors */
    if(res != CURLE_OK) {
      //fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));

    }
    else {
      /* now extract transfer info */
      //curl_easy_getinfo(curl, CURLINFO_SPEED_UPLOAD, &speed_upload);
      //curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);

      //fprintf(stderr, "Speed: %.3f bytes/sec during %.3f seconds\n",speed_upload, total_time);

    }
    curl_slist_free_all(chunk);
    /* always cleanup */
    curl_easy_cleanup(curl);
    return readBuffer;
  }
  return "";
}

string createEnrollmentByWavURL(string email,string passwrd, string urlToEnrollmentWav){

  return createEnrollmentByWavURL(email,passwrd, urlToEnrollmentWav, "");
}

string getEnrollments(string email,string password)
{
  CURL *curl;
  CURLcode res;
  string emailParam = "VsitEmail: "+email;
  string passParam =  "VsitPassword: "+sha256(password);
  string devIdParam = "VsitDeveloperId: "+developerId;
  string platformIdParam = "PlatformID: 11";
  readBuffer.clear();
  curl = curl_easy_init();
  if(curl) {
    struct curl_slist *chunk = NULL;
    chunk = curl_slist_append(chunk, emailParam.c_str());
    chunk = curl_slist_append(chunk, passParam.c_str());
    chunk = curl_slist_append(chunk, devIdParam.c_str());
    chunk = curl_slist_append(chunk, platformIdParam.c_str());
    curl_easy_setopt(curl, CURLOPT_URL, "https://siv.voiceprintportal.com/sivservice/api/enrollments");
    res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
    /* always cleanup */
    curl_easy_cleanup(curl);
    //cout<<"Response is "<<readBuffer<<" For Sure"<<endl;
    return readBuffer;
    /* free the custom headers */
    curl_slist_free_all(chunk);
  }
 return "Error";
}

string deleteEnrollment(string email,string password,string enrollmentId)
{
  CURL *curl;
  CURLcode res;
  string finalURL = "https://siv.voiceprintportal.com/sivservice/api/enrollments/"+enrollmentId;
  string emailParam = "VsitEmail: "+email;
  string passParam =  "VsitPassword: "+sha256(password);
  string devIdParam = "VsitDeveloperId: "+developerId;
  string platformIdParam = "PlatformID: 11";
  curl_global_init(CURL_GLOBAL_ALL);
  readBuffer.clear();
  curl = curl_easy_init();
  if(curl) {
    struct curl_slist *chunk = NULL;
    chunk = curl_slist_append(chunk, emailParam.c_str());
    chunk = curl_slist_append(chunk, passParam.c_str());
    chunk = curl_slist_append(chunk, devIdParam.c_str());
    chunk = curl_slist_append(chunk, platformIdParam.c_str());
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
    curl_easy_setopt(curl, CURLOPT_URL, finalURL.c_str());
    res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
    /* always cleanup */
    curl_easy_cleanup(curl);
    //cout<<"Response is "<<readBuffer<<" For Sure"<<endl;
        /* free the custom headers */
    curl_slist_free_all(chunk);
    return readBuffer;
  }
  return "";
}

string authentication(string email,string passwrd, string pathToAuthenticationWav, string confidence, string contentLanguage){
  CURL *curl;
  CURLcode res;
  struct stat file_info;
  //double speed_upload, total_time;
  FILE *fd;

  string emailParam = "VsitEmail: "+email;
  string passParam =  "VsitPassword: "+sha256(passwrd);
  string devIdParam = "VsitDeveloperId: "+developerId;
  string confidenceParam = "VsitConfidence: "+confidence;
  string contLangParam = "ContentLanguage: "+contentLanguage;
  string platformIdParam = "PlatformID: 11";
  string cType = "Content-Type: audio/wav";
  string accept = "Accept: application/json";
  readBuffer.clear();

  fd = fopen(pathToAuthenticationWav.c_str(), "rb"); /* open file to upload */
  if(!fd) {

    return "Error"; /* can't continue */
  }

  /* to get the file size */
  if(fstat(fileno(fd), &file_info) != 0) {

    return "Error"; /* can't continue */
  }

  curl = curl_easy_init();
  if(curl) {
    /* upload to this place */
    struct curl_slist *chunk = NULL;
    chunk = curl_slist_append(chunk, emailParam.c_str());
    chunk = curl_slist_append(chunk, passParam.c_str());
    chunk = curl_slist_append(chunk, devIdParam.c_str());
    chunk = curl_slist_append(chunk, confidenceParam.c_str());
    chunk = curl_slist_append(chunk, contLangParam.c_str());
    chunk = curl_slist_append(chunk, platformIdParam.c_str());
    chunk = curl_slist_append(chunk, cType.c_str());
    chunk = curl_slist_append(chunk, accept.c_str());
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curl, CURLOPT_URL, "https://siv.voiceprintportal.com/sivservice/api/authentications");

    /* tell it to "upload" to the URL */
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

    /* set where to read from (on Windows you need to use READFUNCTION too) */
    curl_easy_setopt(curl, CURLOPT_READDATA, fd);

    /* and give the size of the upload (optional) */
    //curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,(curl_off_t)file_info.st_size);
    res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    /* enable verbose for easier tracing */
    //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    res = curl_easy_perform(curl);
    /* Check for errors */
    if(res != CURLE_OK) {
      //fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));

    }
    else {
      /* now extract transfer info */
      //curl_easy_getinfo(curl, CURLINFO_SPEED_UPLOAD, &speed_upload);
      //curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);

      //fprintf(stderr, "Speed: %.3f bytes/sec during %.3f seconds\n",speed_upload, total_time);

    }
    curl_slist_free_all(chunk);
    /* always cleanup */
    curl_easy_cleanup(curl);
    return readBuffer;
  }
  return "";
}

string authentication(string email,string passwrd, string pathToAuthenticationWav, string confidence){

  return authentication(email,passwrd,pathToAuthenticationWav,confidence,"");
}

string authenticationByWavURL(string email,string passwrd, string urlToAuthenticationWav, string confidence, string contentLanguage){
  CURL *curl;
  CURLcode res;
  //double speed_upload, total_time;

  string emailParam = "VsitEmail: "+email;
  string passParam =  "VsitPassword: "+sha256(passwrd);
  string devIdParam = "VsitDeveloperId: "+developerId;
  string confidenceParam = "VsitConfidence: "+confidence;
  string contLangParam = "ContentLanguage: "+contentLanguage;
  string wavURLParam = "VsitwavURL: "+urlToAuthenticationWav;
  string platformIdParam = "PlatformID: 11";
  string cType = "Content-Type: audio/wav";
  string accept = "Accept: application/json";
  readBuffer.clear();
  curl = curl_easy_init();
  if(curl) {
    /* upload to this place */
    struct curl_slist *chunk = NULL;
    chunk = curl_slist_append(chunk, emailParam.c_str());
    chunk = curl_slist_append(chunk, passParam.c_str());
    chunk = curl_slist_append(chunk, devIdParam.c_str());
    chunk = curl_slist_append(chunk, confidenceParam.c_str());
    chunk = curl_slist_append(chunk, wavURLParam.c_str());
    chunk = curl_slist_append(chunk, contLangParam.c_str());
    chunk = curl_slist_append(chunk, platformIdParam.c_str());
    chunk = curl_slist_append(chunk, cType.c_str());
    chunk = curl_slist_append(chunk, accept.c_str());
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curl, CURLOPT_URL, "https://siv.voiceprintportal.com/sivservice/api/authentications/bywavurl");


    /* and give the size of the upload (optional) */
    //curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,(curl_off_t)file_info.st_size);
    res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    /* enable verbose for easier tracing */
    //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    res = curl_easy_perform(curl);
    /* Check for errors */
    if(res != CURLE_OK) {
      //fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));

    }
    else {
      /* now extract transfer info */
      //curl_easy_getinfo(curl, CURLINFO_SPEED_UPLOAD, &speed_upload);
      //curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);

      //fprintf(stderr, "Speed: %.3f bytes/sec during %.3f seconds\n",speed_upload, total_time);

    }
    curl_slist_free_all(chunk);
    /* always cleanup */
    curl_easy_cleanup(curl);
    return readBuffer;
  }
  return "";
}

string authenticationByWavURL(string email,string passwrd, string urlToAuthenticationWav, string confidence){
  return authenticationByWavURL(email,passwrd,urlToAuthenticationWav,confidence,"");
}
  };

#endif  // _VoiceIt_