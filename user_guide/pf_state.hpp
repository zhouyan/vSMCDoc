class PFState : public vsmc::StateMatrix<vsmc::RowMajor, 4, double>
{
    public:
    using base = vsmc::StateMatrix<vsmc::RowMajor, 4, double>;

    PFState(base::size_type N) : base(N) {}

    double log_likelihood(std::size_t iter, size_type i) const
    {
        double llh_x = 10 * (this->state(i, PosX) - obs_x_[iter]);
        double llh_y = 10 * (this->state(i, PosY) - obs_y_[iter]);
        llh_x = std::log(1 + llh_x * llh_x / 10);
        llh_y = std::log(1 + llh_y * llh_y / 10);

        return -0.5 * (10 + 1) * (llh_x + llh_y);
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
