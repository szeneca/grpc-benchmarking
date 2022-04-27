#include <iostream>
#include <chrono>
#include <array>
#include <numeric>

#include <grpc++/grpc++.h>

#include "grpc/benchmark.grpc.pb.h"
#include "grpc/benchmark.pb.h"

#include "common.hpp"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

std::chrono::time_point<std::chrono::high_resolution_clock> rxStart, rxEnd;

std::array<int, 1000> timings;
int timeIdx = 0;

class BenchmarkClient
{
public:
    BenchmarkClient(std::shared_ptr<Channel> channel)
    : stub_(Benchmark::NewStub(channel)) {}

    void Stream()
    {
        StartMsg start;
        std::unique_ptr<grpc::ClientReader<DataMsg> > reader;
        ClientContext context;

        reader = stub_->Stream(&context, start);
        while(true)
        {
            rxStart = std::chrono::high_resolution_clock::now();
            DataMsg data;
            bool gotMessage = reader->Read(&data);
            rxEnd = std::chrono::high_resolution_clock::now();

            timings[timeIdx] = std::chrono::duration_cast<common::TimeUnit>(rxEnd - rxStart).count();
            ++timeIdx;
            
            if(timeIdx >= 1000)
            {
                std::cout << std::accumulate(timings.begin(), timings.end(), 0) / 1000 << std::endl;
                timeIdx = 0;
            }

            if(!gotMessage)
            {
                break;
            }
        }
    }

private:
    std::unique_ptr<Benchmark::Stub> stub_;
};

int main(int argc, char** argv)
{
    std::cout << "Client started" << std::endl;

    BenchmarkClient client(
        grpc::CreateChannel(
            common::serverAddress, 
            grpc::InsecureChannelCredentials()
        )
    );

    client.Stream();    

    return 0;
}