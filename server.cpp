#include <iostream>
#include <array>
#include <chrono>
#include <numeric>

#include <grpc++/grpc++.h>

#include "grpc/benchmark.grpc.pb.h"
#include "grpc/benchmark.pb.h"

#include "common.hpp"

using grpc::Channel;
using grpc::ServerContext;
using grpc::Status;

constexpr int TEST_ITER_COUNT = 10000;

// 1024 * 32 doubles -> buffer size of 32768

constexpr int BUFFER_SIZE = 32768;
std::array<double, BUFFER_SIZE> buffer = { 0 };

std::array<int, 1000> timings;
int timeIdx = 0;
std::chrono::time_point<std::chrono::high_resolution_clock> txStart, txEnd;

class BenchmarkServiceImpl final : public Benchmark::Service 
{
public:
    Status Stream(ServerContext* context, const StartMsg*, grpc::ServerWriter<DataMsg>* writer)
    {
        int j = 0;
        while(j < TEST_ITER_COUNT)
        {
            txStart = std::chrono::high_resolution_clock::now();
            DataMsg msg;
            for(int i = 0; i < BUFFER_SIZE; ++i)
            {
                msg.add_data(buffer[i]++);
            }
            writer->Write(msg);
            txEnd = std::chrono::high_resolution_clock::now();
            
            timings[timeIdx++] = std::chrono::duration_cast<common::TimeUnit>(txEnd - txStart).count();

            if(timeIdx >= 1000)
            {
                std::cout << std::accumulate(timings.begin(), timings.end(), 0) / 1000 << std::endl;
                timeIdx = 0;
            }
            ++j;
        }

        DataMsg lastMsg;
        grpc::WriteOptions opts;
        opts.set_last_message();
        writer->WriteLast(lastMsg, opts);

        return Status::OK;
    }

private:
};

int main(int argc, char** argv)
{
    std::cout << "Server started. Timings in (us)" << std::endl;

    BenchmarkServiceImpl service;
    grpc::ServerBuilder builder;
    builder.AddListeningPort(common::serverAddress, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<grpc::Server> server = builder.BuildAndStart();
    server->Wait();

    return 0;
}