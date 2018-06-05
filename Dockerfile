FROM frolvlad/alpine-gxx:latest
MAINTAINER Matthias Doering <mdoering@mpi-inf.mpg.de>
# Install dependencies for htslib and bamToFreq
RUN apk --update add git make zlib-dev bzip2-dev xz-dev boost-dev boost-filesystem boost-system automake autoconf bash && \
    git clone git://github.com/samtools/htslib.git && cd htslib && make && make install 
ADD . bamToFreq
WORKDIR bamToFreq
# Install bamToFreq: manual change of CXXFLAGS to change order of boost inclusion: problem with alpine otherwise
RUN ./configure && make && make install 
ENTRYPOINT ["bamToFreq"]
CMD []
