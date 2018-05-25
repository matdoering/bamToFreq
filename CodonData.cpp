#include "CodonData.h"
#include <iostream>

CodonData::CodonData() {
	
}

CodonData::CodonData(map<string, vector<int> > matches) {
	m_matches = matches;
}
map<string, vector<string> >& CodonData::getAllData() {
	if (m_allData.size() != 0) {
		// all data was computed previously
		return m_allData;
	} else {
		mergeData();
		return m_allData;
	}
}

void CodonData::mergeData() {
	//std::cout << "mergeData for codon insertions ... " << std::endl;
	map<string, vector<string> > allData;
	for (map<string, vector<int> >::iterator it = m_matches.begin(); it != m_matches.end(); ++it) {
		string codon = it->first;
		//std::cout << "Codon is: " << codon << std::endl;
		map<int, vector<map<string, int > > >::iterator insIt = m_insertions.begin();
		for (size_t refPos = 0; refPos < it->second.size(); ++refPos) {
			allData[codon].push_back(to_string(it->second[refPos]));
			if (insIt != m_insertions.end() && refPos == insIt->first) { 
				// insert new entries into vector to represent the insertion; adjust the 'pos' entry!!
				for (size_t i = 0; i < insIt->second.size(); ++i) {
					string insVal = "";
					if (codon == "pos") {
						insVal = to_string(refPos) + "INS" + to_string(i);
					} else {
						if (insIt->second[i].find(codon) != insIt->second[i].end()) {
							// inserted codon was observed at least once
							insVal = to_string(insIt->second[i].at(codon));
						} else {
							insVal = to_string(0); // inserted codon was never observed
						}
					}
					allData[codon].push_back(insVal);
				}
				insIt++; // go to the next insertion entry
			}
		}
	}
	m_allData = allData;
}