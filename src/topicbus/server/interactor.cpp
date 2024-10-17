#include "interactor.hpp"

namespace squawkbus::topicbus
{
  using squawkbus::topicbus::messages::Message;

  Interactor::Interactor(int fd)
    : fd_(fd)
  {
  }

  void Interactor::receive(std::vector<std::vector<char>>&& bufs)
  {
    for (auto &buf : bufs)
      reader_.write(buf);

    while (reader_.has_frame())
    {
      auto frame = reader_.read();
      auto message = Message::read(frame);
      process_message(message);
    }
  }

  void Interactor::process_message(std::shared_ptr<Message> message)
  {

  }

}