#ifndef COMMON_HPP
#define COMMON_HPP

#include <chrono>

/*
    https://groups.google.com/g/grpc-io/c/CyDeQEtmr_M
    https://stackoverflow.com/questions/50301690/how-to-maximize-grpc-throughput
    https://stackoverflow.com/questions/68866806/is-there-a-way-to-achieve-high-throughput-in-grpc

    https://docs.microsoft.com/en-us/aspnet/core/grpc/performance?view=aspnetcore-6.0

    https://developers.google.com/protocol-buffers/docs/reference/arenas
    https://developers.google.com/protocol-buffers/docs/techniques

    https://developers.google.com/protocol-buffers/docs/encoding?hl=en#order

*/


namespace common
{
    constexpr char* serverAddress = "localhost:50051"; 
    using TimeUnit = std::chrono::microseconds; 
}

#endif