FROM hrektts/doxygen
COPY .\ /data
RUN ["doxygen" ,"/data/newdoxygen.config"]
