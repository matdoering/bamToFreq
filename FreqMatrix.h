#ifndef FREQMATRIX_H
#define FREQMATRIX_H

#include <string>
#include <map>
#include <vector>
#include <climits>
#include "ReadData.h"
#include "CodonData.h"

using namespace std;


class FreqMatrix {
	
private:
	map<string, vector<int> > m_freqMatrix;
	CodonData *m_codonMatrix;
	vector<CodonData> m_codonFrames; // matrices for 1st, 2nd, and 3rd reading frame
	static size_t m_defaultNumberOfRows; // viral genomes are small
	void addEvidence(int pos, string obs); // increment evidence at position 'pos' for observation 'obs'
	void addEvidenceCodon(int pos, string obs, int readingFrame); // increment evidence at position 'pos' for observation 'obs'
	void addEvidenceCodonInsertion(int pos, vector<string> &obs, int readingFrame);
	int m_minPos = INT_MAX;
	int m_maxPos = 0; // max position at which value was inserted in freqMatrix
	string m_inputFname;
	void initMatrix(map<string, vector<int> > &freqMatrix, vector<string>& header, vector<int>& readCounts, vector<int> &posVector);
	int findReadingFrame(); // find the right reading frame for the codon matrix
	string translate(string ntSeq);
	static string getConsensus(CodonData& freqFile);
	map<int, vector<string> > getCodons(vector<ReadData> &matches, int readingFrame);

	static map<string, string> m_nt2AA;
	static map<string,string> create_nt2AAMap() {
		map<string,string> nt2AA;
		nt2AA["ACN"] = "T";
		nt2AA["CCS"] = "P";
		nt2AA["TAG"] = "*";
		nt2AA["GGA"] = "G";
		nt2AA["CCD"] = "P";
		nt2AA["AGR"] = "R";
		nt2AA["ATM"] = "I";
		nt2AA["GCC"] = "A";
		nt2AA["GTH"] = "V";
		nt2AA["GTG"] = "V";
		nt2AA["GTT"] = "V";
		nt2AA["ACV"] = "T";
		nt2AA["CGM"] = "R";
		nt2AA["---"] = "-";
		nt2AA["TRA"] = "*";
		nt2AA["GCA"] = "A";
		nt2AA["CCK"] = "P";
		nt2AA["TCG"] = "S";
		nt2AA["AAA"] = "K";
		nt2AA["TCH"] = "S";
		nt2AA["ACY"] = "T";
		nt2AA["TAT"] = "Y";
		nt2AA["TCV"] = "S";
		nt2AA["NNN"] = "X";
		nt2AA["MGR"] = "R";
		nt2AA["TTC"] = "F";
		nt2AA["GCB"] = "A";
		nt2AA["CGV"] = "R";
		nt2AA["CTC"] = "L";
		nt2AA["GAR"] = "E";
		nt2AA["ACW"] = "T";
		nt2AA["CCM"] = "P";
		nt2AA["CTS"] = "L";
		nt2AA["TCW"] = "S";
		nt2AA["ACG"] = "T";
		nt2AA["AAR"] = "K";
		nt2AA["CTH"] = "L";
		nt2AA["TCM"] = "S";
		nt2AA["CCT"] = "P";
		nt2AA["TCY"] = "S";
		nt2AA["TTR"] = "L";
		nt2AA["GCK"] = "A";
		nt2AA["TAA"] = "*";
		nt2AA["CGW"] = "R";
		nt2AA["TCN"] = "S";
		nt2AA["GTM"] = "V";
		nt2AA["TCT"] = "S";
		nt2AA["GTK"] = "V";
		nt2AA["TTY"] = "F";
		nt2AA["AAC"] = "N";
		nt2AA["CCN"] = "P";
		nt2AA["GCY"] = "A";
		nt2AA["AAT"] = "N";
		nt2AA["CTB"] = "L";
		nt2AA["GTY"] = "V";
		nt2AA["ATT"] = "I";
		nt2AA["GGY"] = "G";
		nt2AA["ATC"] = "I";
		nt2AA["CAA"] = "Q";
		nt2AA["CGN"] = "R";
		nt2AA["CTK"] = "L";
		nt2AA["GAC"] = "D";
		nt2AA["CTR"] = "L";
		nt2AA["GGK"] = "G";
		nt2AA["ACD"] = "T";
		nt2AA["AGC"] = "S";
		nt2AA["CAY"] = "H";
		nt2AA["GGR"] = "G";
		nt2AA["TCS"] = "S";
		nt2AA["GTB"] = "V";
		nt2AA["GTC"] = "V";
		nt2AA["ACM"] = "T";
		nt2AA["CCG"] = "P";
		nt2AA["GTN"] = "V";
		nt2AA["GCG"] = "A";
		nt2AA["CGH"] = "R";
		nt2AA["GCT"] = "A";
		nt2AA["TGY"] = "C";
		nt2AA["..."] = ".";
		nt2AA["TAY"] = "Y";
		nt2AA["ACT"] = "T";
		nt2AA["TTA"] = "L";
		nt2AA["GCD"] = "A";
		nt2AA["GGM"] = "G";
		nt2AA["GTV"] = "V";
		nt2AA["CCV"] = "P";
		nt2AA["TCD"] = "S";
		nt2AA["CGG"] = "R";
		nt2AA["GGD"] = "G";
		nt2AA["AGA"] = "R";
		nt2AA["GCM"] = "A";
		nt2AA["TCK"] = "S";
		nt2AA["ACR"] = "T";
		nt2AA["TAC"] = "Y";
		nt2AA["GTD"] = "V";
		nt2AA["CTD"] = "L";
		nt2AA["GGG"] = "G";
		nt2AA["CGD"] = "R";
		nt2AA["ATA"] = "I";
		nt2AA["CAG"] = "Q";
		nt2AA["ACB"] = "T";
		nt2AA["CGY"] = "R";
		nt2AA["AGY"] = "S";
		nt2AA["CTT"] = "L";
		nt2AA["GGT"] = "G";
		nt2AA["CTM"] = "L";
		nt2AA["GAA"] = "E";
		nt2AA["GGC"] = "G";
		nt2AA["TGC"] = "C";
		nt2AA["TGT"] = "C";
		nt2AA["CCW"] = "P";
		nt2AA["YTR"] = "L";
		nt2AA["ATH"] = "I";
		nt2AA["GTR"] = "V";
		nt2AA["CCA"] = "P";
		nt2AA["GCN"] = "A";
		nt2AA["TGA"] = "*";
		nt2AA["GTW"] = "V";
		nt2AA["ACS"] = "T";
		nt2AA["CAT"] = "H";
		nt2AA["CCH"] = "P";
		nt2AA["GGN"] = "G";
		nt2AA["ACC"] = "T";
		nt2AA["ATY"] = "I";
		nt2AA["CGA"] = "R";
		nt2AA["GTA"] = "V";
		nt2AA["TCB"] = "S";
		nt2AA["CTW"] = "L";
		nt2AA["GTS"] = "V";
		nt2AA["TCR"] = "S";
		nt2AA["GAY"] = "D";
		nt2AA["CGR"] = "R";
		nt2AA["MGA"] = "R";
		nt2AA["ACK"] = "T";
		nt2AA["TAR"] = "*";
		nt2AA["AGT"] = "S";
		nt2AA["***"] = "-";
		nt2AA["CGS"] = "R";
		nt2AA["CCY"] = "P";
		nt2AA["AAG"] = "K";
		nt2AA["CCB"] = "P";
		nt2AA["ATW"] = "I";
		nt2AA["CGK"] = "R";
		nt2AA["CTG"] = "L";
		nt2AA["GAG"] = "E";
		nt2AA["ATG"] = "M";
		nt2AA["CGB"] = "R";
		nt2AA["TTG"] = "L";
		nt2AA["GCS"] = "A";
		nt2AA["CTV"] = "L";
		nt2AA["AGG"] = "R";
		nt2AA["ACH"] = "T";
		nt2AA["TCA"] = "S";
		nt2AA["GGW"] = "G";
		nt2AA["CGT"] = "R";
		nt2AA["CCR"] = "P";
		nt2AA["YTG"] = "L";
		nt2AA["AAY"] = "N";
		nt2AA["CAR"] = "Q";
		nt2AA["TCC"] = "S";
		nt2AA["CCC"] = "P";
		nt2AA["GAT"] = "D";
		nt2AA["CTA"] = "L";
		nt2AA["GGB"] = "G";
		nt2AA["GGS"] = "G";
		nt2AA["TGG"] = "W";
		nt2AA["CAC"] = "H";
		nt2AA["GCW"] = "A";
		nt2AA["MGG"] = "R";
		nt2AA["GCV"] = "A";
		nt2AA["GGH"] = "G";
		nt2AA["GCH"] = "A";
		nt2AA["ACA"] = "T";
		nt2AA["YTA"] = "L";
		nt2AA["TTT"] = "F";
		nt2AA["CGC"] = "R";
		nt2AA["CTN"] = "L";
		nt2AA["GCR"] = "A";
		nt2AA["CTY"] = "L";
		nt2AA["GGV"] = "G";
		return nt2AA;
	}


	
public:
	void storeRead(vector<ReadData> &matches);
	void storeReadCodon(vector<ReadData> &matches);
	void storeInsertions(vector<ReadData> &insertions);
	void writeCSV(map<string, vector<int> > &freqMatrix, string suffix);
	void writeCodonCSV(map<string, vector<string> > &freqMatrix, string suffix);
	void writeCSVs();
	FreqMatrix(char* samPath);
	template<typename T>
	static void truncateMatrix(map<string, vector<T> > &freqMatrix, int minPos, int maxPos) {
	for (typename map<string, vector<T> >::iterator it = freqMatrix.begin(); it != freqMatrix.end(); ++it) {
		typename vector<T>::const_iterator first = it->second.begin() + minPos;
		typename vector<T>::const_iterator last = it->second.begin() + maxPos;
		vector<T>truncVec(first,last);
		freqMatrix[it->first] = truncVec;
	}
}
	void truncateCodonMatrix(); // remove positions that are all 0s from codon matrix
	void truncateFreqMatrix(); // remove positions that are all 0s
	void setCodonMatrix(); // select the correct reading frame from the generated codon distributions
};

#endif