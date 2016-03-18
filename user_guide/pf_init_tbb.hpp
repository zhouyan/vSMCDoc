class PFInit : public vsmc::InitializeTBB<PFState, PFInit>
{
    public:
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
        sp.pos_x() = norm_pos(sp.rng());
        sp.pos_y() = norm_pos(sp.rng());
        sp.vel_x() = norm_vel(sp.rng());
        sp.vel_y() = norm_vel(sp.rng());
        w_[sp.id()] = sp.log_likelihood(0);

        return 0;
    }

    void eval_post(vsmc::Particle<PFState> &particle)
    {
        particle.weight().set_log(w_.data());
    }

    private:
    vsmc::Vector<double> w_;
};
