#include "pf.hpp"

int main()
{
    constexpr std::size_t N = 1000; // Number of particles
    constexpr std::size_t n = 100;  // Number of data points

    vsmc::Sampler<PFState> sampler(N, vsmc::Multinomial, 0.5);

    sampler.init(PFInit()).move(PFMove(), false).monitor("pos", 2, PFMEval());
    sampler.monitor("pos").name(0) = "pos.x";
    sampler.monitor("pos").name(1) = "pos.y";

    sampler.initialize(const_cast<char *>("pf.data")).iterate(n - 1);

    std::ofstream output("pf.out");
    output << sampler << std::endl;
    output.close();

    return 0;
}
