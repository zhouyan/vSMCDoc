class PFMEval
{
    public:
    void operator()(std::size_t iter, std::size_t dim,
        vsmc::Particle<PFState> &particle, double *r)
    {
        eval_pre(iter, particle);
        for (std::size_t i = 0; i != particle.size(); ++i, r += dim)
            eval_sp(iter, dim, particle.sp(i), r);
        eval_post(iter, particle);
    }

    void eval_pre(std::size_t iter, vsmc::Particle<PFState> &particle) {}

    void eval_sp(std::size_t iter, std::size_t dim,
        vsmc::SingleParticle<PFState> sp, double *r)
    {
        r[0] = sp.state(PosX);
        r[1] = sp.state(PosY);
    }

    void eval_post(std::size_t iter, vsmc::Particle<PFState> &particle) {}
};
