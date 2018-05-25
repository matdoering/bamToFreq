#include "FreqMatrix.h"
#include <iostream>
#include <fstream>   
#include <unordered_set>
#include <algorithm>

size_t FreqMatrix::m_defaultNumberOfRows = 100000;
map<string, string> FreqMatrix::m_nt2AA = create_nt2AAMap();

string FreqMatrix::translate(string ntSeq) {
	string aaSeq;
	for (int i = 0; i < ntSeq.length(); i = i+3) {
		string codon = ntSeq.substr(i, 3);
		if (codon.length() == 3) {
			string AA = m_nt2AA.at(codon);
			aaSeq += AA;
		}
	}
	return aaSeq;
}


FreqMatrix::FreqMatrix(char* samPath) {
	// initialize name of input sam file for csv output later
	m_inputFname = string(samPath);
	// initialize read counts with zeros
	vector<int> readCounts(m_defaultNumberOfRows,0);
	// initialize pos vector
	vector<int> posVector = readCounts;
	for (size_t i = 0; i < posVector.size(); ++i) {
		posVector[i] = i;
	}
	// initialize single-nucleotide matrix
	vector<string> header = {"A", "C", "G", "T", "N"};
	initMatrix(m_freqMatrix, header, readCounts, posVector);
	for (string obs : header) {
		m_freqMatrix[obs] = readCounts;
	}

	m_freqMatrix["pos"] = posVector;
	// construct codon matrices
	header = {"AAA", "AAC", "AAG", "AAT", "ACA", "ACC", "ACG", "ACT", "AGA", "AGC", "AGG", "AGT", "ATA", 
							"ATC", "ATG", "ATT", "CAA", "CAC", "CAG", "CAT", "CCA",	"CCC", "CCG", "CCT", "CGA", "CGC",
							"CGG", "CGT", "CTA", "CTC", "CTG", "CTT", "GAA", "GAC", "GAG", "GAT", "GCA", "GCC", "GCG", 
							"GCT", "GGA", "GGC", "GGG", "GGT", "GTA", "GTC", "GTG", "GTT", "TAA", "TAC", "TAG", "TAT",
							"TCA", "TCC", "TCG", "TCT", "TGA", "TGC", "TGG", "TGT", "TTA", "TTC", "TTG", "TTT", "---", 
							"other"};
	// init codon matrices
	
	for (size_t i = 0; i < 3; ++i) {
		// matrices for 1st, 2nd, and 3rd reading frame
		map<string, vector<int> > codonMatrix;
		initMatrix(codonMatrix, header, readCounts, posVector);
		CodonData cd = CodonData(codonMatrix);
		m_codonFrames.push_back(cd);
	}
}

void FreqMatrix::initMatrix(map<string, vector<int> > &freqMatrix, vector<string>& header, vector<int>& readCounts, 
							vector<int> &posVector) {
		
	for (string obs : header) {
		freqMatrix[obs] = readCounts;
	}

	freqMatrix["pos"] = posVector;

}

void FreqMatrix::addEvidence(int pos, string obs) {
	if (pos < 0) {
		throw std::invalid_argument("Read pos '" + to_string(pos) + "' was < 0");
	}
	//std::cout << "Cur Evidence at pos: " << pos << ", obs: " << obs << ": " << m_freqMatrix.at(obs)[pos] << std::endl;
	m_freqMatrix.at(obs)[pos] = m_freqMatrix.at(obs).at(pos) + 1;
}

void FreqMatrix::addEvidenceCodon(int pos, string obs, int readingFrame) {
	if (pos < 0) {
		throw std::invalid_argument("Read pos '" + to_string(pos) + "' was < 0");
	}
	if (readingFrame <0 || readingFrame >= 3) {
		throw std::invalid_argument("Reading frame '" + to_string(readingFrame) + "' should be in {0,1,2}.");
	}
	//std::cout << "Cur Evidence at pos: " << pos << ", obs: " << obs << ": " << m_freqMatrix.at(obs)[pos] << std::endl;
	m_codonFrames.at(readingFrame).getMatches().at(obs)[pos]++;
}
void FreqMatrix::addEvidenceCodonInsertion(int pos, vector<string>& obs, int readingFrame) {
	if (pos < 0) {
		throw std::invalid_argument("Read pos '" + to_string(pos) + "' was < 0");
	}
	if (readingFrame <0 || readingFrame >= 3) {
		throw std::invalid_argument("Reading frame '" + to_string(readingFrame) + "' should be in {0,1,2}.");
	}
	map<int, vector<map<string, int> > > &insertions = m_codonFrames.at(readingFrame).getInsertions();
	if (insertions.find(pos) == insertions.end()) {
		// create entry for insertion position
		insertions[pos] = vector<map<string, int> >();
	}
	for (size_t i = 0; i < obs.size(); ++i) {
		// check if insertion is already present in vector
		if (i >= insertions[pos].size()) {
			// no values are present yet
			map<string, int> mapEntry = {{obs[i], 1}};
			insertions[pos].push_back(mapEntry);
		} else {
			if (insertions[pos][i].find(obs[i]) != insertions[pos][i].end()) {
				// map entry already exists
				insertions[pos][i][obs[i]] += 1;
			} else {
				//map entry doesnt' exist
				insertions[pos][i][obs[i]] = 1;
			}
		}
	}
}
void FreqMatrix::truncateFreqMatrix() {
	truncateMatrix(m_freqMatrix, m_minPos, m_maxPos);
}

void FreqMatrix::truncateCodonMatrix() {
	// std::cout << "truncating codon matrix .." << std::endl;
	map<int, vector<map<string, int > > > ins = m_codonMatrix->getInsertions();
	int nbrInsertions = 0;
	for (map<int, vector<map<string, int > > >::iterator it = ins.begin(); it != ins.end(); ++it) {
		nbrInsertions += it->second.size();
	}
	int codonMaxPos = m_maxPos / 3 + nbrInsertions;
	int codonMinPos = ceil(m_minPos / 3); // divide by 3 to adjust for codon positions rather than single nt positions
	truncateMatrix(m_codonMatrix->getAllData(), codonMinPos, codonMaxPos);
}



void FreqMatrix::writeCSVs() {
	writeCSV(m_freqMatrix, "freqs");
	std::cout << "single nt file successfully written" << std::endl;
	writeCodonCSV(m_codonMatrix->getAllData(), "codonFreqs");
}

void FreqMatrix::writeCodonCSV(map<string, vector<string> > &freqMatrix, string suffix) {
	// duplicate code with writeCSV: template function?
	std::fstream fs;
	string outName = m_inputFname;
	suffix = "_" + suffix + ".csv";
	size_t pos = outName.find_last_of(".");
	// make sure the poisition is valid
	if (pos != string::npos) {
		outName = outName.substr(0, pos) + suffix;
	} else {
		outName = outName + suffix;
	}
	fs.open (outName, std::fstream::out | std::fstream::trunc);
	// store header as vector for change of order
	vector<string> header;
	for (map<string, vector<string> >::iterator it = freqMatrix.begin(); it != freqMatrix.end(); ++it) {
		string headerEntry = it->first;
		header.push_back(headerEntry);
	}

	// add data
	if (freqMatrix.find("pos") == freqMatrix.end()) {
		// matrix wasn't initialized correctly
		std::cout << "Not storing CSV '" << outName << "' because matrix wasn't initialized correctly." << std::endl;
		return;
	}
	// rearrange header: pos should be first
	header.erase(std::find(header.begin(), header.end(), "pos"));
	header.insert(header.begin(), "pos");
	// write out header:
	for (size_t i = 0; i < header.size(); ++i) {
		string headerEntry = header[i];
		if (i != header.size() - 1) {
			fs << headerEntry << ",";
		} else {
			fs << headerEntry << std::endl;
		}
	}
	size_t nbrRows = freqMatrix.at("pos").size();
	for (size_t i = 0; i < nbrRows; ++i) {
		// std::cout << "Pos: " << i << std::endl;
		for (size_t j = 0; j < header.size(); ++j) {
			string column = header[j];
			string val = freqMatrix.at(column).at(i);
			if (j != header.size() - 1) {
				fs << val << ",";
			} else {
				fs << val << std::endl;
			}
		}
	}
	fs.close();
	std::cout << "Successfully stored frequencies in CSV '" << outName << "'." << std::endl;
}
void FreqMatrix::writeCSV(map<string, vector<int> > &freqMatrix, string suffix) {
	std::fstream fs;
	string outName = m_inputFname;
	suffix = "_" + suffix + ".csv";
	size_t pos = outName.find_last_of(".");
	// make sure the poisition is valid
	if (pos != string::npos) {
		outName = outName.substr(0, pos) + suffix;
	} else {
		outName = outName + suffix;
	}
	fs.open (outName, std::fstream::out | std::fstream::trunc);
	// store header as vector for change of order
	vector<string> header;
	for (map<string, vector<int> >::iterator it = freqMatrix.begin(); it != freqMatrix.end(); ++it) {
		string headerEntry = it->first;
		header.push_back(headerEntry);
	}

	// add data
	if (freqMatrix.find("pos") == freqMatrix.end()) {
		// matrix wasn't initialized correctly
		std::cout << "Not storing CSV '" << outName << "' because matrix wasn't initialized correctly." << std::endl;
		return;
	}
	// rearrange header: pos should be first
	header.erase(std::find(header.begin(), header.end(), "pos"));
	header.insert(header.begin(), "pos");
	// write out header:
	for (size_t i = 0; i < header.size(); ++i) {
		string headerEntry = header[i];
		if (i != header.size() - 1) {
			fs << headerEntry << ",";
		} else {
			fs << headerEntry << std::endl;
		}
	}
	size_t nbrRows = freqMatrix.at("pos").size();
	for (size_t i = 0; i < nbrRows; ++i) {
		// std::cout << "Pos: " << i << std::endl;
		for (size_t j = 0; j < header.size(); ++j) {
			string column = header[j];
			int val = freqMatrix.at(column).at(i);
			if (j != header.size() - 1) {
				fs << to_string(val) << ",";
			} else {
				fs << to_string(val) << std::endl;
			}
		}
	}
	fs.close();
	std::cout << "Successfully stored frequencies in CSV '" << outName << "'." << std::endl;
}
void FreqMatrix::storeRead(vector<ReadData> &matches) {
	//std::cout << "Storing read at pos " << pos << ": " << seq << std::endl;
	// iterate through read and store values
		if (matches.size() == 0) { // nothing to store
		return;
	}
	int idx = matches.at(0).getRefPos();
	if (idx < m_minPos) {
		m_minPos = idx;
	}
	for (size_t i = 0; i < matches.size(); ++i) {
		string obs = matches.at(i).getNt();
		idx = matches.at(i).getRefPos();
		int phredQual = matches.at(i).getQuality();
		if (phredQual >= 30) {
			addEvidence(idx, obs);
		}
	}
	// update the largest insertion index to truncate the freqMatrix later
	if (idx > m_maxPos) {
		m_maxPos = idx;
	}
	//std::cout << "storeRead done" << std::endl;
}

void FreqMatrix::setCodonMatrix() {
	int readingFrame = findReadingFrame();
	m_codonMatrix = &m_codonFrames[readingFrame];
	truncateCodonMatrix();
}

string FreqMatrix::getConsensus(CodonData& codonData) {
	// form majority consensus: select max prevalence per row
	map<string, vector<int> > freqMatrix = codonData.getMatches();
	std::cout << "getConsensus" << std::endl;
	string consensus = "";
	unordered_set<string> ignoreColumns({"pos", "other"});
	for (size_t i = 0; i < m_defaultNumberOfRows; ++i) {
		//std::cout << "pos: " << i << std::endl;
		string maxObs = "";
		int maxCount = 0;
		for (map<string, vector<int> >::iterator it = freqMatrix.begin(); it != freqMatrix.end(); ++it) {
			string obs = it->first;
			if (ignoreColumns.find(obs) != ignoreColumns.end()) {
				// column doesn't relate to coverage -> ignore
				continue;
			}
			int count = it->second.at(i);
			if (count > maxCount) {
				maxObs = obs;
				maxCount = count;
			}
		}
		consensus += maxObs;
	}
	return consensus;
	
}

void FreqMatrix::storeInsertions(vector<ReadData> &insertions) {
	// store insertions in the codon files
	if (insertions.size() == 0) { // nothing to store
		return;
	}
	//std::cout << "store insertions: " << insertions.size() << std::endl;
	for (int readingFrame = 0; readingFrame < 3; ++readingFrame) {
		map<int, vector<string> > codonMap = getCodons(insertions, readingFrame); // TODO: getCodons should return a vector of strings per position
		for (map<int, vector<string> >::iterator it = codonMap.begin(); it != codonMap.end(); ++it) {
			addEvidenceCodonInsertion(it->first, it->second, readingFrame);
		}
	}
}



int FreqMatrix::findReadingFrame() {
	//std::cout << "findReadingFrame" << std::endl;
	vector<int> nbrStopCodons(m_codonFrames.size(), 0);
	int minNbrStopCodons = 100000;
	int readingFrame = 0;
	for (size_t i = 0; i < m_codonFrames.size(); ++i) {
		string consensus = getConsensus(m_codonFrames[i]);
		string aaSeq = translate(consensus);
		/*
		std::cout << "Reading frame: " << i << ", nt seq is: " << std::endl << consensus << std::endl << std::endl << aaSeq << std::endl;
		std::cout << std::endl;
		 */
		for (char &c : aaSeq) {
			if (c == '*') {
				nbrStopCodons[i]++;
				
			}
		}
		if (nbrStopCodons[i] < minNbrStopCodons) {
			minNbrStopCodons = nbrStopCodons[i];
			readingFrame = i;
		}
	}
	// std::cout << "Selected reading frame: " << readingFrame << std::endl;
	return readingFrame;
}

map<int, vector<string> > FreqMatrix::getCodons(vector<ReadData> &matches, int readingFrame) {
	map<int, vector<string> > codonMap; // map from refPos to codons (vector: multiple insertions possible)
	if (matches.size() == 0) {
		return codonMap;
	}
	int i = 0;
	while (i < ((int)matches.size()) - 2) {
		int curPos = matches[i].getRefPos();
		int posReadingFrame = (curPos - 1) % 3;
		if (readingFrame != posReadingFrame) {
			// don't start a new codon from an incorrect reading frame
			++i;
			continue;
		}
		string codon = matches.at(i).getNt() + matches.at(i+1).getNt() + matches.at(i+2).getNt();
		vector<int> phredQuals = {matches.at(i).getQuality(), matches.at(i+1).getQuality(), matches.at(i+2).getQuality()};
		// check qualities
		bool qualOK = true;
		for (int qual: phredQuals) {
			if (qual < 30) {
				qualOK = false;
			}
		}
		// check positions
		vector<int> posis = {matches.at(i).getRefPos(), matches.at(i+1).getRefPos(), matches.at(i+2).getRefPos()};
		bool posOK = true;
		for (size_t j = 1; j < posis.size(); ++j) {
			int diff = posis[j] - posis[j-1];
			if (diff > 1) {
				// nonconsecutive bases or not the same pos (diff = 0 for multiple insertions at a pos)
				posOK = false;
			}
		}
		if (qualOK && posOK && codon.length() == 3) {
			int codonPos = ((curPos - 1) / 3) + 1;
			codonMap[codonPos].push_back(codon);
		}
		// adjust iteration
		if (posOK) {
			// 3 nucleotides were in the reading frame
			i += 3;
		} else {
			// there's a break in the reading frame somewhere, check the next base for right reading frame
			i += 1;
		}
	}
	return codonMap;
}

void FreqMatrix::storeReadCodon(vector<ReadData> &matches) {
	//std::cout << "Storing read at pos " << pos << ": " << seq << std::endl;
	// iterate through read and store values
	
	if (matches.size() == 0) { // nothing to store
		return;
	}
	for (int readingFrame = 0; readingFrame < 3; ++readingFrame) {
		//std::cout << "readingFrame: " << readingFrame << std::endl;
		map<int, vector<string> > codonMap = getCodons(matches, readingFrame); // TODO: fixme for insertions/deletions
		for (map<int, vector<string> >::iterator it = codonMap.begin(); it != codonMap.end(); ++it) {
			if (it->second.size() != 1) { // matches should only have 1 value per pos
				throw std::invalid_argument("Match had more than 1 observation per position!");
			} else {
				addEvidenceCodon(it->first, it->second[0], readingFrame);
			}
		}
	}
}

/*
void FreqMatrix::storeReadCodon(vector<string> &seq, vector<int>& pos, vector<int> &baseQuals) {
	//std::cout << "Storing read at pos " << pos << ": " << seq << std::endl;
	// iterate through read and store values
	
	// TODO: truncation pair for codon freq should be different due to storing insertions in the file!
	if (pos.size() == 0) { // nothing to store
		return;
	}
	vector<string> codons = {"", "", ""};
	int lastCodonPos = pos[0];
	for (size_t i = 0; i < seq.size(); ++i) {
		string obs = seq.at(i);
		int curPos = pos[i];
		int phredQual = baseQuals.at(i);
		if (phredQual >= 30) {
			//addEvidence(idx, obs);
			for (size_t j = 0; j < codons.size(); ++j) {
				if (codons[j].length() == 0) {
					// start a new codon if current pos fits the reading frame j
					int readingFrame = (curPos - 1) % 3;
					if (readingFrame == j) { // correct reading frame for current position in the reference
						codons[j] = obs;
					}
				} else if (codons[j].length() != 0 && curPos - lastCodonPos == 1) { // ensure that bases are consecutive
					// extend the current codon
					codons[j] = codons[j] + obs;
				} else if (codons[j].length() != 0 && curPos - lastCodonPos != 1) {
					// reset codon
					codons[j] = ""; 
				}
				if (codons[j].length() == 3) { // codon was constructed
					// store codon evidence
					int codonPos = ((curPos - 1) / 3) + 1;
					//std::cout << "codon " << codons[j] << " at pos "<< codonPos << was constructed!" << std::endl;
					addEvidenceCodon(codonPos, codons[j], j);
					// clear codon
					codons[j] = "";
				}
			}
			lastCodonPos = curPos;
		}
		// std::cout << "codon 0: " << codons[0] << std::endl;
	}
}
*/
