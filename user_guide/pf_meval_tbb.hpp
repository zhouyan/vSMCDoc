class PFMEval : public vsmc::MonitorEvalTBB<PFState, PFMEval>
{
    public:
    void eval_sp(std::size_t iter, std::size_t dim,
        vsmc::SingleParticle<PFState> sp, double *r)
    {
        r[0] = sp.pos_x();
        r[1] = sp.pos_x();
    }
};
