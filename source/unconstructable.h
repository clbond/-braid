namespace braid {
  struct unconstructable {
    unconstructable() = delete;
    unconstructable(const unconstructable&) = delete;

    unconstructable& operator=(const unconstructable&) = delete;
  };
}