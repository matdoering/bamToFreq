# bamToFreq
bamToFreq is a tool for converting mapped reads from next-generation sequencing (NGS) in terms of a [BAM file](https://samtools.github.io/hts-specs/SAMv1.pdf) to the [frequency file format](http://doi.org/10.1093/nar/gky349). BAM files can be generated from FASTQ files using a variety of mappers such as BWA, SMALT, or Bowtie. For mapping reads and analyzing resistance of of viral such as HIV-1 from a FASTQ file, there exist freely available pipelines such as [MinVar](https://github.com/ozagordi/MinVar) or web services such as [PASeq](https://paseq.org). Other tools such as <a href = "https://ngs.geno2pheno.org">geno2pheno[ngs-freq]</a> require the input of frequency files, whose generation is facilitated by the use of bamToFreq.

## Requirements
The only requirements of bamToFreq are [htslib](https://github.com/samtools/htslib) and boost. For example, if htslib has not been properly setup, bamToFreq will not work and output the following error:
```
error while loading shared libraries: libhts.so.2: cannot open shared object file: No such file or directory
```
Therefore, please ensure that you either have installed the requirements system-wide or that you have adjusted your ```LD_LIBRARY_PATH``` accordingly, for example in case that htslib is installed locally:
```
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:<pathToFolderContainingLibHts>
```

## Usage
You can simply run bamToFreq via
```
bamToFreq <bamFile>
```
Of course you can also pass multiple BAM files to the program. Once the computations have completed, the frequency files are automatically generated in terms of CSVs whose filenames are generated from ```<bamFile>``` by appending ```_freqs.csv``` or ```_codonFreqs.csv``` for single-nucleotide or codon frequency files, respectively. Please see the [examples folder](examples) for an example providing input and output for a HIV-1 sample of the polymerase region. Please note that the runtime of bamToFreq largely depends on the number of reads. If high coverage is not necessary, samtools can be used to sample reads from a BAM file.

For an overview of available options, please use
```
bamToFreq -h
```

The Phred-scaled quality score that is used to select reads/nucleotides that are considered for frequency file generation, is determined via the ```-q``` argument. The generation of codon frequency files can be disabled with the ```-c``` argument. A different output folder can be specified with the ```-o``` argument.

