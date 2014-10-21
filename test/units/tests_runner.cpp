#include <thirdparty/catch/catch.hpp>

extern "C" int CallUnits(int argc, char* const argv[]);

int main(int argc, char* const argv[])
{
    return CallUnits(argc, argv);
}
