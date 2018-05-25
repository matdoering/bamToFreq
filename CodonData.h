#ifndef CODONDATA_H
#define CODONDATA_H

#include <string>
#include <map>
#include <vector>

using namespace std;

class CodonData {
	private:
		map<string, vector<int> > m_matches;
		map<int, vector<map<string, int > > > m_insertions; // pos -> [codon -> evidence, ...]
		map<string, vector<string> > m_allData; // matches + insertions: for output
		void mergeData(); // merge insertions into m_allData
		
public:
	CodonData();
	CodonData(map<string, vector<int> > matches);
	map<string, vector<int> >& getMatches() {return m_matches;};
	map<int, vector<map<string, int > > >& getInsertions() {return m_insertions;};
	map<string, vector<string> >& getAllData();


	
		
};

#endif