#ifdef BASECFD_WITH_UNIT_TESTS
#define CATCH_CONFIG_RUNNER
#include <thirdparty/catch/catch.hpp>
extern "C" int CallUnits(int argc, char* const argv[])
{
    return Catch::Session().run(argc, argv);
}
#endif // BASECFD_WITH_UNIT_TESTS
