#include "protos/stringmanipulation.grpc.pb.h"
#include "protos/stringmanipulation.pb.h"
#include <grpcpp/grpcpp.h>
#include <grpcpp/impl/codegen/client_context.h>
#include <sys/time.h>

class StringManipulationClient {
public:
    StringManipulationClient(std::shared_ptr<grpc::Channel> channel)
    : stub_(stringmanipulation::StringManipulation::NewStub(channel)) {
         
    }
    std::string reverseString(std::string input) {
        stringmanipulation::StringRequest request;
        stringmanipulation::StringReply reply;
        grpc::ClientContext context;
        
        struct timeval tv;
        gettimeofday(&tv, NULL);

        auto *im = new stringmanipulation::innerMessage();
        im->set_some_val(123);
        im->set_some_string("some string");

        auto *cmess = new stringmanipulation::complexMessage();
        cmess->set_innerenum(stringmanipulation::testEnumeration::FOUR);
        cmess->set_allocated_some_message(im);

        request.set_allocated_cm(cmess);

        request.set_original(input);
        request.set_timestamp(tv.tv_sec);

        request.set_testval(true);
        request.set_testval2(true);
        request.set_testval3(true);

        grpc::Status status = stub_->reverseString(&context, request, &reply);

        if (status.ok()) { return reply.result(); }
        else { return status.error_message(); }
    }

    std::string uppercaseString(std::string input) {
        stringmanipulation::StringRequest request;
        stringmanipulation::StringReply reply;
        grpc::ClientContext context;
        
        request.set_original(input);
        grpc::Status status = stub_->uppercaseString(&context, request, &reply);
        if (status.ok()) return reply.result();
        else return status.error_message();
    }

private:
    std::unique_ptr<stringmanipulation::StringManipulation::Stub> stub_;
};

void RunClient(std::string input_str) {
    std::string connect_addr = "127.0.0.1:50006";
    StringManipulationClient client(grpc::CreateChannel(connect_addr, grpc::InsecureChannelCredentials()));
    std::string response;

    response = client.uppercaseString(input_str);
    std::cout << "Original: " << input_str << "\n";
    std::cout << "Response: " << response << "\n";

    response = client.reverseString(input_str);
    std::cout << "Original: " << input_str << "\n";
    std::cout << "Response: " << response << "\n";

}

int main (int argc, char **argv) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <string to reverse>\n";
        return -1;
    }
    std::string input = argv[1];
    RunClient(input);
    return 0;
}
