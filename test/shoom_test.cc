
#include "lest.hpp"

using namespace std;

const lest::test specification[] = {
    CASE("shared memory can be created and opened") {

    },
};

int main (int argc, char *argv[]) {
  return lest::run(specification, argc, argv);
}
