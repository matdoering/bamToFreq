# bamToFreq
bamToFreq is a tool for converting mapped reads from next-generation sequencing (NGS) in terms of a [BAM file](https://samtools.github.io/hts-specs/SAMv1.pdf) to the [frequency file format](http://doi.org/10.1093/nar/gky349). BAM files can be generated from FASTQ files using a variety of mappers such as BWA, SMALT, or Bowtie. For mapping reads and analyzing resistance of of viral such as HIV-1 from a FASTQ file, there exist freely available pipelines such as [MinVar](https://github.com/ozagordi/MinVar) or web services such as [PASeq](https://paseq.org). Other tools such as <a href = "https://ngs.geno2pheno.org">geno2pheno[ngs-freq]</a> require the input of frequency files, whose generation is facilitated by the use of bamToFreq.

## Requirements
The only requirement of bamToFreq is [htslib](https://github.com/samtools/htslib). If htslib has not been properly setup, bamToFreq will not work and output the following error:
```
error while loading shared libraries: libhts.so.2: cannot open shared object file: No such file or directory
```
Therefore, please ensure that you either have installed htslib system-wide or that you have adjusted your ```LD_LIBRARY_PATH``` accordingly, for example:
```
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:<pathToBamToFreq>/htslib/lib/
```

## Usage
You can simply run bamToFreq via
```
bamToFreq <bamFile>
```
Once the computations have completed, the frequency files are automatically generated in terms of CSVs whose filenames are generated from ```<bamFile>``` by appending ```_freqs.csv``` or ```_codonFreqs.csv``` for single-nucleotide or codon frequency files, respectively.
