#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <htslib/sam.h> /// linker: libhts
#include <iostream>
#include "bamReader.h"
#include "FreqMatrix.h"
#include <stdlib.h>
#include "ReadData.h"
#include <unistd.h>
#include <stdio.h>
#include <boost/filesystem.hpp> // linker: -lboost_filesystem -lboost_system

struct globalArgs_t {
    int phredQualCutoff;        /* -q option for phred quality cutoff*/
    string outFolder;           /* -o option for outFolder */
    bool buildCodonFile;      /* -c option to prevent codon file from being built*/
    char** inputFiles; /* array of input BAM files */
    int numInputFiles; /* number of input files in 'inputFiles' array */
} globalArgs;


void printUsage(char *argv[]) {
    fprintf(stderr, "Usage: %s [-q phredQualCutoff] [-o outFolder] [-c] file.bam\n", argv[0]);
}
int main(int argc, char *argv[]){
    if (argc == 1) {
        printUsage(argv);
        return 1;
    }
    

    // q: requires additional argument, o: as well, c,h,?: no args
    static const char *optString = "q:o:ch?"; // determine which options are available (column indicates there's an argument)
    int opt = 0;
     
    /* Initialize globalArgs before we get to work. */
    globalArgs.phredQualCutoff = 30;
    globalArgs.outFolder = "";
    globalArgs.buildCodonFile = true;
    globalArgs.inputFiles = argv + 1; // first non-program-name argument
    globalArgs.numInputFiles = argc - 1; // expect that all args except the program name are bam files
    
    // global vars from getopt: optarg (argument supplied), optind (index of next argv pointer), optopt (last known option)
    while ((opt = getopt (argc, argv, optString)) != -1) {
    switch (opt) {
      case 'q':
        globalArgs.phredQualCutoff = atoi(optarg);
        break;
      case 'o': {
        globalArgs.outFolder = optarg;
        // create folder if it doesn't exist yet
        boost::filesystem::path dir(optarg);
        boost::filesystem::create_directory(dir);
        break;
      }
      case 'c':
        globalArgs.buildCodonFile = false;
        break;
      case '?':
        printUsage(argv);
        return 1;
      default:
        abort ();
      }
      /* count the number of input files that are remaining */
      globalArgs.inputFiles = argv + optind;
      globalArgs.numInputFiles = argc - optind;
    }
    for (int i = 0; i < globalArgs.numInputFiles; ++i) {
        char* samPath = globalArgs.inputFiles[i];
        std::cout << "Generating frequencies from file " << i+1 << "/" << globalArgs.numInputFiles << ": '" << samPath << "' ..."  << std::endl;
        processSamFile(samPath); 
    }
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
	samFile *fp_in = hts_open(samPath,"r"); //open bam file
	bam_hdr_t *bamHdr = sam_hdr_read(fp_in); //read header
    bam1_t *aln = bam_init1(); //initialize an alignment
	if (fp_in == NULL || bamHdr == NULL || aln == NULL) {
		return;
	}
	// init matrix with nucleotide counts
	FreqMatrix *freqs = new FreqMatrix(samPath, globalArgs.phredQualCutoff, globalArgs.outFolder);
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
        if (globalArgs.buildCodonFile) {
            freqs->storeReadCodon(matches);
            // TODO: range check errors for both functions
            freqs->storeInsertions(insertions); // store separately to not mess up vector pos indexing
            freqs->storeReadCodon(deletions); // store in the codon matrix via "---" column
        }
    }
	if (readCount != 0) {
		// any reads present?
		freqs->truncateFreqMatrix(); // truncate unnecessary rows in matrix
        if (globalArgs.buildCodonFile) {
            freqs->setCodonMatrix();
        }
		freqs->writeCSVs();
	}
    bam_destroy1(aln);
    sam_close(fp_in);
}