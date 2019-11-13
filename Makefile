#Paths
OUTPUT = ./out/
MAIN = ./src/
SERVER = ./src/server/
CLIENT = ./src/client/
JWT = ./libs/jwt-cpp/
NLOHMANN_JSON = ./libs/json/
CXXOPTS = ./libs/cxxopts/
SPDLOG = ./libs/spdlog/
THRIFT_GENERATED = $(MAIN)/gen-cpp/
RESOURCES = ./res/

#Configuration
CPP  = g++
THRIFT = thrift
THRIFTFILE = SecurityProvider.thrift
CPPFLAGS = -Wall -Wformat -Os -std=c++17

#Sourcecode and flags
SERVER_EXE = server
SERVER_SOURCES = $(SERVER)/Server.cpp $(SERVER)/SecurityProvider.cpp
SERVER_OBJS = $(addsuffix .o, $(basename $(SERVER_SOURCES)))
SERVER_CPP = $(THRIFT_CPP) -I$(SPDLOG) -I$(JWT) -I$(CXXOPTS)
SERVER_LDFLAGS = -lpthread $(THRIFT_LDFLAGS)

CLIENT_EXE = client
CLIENT_SOURCES = $(CLIENT)/Client.cpp
CLIENT_OBJS = $(addsuffix .o, $(basename $(CLIENT_SOURCES)))
CLIENT_CPP = $(THRIFT_CPP) -I$(NLOHMANN_JSON)/ -I$(SPDLOG) -I$(JWT)  -I$(CXXOPTS)
CLIENT_LDFLAGS = $(THRIFT_LDFLAGS)

THRIFT_SOURCES = $(THRIFT_GENERATED)/$(basename $(THRIFTFILE)).cpp
THRIFT_SOURCES += $(THRIFT_GENERATED)/$(basename $(THRIFTFILE))_constants.cpp
THRIFT_SOURCES += $(THRIFT_GENERATED)/$(basename $(THRIFTFILE))_types.cpp
THRIFT_OBJS = $(addsuffix .o, $(basename $(THRIFT_SOURCES)))
THRIFT_CPP = -I$(THRIFT_GENERATED)/ -I/usr/local/include/thrift
THRIFT_LDFLAGS = -L/usr/local/lib -lthrift

#Build rules
all: server client

$(SERVER_OBJS): %.o : %.cpp
	$(CPP) $(CPPFLAGS) $(SERVER_CPP) -c -o $@ $<
	
$(CLIENT_OBJS): %.o : %.cpp
	$(CPP) $(CPPFLAGS) $(CLIENT_CPP) -c -o $@ $<
	
$(THRIFT_OBJS): %.o : %.cpp
	$(CPP) $(CPPFLAGS) $(THRIFT_CPP) -c -o $@ $<

$(SERVER_EXE): $(OUTPUT)/$(SERVER_EXE)

$(CLIENT_EXE): $(OUTPUT)/$(CLIENT_EXE)

$(OUTPUT)/$(SERVER_EXE): $(SERVER_OBJS) $(THRIFT_OBJS)
	mkdir -p $(OUTPUT)
	$(CXX) -o $@ $^ $(SERVER_LDFLAGS)
	
$(OUTPUT)/$(CLIENT_EXE): $(CLIENT_OBJS)  $(THRIFT_OBJS)
	mkdir -p $(OUTPUT)
	$(CXX) -o $@ $^ $(CLIENT_LDFLAGS)
	
clean:
	rm -f $(SERVER_OBJS) $(CLIENT_OBJS) $(THRIFT_OBJS)
	
distclean: clean
	rm -rf $(OUTPUT)
	
thrift:
	rm -f $(THRIFT_GENERATED)/*
	mkdir -p $(THRIFT_GENERATED)
	$(THRIFT) --gen cpp -out $(THRIFT_GENERATED) $(RESOURCES)/$(THRIFTFILE)
	
doku: 
	- doxygen Doxyfile

format: $(addprefix format-, $(wildcard $(MAIN)/*.*pp) $(wildcard $(MAIN)/*/*.*pp)))

format-$(MAIN)%.cpp: $(MAIN)%.cpp
	clang-format $< -style="{BasedOnStyle: webkit, IndentWidth: 4, TabWidth: 4, UseTab: ForContinuationAndIndentation}" > $<_2
	mv $<_2 $<
	rm -f $<_2

format-$(MAIN)%.hpp: $(MAIN)%.hpp
	clang-format $< -style="{BasedOnStyle: webkit, IndentWidth: 4, TabWidth: 4, UseTab: ForContinuationAndIndentation}" > $<_2
	mv $<_2 $<
	rm -f $<_2

format-$(MAIN)%.h: $(MAIN)%.h
	clang-format $< -style="{BasedOnStyle: webkit, IndentWidth: 4, TabWidth: 4, UseTab: ForContinuationAndIndentation}" > $<_2
	mv $<_2 $<
	rm -f $<_2
