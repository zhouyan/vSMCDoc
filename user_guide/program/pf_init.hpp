class PFInit
{
    public:
    std::size_t operator()(vsmc::Particle<PFState> &particle, void *param)
    {
        eval_param(particle, param);
        eval_pre(particle);
        std::size_t acc = 0;
        for (std::size_t i = 0; i != particle.size(); ++i)
            acc += eval_sp(particle.sp(i));
        eval_post(particle);

        return acc;
    }

    void eval_param(vsmc::Particle<PFState> &particle, void *param)
    {
        particle.value().read_data(static_cast<const char *>(param));
    }

    void eval_pre(vsmc::Particle<PFState> &particle)
    {
        w_.resize(particle.size());
    }

    std::size_t eval_sp(vsmc::SingleParticle<PFState> sp)
    {
        vsmc::NormalDistribution<double> norm_pos(0, 2);
        vsmc::NormalDistribution<double> norm_vel(0, 1);
        sp.state(PosX) = norm_pos(sp.rng());
        sp.state(PosY) = norm_pos(sp.rng());
        sp.state(VelX) = norm_vel(sp.rng());
        sp.state(VelY) = norm_vel(sp.rng());
        w_[sp.id()] = sp.particle().value().log_likelihood(0, sp.id());

        return 0;
    }

    void eval_post(vsmc::Particle<PFState> &particle)
    {
        particle.weight().set_log(w_.data());
    }

    private:
    vsmc::Vector<double> w_;
};
