template <typename T> class channel {

  public:
    channel() {};

    T recv() { return T(); }

    void send(T value) {}
};
