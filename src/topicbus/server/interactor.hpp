#ifndef SQUAWKBUS_TOPICBUS_SERVER_INTERACTOR_HPP
#define SQUAWKBUS_TOPICBUS_SERVER_INTERACTOR_HPP

namespace squawkbus::topicbus
{

  class Interactor
  {
  private:
    int fd_;

  public:
    Interactor(int fd);

    int fd() const noexcept { return fd_; }
  };

}

#endif // SQUAWKBUS_TOPICBUS_SERVER_INTERACTOR_HPP
