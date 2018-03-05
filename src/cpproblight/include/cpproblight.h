#ifndef CPPROBLIGHT_H
#define CPPROBLIGHT_H
#include <string>
#include "xtensor/xarray.hpp"
#include "xtensor/xio.hpp"
#include "pplprotocol.h"
#include <zmq.hpp>

#define VERSION "0.1.0"
#define GIT_BRANCH "master"
#define GIT_COMMIT_HASH "fd6b5a4"

namespace cpproblight
{
  namespace distributions
  {
    class Distribution
    {
    public:
      virtual xt::xarray<double> sample(const bool control, const bool replace, const std::string& address);
      virtual void observe(xt::xarray<double> value);
    };

    class Uniform: public Distribution
    {
    private:
      double low;
      double high;

    public:
      Uniform(double low=0, double high=1);
      xt::xarray<double> sample(const bool control, const bool replace, const std::string& address);
      void observe(xt::xarray<double> value);
    };

    class Normal: public Distribution
    {
    private:
      xt::xarray<double> mean;
      xt::xarray<double> stddev;

    public:
      Normal(xt::xarray<double> mean=xt::xarray<double> {0}, xt::xarray<double> stddev=xt::xarray<double> {1});
      xt::xarray<double> sample(const bool control, const bool replace, const std::string& address);
      void observe(xt::xarray<double> value);
    };
  }

  class Model
  {
  private:
    xt::xarray<double> (*modelFunction)(xt::xarray<double>);
    std::string serverAddress;
    std::string modelName;
    std::string systemName;

  public:
    Model(xt::xarray<double> (*modelFunction)(xt::xarray<double>), const std::string& modelName = "Unnamed cpproblight C++ model");
    void startServer(const std::string& serverAddress = "tcp://*:5555");
  };

  xt::xarray<double> sample(distributions::Distribution& distribution, const bool control=true, const bool replace=false, const std::string& address="");

  void observe(distributions::Distribution& distribution, xt::xarray<double> value);

  xt::xarray<double> ProtocolTensorToXTensor(const PPLProtocol::ProtocolTensor* protocolTensor);

  flatbuffers::Offset<PPLProtocol::ProtocolTensor> XTensorToProtocolTensor(flatbuffers::FlatBufferBuilder& builder, xt::xarray<double> xtensor);

  void sendMessage(flatbuffers::Offset<PPLProtocol::Message> message);

  std::string demangleAddress(std::string address);

  std::string extractAddress();
}

#endif
