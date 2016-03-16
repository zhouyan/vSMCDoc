class PFMove
{
    public:
    std::size_t operator()(std::size_t iter, vsmc::Particle<PFState> &particle)
    {
        const double sd_pos = std::sqrt(0.02);
        const double sd_vel = std::sqrt(0.001);
        const double delta = 0.1;
        vsmc::NormalDistribution<double> norm_pos(0, sd_pos);
        vsmc::NormalDistribution<double> norm_vel(0, sd_vel);

        for (std::size_t i = 0; i != particle.size(); ++i) {
            auto sp = particle.sp(i);
            sp.state(PosX) += norm_pos(sp.rng()) + delta * sp.state(VelX);
            sp.state(PosY) += norm_pos(sp.rng()) + delta * sp.state(VelY);
            sp.state(VelX) += norm_vel(sp.rng());
            sp.state(VelY) += norm_vel(sp.rng());
            sp.state(LogL) = particle.value().log_likelihood(iter, sp.id());
        }

        w_.resize(particle.size());
        particle.value().read_state(LogL, w_.data());
        particle.weight().add_log(w_.data());

        return 0;
    }

    private:
    vsmc::Vector<double> w_;
};
