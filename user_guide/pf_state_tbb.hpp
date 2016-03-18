using PFStateBase = vsmc::StateMatrix<vsmc::RowMajor, 4, double>;

template <typename T>
using PFStateSPBase = PFStateBase::single_particle_type<T>;

class PFState : public PFStateBase
{
    public:
    using PFStateBase::StateMatrix;

    template <typename S>
    class single_particle_type : public PFStateSPBase
    {
        public:
        using PFStateSPBase::single_particle_type;

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

    void read_data(const char *file)
    {
        if (!file)
            return;

        constexpr std::size_t n = 100; // Number of data points
        obs_x_.resize(n);
        obs_y_.resize(n);
        std::ifstream data(file);
        for (std::size_t i = 0; i != n; ++i)
            data >> obs_x_[i] >> obs_y_[i];
        data.close();
    }

    private:
    vsmc::Vector<double> obs_x_;
    vsmc::Vector<double> obs_y_;
};
