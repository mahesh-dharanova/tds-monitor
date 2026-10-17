#include "filter.h"
#include <cassert>

int main() {
    TdsFilter filter;
    // Chunk 1 stub passes samples through unchanged. This assertion is
    // expected to be REWRITTEN in Chunk 2 once real smoothing lands.
    assert(filter.update(1.23f) == 1.23f);
    return 0;
}
