class PFState : public vsmc::StateMatrix<vsmc::RowMajor, 4, double>
{
    public:
    using base = vsmc::StateMatrix<vsmc::RowMajor, 4, double>;

    PFState(base::size_type N) : base(N) {}

    template <typename S>
    class single_particle_type : public base::single_particle_type<S>
    {
        public:
        single_particle_type(std::size_t i, vsmc::Particle<T> *pptr)
            : base::single_particle_type<S>(i, pptr)
        {
        }

        double &pos_x() { return this->state(0); }
        double &pos_y() { return this->state(1); }
        double &vel_x() { return this->state(2); }
        double &vel_y() { return this->state(3); }

        double log_likelihood(std::size_t iter)
        {
            double llh_x = 10 * (pos_x() - obs_x());
            double llh_y = 10 * (pos_y() - obs_y());
            llh_x = std::log(1 + llh_x * llh_x / 10);
            llh_y = std::log(1 + llh_y * llh_y / 10);

            return -0.5 * (10 + 1) * (llh_x + llh_y);
        }

        private:
        double obs_x(std::size_t iter)
        {
            this->particle().value().obs_x_[iter];
        }

        double obs_y(std::size_t iter)
        {
            this->particle().value().obs_y_[iter];
        }
    }

    void read_data(const char *file) { /* same as before */ }

    private:
    vsmc::Vector<double> obs_x_;
    vsmc::Vector<double> obs_y_;
};
