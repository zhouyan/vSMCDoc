class PFMove
{
    public:
    std::size_t operator()(std::size_t iter, vsmc::Particle<PFState> &particle)
    {
        eval_pre(iter, particle);
        std::size_t acc = 0;
        for (std::size_t i = 0; i != particle.size(); ++i)
            acc += eval_sp(iter, particle.sp(i));
        eval_post(iter, particle);

        return 0;
    }

    void eval_pre(std::size_t iter, vsmc::Particle<PFState> &particle)
    {
        w_.resize(particle.size());
    }

    std::size_t eval_sp(std::size_t iter, vsmc::SingleParticle<PFState> sp)
    {
        vsmc::NormalDistribution<double> norm_pos(0, std::sqrt(0.02));
        vsmc::NormalDistribution<double> norm_vel(0, std::sqrt(0.001));
        sp.state(PosX) += norm_pos(sp.rng()) + 0.1 * sp.state(VelX);
        sp.state(PosY) += norm_pos(sp.rng()) + 0.1 * sp.state(VelY);
        sp.state(VelX) += norm_vel(sp.rng());
        sp.state(VelY) += norm_vel(sp.rng());
        w_[sp.id()] = sp.particle().value().log_likelihood(iter, sp.id());

        return 0;
    }

    void eval_post(std::size_t iter, vsmc::Particle<PFState> &particle)
    {
        particle.weight().add_log(w_.data());
    }

    private:
    vsmc::Vector<double> w_;
};
