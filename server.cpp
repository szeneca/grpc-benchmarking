#include <iostream>

#include <grpc++/grpc++.h>

#include "grpc/benchmark.grpc.pb.h"
#include "grpc/benchmark.pb.h"

#include "common.hpp"

using grpc::Channel;
using grpc::ServerContext;
using grpc::Status;

class BenchmarkServiceImpl final : public Benchmark::Service 
{
public:
    Status Stream(ServerContext* context, const StartMsg*, grpc::ServerWriter<DataMsg>* writer)
    {
        while(true)
        {
               
        }

        return Status::OK;
    }

private:
};

int main(int argc, char** argv)
{
    std::cout << "Server started" << std::endl;
    
    return 0;
}