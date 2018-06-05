FROM frolvlad/alpine-gxx:latest
MAINTAINER Matthias Doering <mdoering@mpi-inf.mpg.de>
# Install dependencies for htslib and bamToFreq
RUN apk --update add git make zlib-dev bzip2-dev xz-dev boost-dev boost-filesystem boost-system automake autoconf && \
    git clone git://github.com/samtools/htslib.git && cd htslib && make && make install 
ADD . bamToFreq
WORKDIR bamToFreq
# Install bamToFreq: manual change of CXXFLAGS to change order of boost inclusion: problem with alpine otherwise
RUN ./configure && make && make install 
#RUN pip install selenium
#WORKDIR /srv/
#RUN mkdir primer_design
#ADD . primer_design
## install openPrimeR package to install all the dependencies of the tool
#RUN Rscript primer_design/src/install.R FALSE TRUE
##update shiny server conf and configure it to run the primer design tool in single app mode
#ADD shiny-server.conf /etc/shiny-server/shiny-server.conf
#COPY .docker_bashrc /home/shiny/.bashrc
#WORKDIR /srv/primer_design
