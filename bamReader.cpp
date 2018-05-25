#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <htslib/sam.h>
#include <iostream>
#include "bamReader.h"
#include "FreqMatrix.h"
#include <stdlib.h>
#include "ReadData.h"

int main(int argc, char *argv[]){
    char* samPath = argv[1];
	processSamFile(samPath);
    return 0;
}
int readFromSam(samFile *fp_in, bam_hdr_t *bamHdr, bam1_t *aln, char* samPath) {
	try {
		return sam_read1(fp_in,bamHdr,aln);
	}
	catch (...) {
		std::cerr << "File '" << samPath << "' does not seem to be supported. Are you sure it's a BAM file?" << std::endl;
		return 0;
	}
}
void processSamFile(char* samPath) {
	std::cout << "Generating frequencies from file '" << samPath << "' ..."  << std::endl;
	samFile *fp_in = hts_open(samPath,"r"); //open bam file
	bam_hdr_t *bamHdr = sam_hdr_read(fp_in); //read header
    bam1_t *aln = bam_init1(); //initialize an alignment
	if (fp_in == NULL || bamHdr == NULL || aln == NULL) {
		return;
	}
	// init matrix with nucleotide counts
	FreqMatrix *freqs = new FreqMatrix(samPath);
	uint8_t qualCutoff = 30;
	int readCount = 0;
    while(readFromSam(fp_in, bamHdr, aln, samPath) > 0){
		readCount++;
        int32_t pos = aln->core.pos + 1; // left most position of alignment in zero based coordinate (+1)
        //char *chr = bamHdr->target_name[aln->core.tid] ; // contig name (chromosome)
        uint32_t len = aln->core.l_qseq; // length of the read.
		uint32_t readQuality = aln->core.qual ; //mapping quality
		if (readQuality < qualCutoff) {
			// ignore reads whose quality is not sufficient
			continue;
		}
		uint8_t* baseQuals = bam_get_qual(aln);
        uint8_t *q = bam_get_seq(aln); // read sequence
        uint32_t *cigar = bam_get_cigar(aln); // lower 4 bits indicate cigar state, upper 28 bits give the cigar length		
		// interpret cigar string
		vector<ReadData> matches;
		vector<ReadData> insertions;
		vector<ReadData> deletions;
		//map<int, string> insertions;
		//map<int, string> deletions;
		int refPos = pos;
		int readIdx = 0;
		//std::cout << "cigar len: " << aln->core.n_cigar << std::endl;
		for (int k = 0; k < aln->core.n_cigar; ++k) {
			//std::cout << "k: " << k << ", refPos: " << refPos << ", readIdx: " << readIdx << std::endl;
			int op  = bam_cigar_op(cigar[k]);
			bool isInsertion = op == BAM_CINS; // macros defined in sam.h of htslib
			bool isDeletion = op == BAM_CDEL;
			int len = bam_cigar_oplen(cigar[k]);
			// std::cout << "op: " << op << " binary type: " << bam_cigar_type(op) << ", len: " << len<< std::endl;
			if (bam_cigar_type(op) == 1) { // bitwise operation to check whether query is consumed (e.g. insertion in query))
				if (isInsertion) {
					// store insertion for codon file
					// std::cout << "isInsertion" << std::endl;
					for (int i = 0; i < len; ++i) { 
						ReadData rd = ReadData(string(1, seq_nt16_str[bam_seqi(q,readIdx)]), refPos, (int)baseQuals[readIdx]);
						insertions.push_back(rd);
						readIdx++;
					}
				} else {
					readIdx += len;
				}
			} else if (bam_cigar_type(op) == 2) { // bitwise operation to check whether ref is consumed (e.g. deletion in query)
				//std:cout << "Consume reference" << std::endl;
				if (isDeletion) {
					for (int i = 0; i < len; ++i) {
						ReadData rd = ReadData("-", refPos, (int)baseQuals[readIdx]);
						deletions.push_back(rd);
						refPos++;
					}
				} else {
					refPos += len;
				}
			} else if (bam_cigar_type(op) == 3) { // both are consumed -> store values readSeq
				//std::cout << "Consume both" << std::endl;
				for (int i = 0; i < len; ++i) {
					ReadData rd = ReadData(string(1, seq_nt16_str[bam_seqi(q,readIdx)]), refPos, (int)baseQuals[readIdx]);
					matches.push_back(rd);
					refPos++;
					readIdx++;
				}
			} // otherwise: nothing is consumed (e.g. hard clipping)
		}
		// output results:
		/*
		std::cout << "deletions: " << std::endl;
		for (size_t i = 0; i < deletions.size(); ++i) {
			std::cout << deletions.at(i).getNt();
		}
		std::cout << std::endl;
		 */
		freqs->storeRead(matches);
		freqs->storeReadCodon(matches);
		// TODO: range check errors for both functions
		freqs->storeInsertions(insertions); // store separately to not mess up vector pos indexing
		//freqs->storeReadCodon(deletions); // store in the codon matrix via "---" column

    }
	if (readCount != 0) {
		// any reads present?
		freqs->truncateFreqMatrix(); // truncate unnecessary rows in matrix
		freqs->setCodonMatrix();
		freqs->writeCSVs();
	}
    bam_destroy1(aln);
    sam_close(fp_in);
}