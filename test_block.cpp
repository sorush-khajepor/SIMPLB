#include "block.hh"

int main(int argc, char const *argv[]) {

    intND dim={16,16};
    boolND periodic={false,true};

    StructuredDecomposition  structuredDecomposition(dim,periodic);

    Block block(structuredDecomposition);

    return 0;
}
