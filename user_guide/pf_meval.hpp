class PFMEval
{
    public:
    void operator()(std::size_t iter, std::size_t dim,
        vsmc::Particle<PFState> &particle, double *res)
    {
        for (std::size_t i = 0; i != particle.size(); ++i) {
            auto sp = particle.sp(i);
            *res++ = sp.state(PosX);
            *res++ = sp.state(PosY);
        }
    }
};
