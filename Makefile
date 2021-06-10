CC = gcc
CXX = g++
RM = rm -f

CXXFLAGS = -Ofast
INCLUDE = -I.
LIB = -lprotobuf -lgrpc++
PROTOC = /usr/bin/protoc
SERVER_SOURCES = $(wildcard server_src/*.cpp)
CLIENT_SOURCES = $(wildcard client_src/*.cpp)
PROTO_SOURCES = $(wildcard protos/*.proto)

SERVER_OBJ=$(SERVER_SOURCES:.cpp=.o)
CLIENT_OBJ=$(CLIENT_SOURCES:.cpp=.o)
PROTO_OBJECT = $(patsubst protos/%.proto,protos/%.pb.o,$(PROTO_SOURCES))
GRPC_OBJECT = $(patsubst protos/%.proto,protos/%.grpc.pb.o,$(PROTO_SOURCES))

all: client server

# Define the rule for generating proto's cpp sources
protos/%.grpc.pb.cc: protos/%.proto
	$(PROTOC) --plugin=protoc-gen-grpc=/usr/bin/grpc_cpp_plugin --grpc_out=. $<

# Define the rule for generating proto's cpp sources
protos/%.pb.cc: protos/%.proto
	$(PROTOC) --cpp_out=. $<

# Define the rule for compiling generated proto grpc binding cpp sources
protos/%.pb.o: protos/%.pb.cc
	$(CXX) $(INCLUDE) -c $< -o $@

# Define the rule for compiling other cpp sources
%.o: %.cpp
	$(CXX) $(INCLUDE) -c $< -o $@

# Define the rule for linking the final executable
server: $(PROTO_OBJECT) $(GRPC_OBJECT) $(SERVER_OBJ)
	$(CXX) $(INCLUDE) -o $@ $^ ${LIB}

# Define the rule for linking the final executable
client: $(PROTO_OBJECT) $(GRPC_OBJECT) $(CLIENT_OBJ)
	$(CXX) $(INCLUDE) -o $@ $^ ${LIB}

clean:
	${RM} $(CLIENT_OBJ) $(SERVER_OBJ) $(PROTO_OBJECT) server client protos/*.pb.{cc,o} protos/*.grpc.pb.{cc,o}

.PHONY: all clean
