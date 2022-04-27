#include <iostream>

#include <grpc++/grpc++.h>

#include "grpc/benchmark.grpc.pb.h"
#include "grpc/benchmark.pb.h"

#include "common.hpp"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

class BenchmarkClient
{
public:
    BenchmarkClient(std::shared_ptr<Channel> channel)
    : stub_(Benchmark::NewStub(channel)) {}

    void streamTest()
    {
        while(true)
        {
            StartMsg start;
            std::unique_ptr<grpc::ClientReader<DataMsg> > reader;
            ClientContext context;

            reader = stub_->Stream(&context, start);

            // read from stream
        }
    }

private:
    std::unique_ptr<Benchmark::Stub> stub_;
};

int main(int argc, char** argv)
{
    std::cout << "Client started" << std::endl;

    return 0;
}