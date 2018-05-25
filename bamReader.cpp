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

void processSamFile(char* samPath) {
	std::cout << "processSamFile" << std::endl;
	samFile *fp_in = hts_open(samPath,"r"); //open bam file
	bam_hdr_t *bamHdr = sam_hdr_read(fp_in); //read header
	std::cout << "before init" << std::endl;
    bam1_t *aln = bam_init1(); //initialize an alignment
	if (fp_in == NULL || bamHdr == NULL || aln == NULL) {
		return;
	}
	std::cout << "after init" << std::endl;
	// init matrix with nucleotide counts
	FreqMatrix *freqs = new FreqMatrix(samPath);
	uint8_t qualCutoff = 30;
    while(sam_read1(fp_in,bamHdr,aln) > 0){
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
		/*bam_cigar_type returns a bit flag with:
 *   bit 1 set if the cigar operation consumes the query
 *   bit 2 set if the cigar operation consumes the reference
 *
 * For reference, the unobfuscated truth table for this function is:
 * BAM_CIGAR_TYPE  QUERY  REFERENCE
 * --------------------------------
 * BAM_CMATCH      1      1
 * BAM_CINS        1      0
 * BAM_CDEL        0      1
 * BAM_CREF_SKIP   0      1
 * BAM_CSOFT_CLIP  1      0
 * BAM_CHARD_CLIP  0      0
 * BAM_CPAD        0      0
 * BAM_CEQUAL      1      1
 * BAM_CDIFF       1      1
 * BAM_CBACK       0      0
 * --------------------------------
 */
		
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
	freqs->truncateFreqMatrix(); // truncate unnecessary rows in matrix
    freqs->setCodonMatrix();
    bam_destroy1(aln);
    sam_close(fp_in);
	freqs->writeCSVs();
}