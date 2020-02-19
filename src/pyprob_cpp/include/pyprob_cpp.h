#ifndef pyprob_cpp_H
#define pyprob_cpp_H
#include <string>
#include "xtensor/xarray.hpp"
#include "xtensor/xio.hpp"
#include "ppx_generated.h"
#include <zmq.hpp>
#include <random>

#define VERSION "0.1.11"
#define GIT_BRANCH "master"
#define GIT_COMMIT_HASH "450031c"

#define NONE_VALUE 17081023.17081023f

namespace pyprob_cpp
{
  extern std::default_random_engine generator;
  namespace distributions
  {
    class Distribution
    {
    public:
      virtual xt::xarray<double> sample(const bool control, const bool replace, const std::string& address, const std::string& name);
      virtual void observe(xt::xarray<double> value, const std::string& address, const std::string& name);
    };

    class Uniform: public Distribution
    {
    private:
      xt::xarray<double> low;
      xt::xarray<double> high;

    public:
      Uniform(xt::xarray<double> low=xt::xarray<double> {0}, xt::xarray<double> high=xt::xarray<double> {1});
      xt::xarray<double> sample(const bool control, const bool replace, const std::string& address, const std::string& name);
      void observe(xt::xarray<double> value, const std::string& address, const std::string& name);
    };

    class Normal: public Distribution
    {
    private:
      xt::xarray<double> mean;
      xt::xarray<double> stddev;

    public:
      Normal(xt::xarray<double> mean=xt::xarray<double> {0}, xt::xarray<double> stddev=xt::xarray<double> {1});
      xt::xarray<double> sample(const bool control, const bool replace, const std::string& address, const std::string& name);
      void observe(xt::xarray<double> value, const std::string& address, const std::string& name);
    };

    class Categorical: public Distribution
    {
    private:
      xt::xarray<double> probs;

    public:
      Categorical(xt::xarray<double> probs=xt::xarray<double> {1});
      xt::xarray<double> sample(const bool control, const bool replace, const std::string& address, const std::string& name);
      void observe(xt::xarray<double> value, const std::string& address, const std::string& name);
    };

    class Poisson: public Distribution
    {
    private:
      xt::xarray<double> rate;

    public:
      Poisson(xt::xarray<double> rate=0);
      xt::xarray<double> sample(const bool control, const bool replace, const std::string& address, const std::string& name);
      void observe(xt::xarray<double> value, const std::string& address, const std::string& name);
    };
  }

  class Model
  {
  private:
    xt::xarray<double> (*modelFunction)();
    std::string serverAddress;
    std::string modelName;
    std::string systemName;

  public:
    Model(xt::xarray<double> (*modelFunction)(), const std::string& modelName);
    void startServer(const std::string& serverAddress = "tcp://*:5555");
  };

  xt::xarray<double> sample(distributions::Distribution& distribution);
  xt::xarray<double> sample(distributions::Distribution& distribution, const std::string& name);
  xt::xarray<double> sample(distributions::Distribution& distribution, const bool control, const bool replace);
  xt::xarray<double> sample(distributions::Distribution& distribution, const bool control, const bool replace, const std::string& name);
  void observe(distributions::Distribution& distribution, xt::xarray<double> value);
  void observe(distributions::Distribution& distribution, const std::string& name);
  void observe(distributions::Distribution& distribution, xt::xarray<double> value, const std::string& name);
  void tag(xt::xarray<double> value);
  void tag(xt::xarray<double> value, const std::string& name);

  void setDefaultControl(bool control = true);
  void setDefaultReplace(bool replace = false);

  xt::xarray<double> TensorToXTensor(const ppx::Tensor* protocolTensor);

  flatbuffers::Offset<ppx::Tensor> XTensorToTensor(flatbuffers::FlatBufferBuilder& builder, xt::xarray<double> xtensor);

  void sendMessage(flatbuffers::Offset<ppx::Message> message);

  std::string demangleAddress(std::string address);

  std::string extractAddress();
}

#endif
