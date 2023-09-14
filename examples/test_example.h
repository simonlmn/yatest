
#include <yatest/TestSuite.h>
#include <yatest/Expect.h>
#include <yatest/Mocks.h>

static const yatest::TestSuite& ExampleTestSuite =
yatest::suite("Example")
  .tests("simple arithmetic", [] () {
    yatest::expect(1 + 1 == 2, "1 + 2 should equal 2");
  });
