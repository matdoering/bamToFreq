FROM frolvlad/alpine-gxx:latest
MAINTAINER Matthias Doering <mdoering@mpi-inf.mpg.de>
RUN rm -rf /var/lib/apt/lists/*
RUN apt-get update && apt-get install -y libfontconfig1 libxml2-dev default-jre libssl-dev/unstable libv8-dev libgslcblas0 python-pip pandoc texlive texlive-latex-extra libmysqlclient-dev
#    && rm -rf /var/lib/apt/lists/*
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
