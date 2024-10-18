#include "interactor.hpp"

#include <format>

#include "serialization/frame_buffer.hpp"
#include "serialization/frame_buffer_io.hpp"

namespace squawkbus::topicbus
{
  using squawkbus::io::Poller;
  using squawkbus::serialization::FrameBuffer;
  using squawkbus::topicbus::messages::Message;

  Interactor::Interactor(int fd, Poller& poller, const std::string& host, std::uint16_t port)
    : fd_(fd),
      host_(host),
      id_(std::format("{}:{}", host, port)),
      poller_(poller)
  {
  }

  void Interactor::send(std::shared_ptr<Message> message)
  {
    FrameBuffer frame;
    message->write(frame);
    auto buf = std::vector<char>(frame);
    poller_.write(fd_, buf);
  }

  void Interactor::receive(std::vector<char>&& buf)
  {
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