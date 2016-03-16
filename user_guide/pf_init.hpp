class PFInit
{
    public:
    std::size_t operator()(vsmc::Particle<PFState> &particle, void *param)
    {
        if (param != nullptr)
            particle.value().read_data(static_cast<const char *>(param));

        const double sd_pos0 = 2;
        const double sd_vel0 = 1;
        vsmc::NormalDistribution<double> norm_pos(0, sd_pos0);
        vsmc::NormalDistribution<double> norm_vel(0, sd_vel0);

        for (std::size_t i = 0; i != particle.size(); ++i) {
            auto sp = particle.sp(i);
            sp.state(PosX) = norm_pos(sp.rng());
            sp.state(PosY) = norm_pos(sp.rng());
            sp.state(VelX) = norm_vel(sp.rng());
            sp.state(VelY) = norm_vel(sp.rng());
            sp.state(LogL) = particle.value().log_likelihood(0, i);
        }

        w_.resize(particle.size());
        particle.value().read_state(LogL, w_.data());
        particle.weight().set_log(w_.data());

        return 0;
    }

    private:
    vsmc::Vector<double> w_;
};
