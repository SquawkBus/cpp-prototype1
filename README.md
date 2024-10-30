# cpp-prototype1

A C++ prototype for SquawkBus.

## Things to do

* Add authentication
  * Make an htpasswd file reader
  * Add a JSON or YAML loader for configuration
  * Reload config on signal.
* Remove regex from subscriptions, but leave for notifications.
* Make data packets have a single entitlement.
* Make task specific clients

## Container

I used ubuntu noble 24.04
I installed clang (18)
I installed
```bash
sudo apt update
sudo apt install clang
sudo apt install uuid-dev
sudo apt install libyaml-cpp-dev
sudo apt install gdb
# for htpasswd utility
sudo apt install apache2-utils
```
