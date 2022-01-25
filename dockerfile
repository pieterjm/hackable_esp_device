FROM hrektts/doxygen
COPY .\ /data
RUN ["doxygen" ,"/data/Dockerdoxygen.config"]
