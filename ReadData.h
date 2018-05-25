#ifndef READDATA_H
#define READDATA_H

#include <string>
using namespace std;
class ReadData {
	private:
		string m_nt;
		int m_refPos;
		int m_baseQuality;
public:
	ReadData(string nt, int refPos, int baseQuality);
	string getNt() {return m_nt;};
	int getRefPos() {return m_refPos;};
	int getQuality() {return m_baseQuality;};


	
		
};

#endif