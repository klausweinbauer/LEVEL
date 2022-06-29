#include <InMemoryDatabase.hpp>

namespace c2x::cam {

class Factory {
private:
  Factory(){};

public:
  ~Factory(){};

  static IDatabase &GetDatabase() {
    static auto instance =
        std::unique_ptr<InMemoryDatabase>(new InMemoryDatabase());
    return *instance;
  }
};

}; // namespace c2x::cam