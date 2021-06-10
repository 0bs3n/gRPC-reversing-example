#include "protos/stringmanipulation.grpc.pb.h"
#include <grpcpp/grpcpp.h>

#include <stdio.h>
#include <sys/time.h>

using grpc::Status;
using grpc::ServerContext;
using grpc::ServerBuilder;

using stringmanipulation::StringRequest;
using stringmanipulation::StringReply;

class StringManipulationImpl : public stringmanipulation::StringManipulation::Service {
    Status reverseString(ServerContext *context, 
                         const StringRequest *request, 
                         StringReply *reply) {


        std::string original = request->original();
        std::string working_copy = original;
        size_t sent_at = request->timestamp();

        bool testval = request->testval();
        bool testval2 = request->testval2();
        bool testval3 = request->testval3();
        printf("%d\n", testval);
        printf("%d\n", testval2);
        printf("%d\n", testval3);

        std::reverse(working_copy.begin(), working_copy.end());

        stringmanipulation::complexMessage *cmess = new stringmanipulation::complexMessage(request->cm());
        printf("%d\n", cmess->some_message().some_val());

        struct timeval tv;
        gettimeofday(&tv, NULL);
        reply->set_result(working_copy);
        reply->set_allocated_cm(cmess);
        reply->set_timestamp(tv.tv_sec);

        printf("[%ld|%s] reverseString(\"%s\")@%lu -> \"%s\"\n", 
                tv.tv_sec, 
                context->peer().c_str(), 
                request->original().c_str(), 
                sent_at,
                working_copy.c_str());

        return Status::OK;
    }

    Status uppercaseString(ServerContext *context, 
                           const StringRequest *request, 
                           StringReply *reply) {

        std::string working_copy = request->original();
        for (auto &c: working_copy) c = toupper(c);
        reply->set_result(working_copy.c_str());

        struct timeval tv;
        gettimeofday(&tv, NULL);

        printf("[%ld|%s] uppercaseString(\"%s\") -> \"%s\"\n", 
                tv.tv_sec, 
                context->peer().c_str(), 
                request->original().c_str(), 
                working_copy.c_str());

        return Status::OK;

    }
};

void RunServer() {
    std::string listen = "127.0.0.1:50006";
    StringManipulationImpl service;
    ServerBuilder builder;

    builder.AddListeningPort(listen, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on port: " << listen << "\n";
    server->Wait();
}


int
main() {
    RunServer();
    return 1;
}
