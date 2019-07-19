CXX=g++
CXXOPTIMIZE= -O0
CXXFLAGS= -g -Wall -pthread -std=c++11 $(CXXOPTIMIZE)
USERID=EDIT_MAKE_FILE


all: web-server-per web-client-per

web-server-per: $(SERVERCLASSES)
    $(CXX) -o $@ $^ $(CXXFLAGS) $@.cpp

web-client-per: $(CLIENTCLASSES)
    $(CXX) -o $@ $^ $(CXXFLAGS) $@.cpp

clean:
    rm -rf *.o *~ *.gch *.swp *.dSYM web-server-per web-client-per *.tar.gz

tarball: clean
    tar -cvf $(USERID).tar.gz *






