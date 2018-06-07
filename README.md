# bamToFreq
bamToFreq is a tool for converting mapped reads from next-generation sequencing (NGS) in terms of a [BAM file](https://samtools.github.io/hts-specs/SAMv1.pdf) to the [frequency file format](http://doi.org/10.1093/nar/gky349). BAM files can be generated from FASTQ files using a variety of mappers such as BWA, SMALT, or Bowtie. For mapping reads and analyzing resistance of viral such as HIV-1 from a FASTQ file, there exist freely available pipelines such as [MinVar](https://github.com/ozagordi/MinVar) or web services such as [PASeq](https://paseq.org). Other tools such as <a href = "https://ngs.geno2pheno.org">geno2pheno[ngs-freq]</a> require the input of frequency files whose generation is facilitated by the use of bamToFreq.

## Linux 64 Bit Binary
A statically-linked binary for 64 bit Linux systems [is available](bin/bamToFreq).

## Docker Container
A Docker container for bamToFreq is available at [Docker Hub](https://hub.docker.com/r/mdoering88/bamtofreq/). If you have Docker installed, you can simply run bamToFreq directly using 
```
docker run -v <bamFolder>:/data/ mdoering88/bamtofreq:latest /data/bamFile.bam

```

or via an interactive session
```
docker run -v <bamFolder>:/data/ -it --entrypoint /bin/bash mdoering88/bamtofreq:latest
```

## Requirements of bamToFreq
The only requirements for using the compiled version of bamToFreq are [htslib](https://github.com/samtools/htslib) and boost. For example, if htslib has not been properly setup, bamToFreq will not work and output the following error:
```
error while loading shared libraries: libhts.so.2: cannot open shared object file: No such file or directory
```
Therefore, please ensure that you either have installed the requirements system-wide or that you have adjusted your ```LD_LIBRARY_PATH``` accordingly, for example in case that htslib is installed in a local folder:
```
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:<pathToFolderContainingHtsLib>/lib
```
## Installation from Source
If you fulfill the requirements for bamToFreq, you can simply run
```
./configure && make && sudo make install
```
If you do not have superuser privileges, you can also specify a local folder for installation using ```./configure --prefix <path_to_local_folder>```. If ```./configure``` should fail due to libraries that are missing from the system, you can provide the paths to the local libraries in this way
```
export CPPFLAGS='-I<pathToFolderContainingHtsLib>/include' && export LDFLAGS='-L<pathToFolderContainingHtsLib>/lib'
```
and then call configure and make once again.

## Usage
Once bamToFreq has been installed, you can run the tool via
```
bamToFreq <bamFile>
```
Of course you can also pass multiple BAM files to the program. Once the computations have completed, the frequency files are automatically generated in terms of CSVs whose filenames are generated from ```<bamFile>``` by appending ```_freqs.csv``` or ```_codonFreqs.csv``` for single-nucleotide or codon frequency files, respectively. Please see the [examples folder](examples) for an example providing input and output for a HIV-1 sample of the polymerase region. Please note that the runtime of bamToFreq largely depends on the number of reads. If high coverage is not necessary, samtools can be used to sample reads from a BAM file.

For an overview of available options, please use
```
bamToFreq -h
```

The Phred-scaled quality score that is used to select reads/nucleotides that are considered for frequency file generation, is determined via the ```-q``` argument. The generation of codon frequency files can be disabled with the ```-c``` argument. A different output folder can be specified with the ```-o``` argument.

## License
This code is subject to the terms of the Mozilla Public License, v. 2.0.
