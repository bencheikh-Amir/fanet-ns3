﻿#include <iostream>
#include <fstream>
#include <functional>
#include <numeric>

#include "ns3/core-module.h"
#include "ns3/node.h"
#include "ns3/node-container.h"
#include "ns3/csma-module.h"
#include "ns3/csma-star-helper.h"
#include "ns3/bridge-module.h"
#include "ns3/wifi-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/v4ping.h"
#include "ns3/v4ping-helper.h"
#include "ns3/packet-sink-helper.h"
#include "ns3/bulk-send-helper.h"
#include "ns3/mobility-helper.h"
#include "ns3/rectangle.h"
#include "ns3/animation-interface.h"
#include "ns3/point-to-point-helper.h"

#include "utils/tracers.h"
#include "fanetrouting.h"
#include "utils/script.h"
#include "time.h"

using namespace ns3;

class ScalarsByNodesTracer : public TracerBase
{
private:
  std::string m_cached_hdr;
  std::vector<std::string> m_lines;
public:
  static ns3::TypeId GetTypeId (void)
  {
    static ns3::TypeId tid = ns3::TypeId ("ScalarsByNodesTracer")
      .SetParent<TracerBase> ()
      .AddConstructor<ScalarsByNodesTracer> ();
    return tid;
  }

  ScalarsByNodesTracer()
  {
    std::string ss;
    m_cb_name_to_hdr_map.insert(std::make_pair("all", ss));
  }

  void AddNewExpResults(uint32_t n, const ExpResults& r)
  {
    std::string vals = std::to_string(n);
    std::string hdr = "nodes";

    for(const auto& it : r)
    {
       hdr += m_delimeter + it.first;
       vals += m_delimeter + it.second;
    }

    if(m_cached_hdr.length() == 0)
    {
      m_cached_hdr = hdr;
    }
    else
    {
      NS_ASSERT(m_cached_hdr == hdr);
    }

    m_lines.push_back(vals);

  }

  void Dump()
  {
    m_cb_out_map.at("all") << m_cached_hdr << std::endl;
    for(auto it : m_lines)
    {
      m_cb_out_map.at("all") << it << std::endl;
    }
  }
};

class ScalarsTracer : public TracerBase
{
private:
public:
  static ns3::TypeId GetTypeId (void)
  {
    static ns3::TypeId tid = ns3::TypeId ("ScalarsTracer")
      .SetParent<TracerBase> ()
      .AddConstructor<ScalarsTracer> ();
    return tid;
  }

  ScalarsTracer()
  {
    std::string ss;

    ss = "";
    m_cb_name_to_hdr_map.insert(std::make_pair("scalars", ss));
  }

  void Dump(const ExpResults& r)
  {
    m_cb_out_map.at("scalars").seekp(0);

    std::string hdr, vals;

    for(const auto& it : r)
    {
       hdr += it.first + m_delimeter;
       vals += it.second + m_delimeter;
    }
    hdr = hdr.substr(0, hdr.length() - m_delimeter.length());
    vals = vals.substr(0, vals.length() - m_delimeter.length());
    m_cb_out_map.at("scalars") << hdr << std::endl << vals;
  }
};

class ScalarsStatisticTracer : public TracerBase
{
private:
  std::vector<ExpResults> m_scalars;
  uint16_t m_params;
public:

  ExpResults m_mean, m_median, m_devation;

  static ns3::TypeId GetTypeId (void)
  {
    static ns3::TypeId tid = ns3::TypeId ("ScalarsStatisticTracer")
      .SetParent<TracerBase> ()
      .AddConstructor<ScalarsStatisticTracer> ();
    return tid;
  }

  ScalarsStatisticTracer()
  {
    std::string ss;

    m_cb_name_to_hdr_map.insert(std::make_pair("vector", ss));

    m_cb_name_to_hdr_map.insert(std::make_pair("stats", ss));
  }

  void Reserv(uint16_t num_of_tests)
  {
    m_scalars.reserve(num_of_tests);
  }

  void Add(const ExpResults& c)
  {
    if(m_scalars.empty())
    {
      m_params = c.size();
    }
    else
    {
      NS_ASSERT(c.size() == m_params);
    }
    m_scalars.push_back(c);
  }

  void Dump()
  {
    std::string hdr, vals;
    std::map<std::string, std::vector<double> > numeric_vals;
    std::vector<std::string> output_lines(m_scalars.size());

    //Check
    ExpResults first_line = m_scalars.front();
    for(const auto& kv : first_line)
    {
      std::string k = kv.first;
      bool add_to_vec = true;

      if(k.find("_str") != std::string::npos)
      {
        //this is str value
        add_to_vec = false;
      }
      else
      {
        std::vector<double> vec;
        vec.reserve(m_scalars.size());
        numeric_vals.insert(std::make_pair(k, vec));
      }

      hdr += k + m_delimeter;

      auto str_line = output_lines.begin();
      for(const auto& lines : m_scalars)
      {
        auto find_it = lines.find(k);
        NS_ASSERT(find_it != lines.end());

        str_line->append(find_it->second + m_delimeter);
        vals += find_it->second + m_delimeter;
        if(add_to_vec)
        {
          numeric_vals.at(find_it->first).push_back(std::stod(find_it->second));
        }
        str_line++;
      }
    }

    hdr = hdr.substr(0, hdr.length() - m_delimeter.length());
    m_cb_out_map.at("vector") << hdr
                              << std::endl;

    for(auto& s : output_lines)
    {
      m_cb_out_map.at("vector") << s.substr(0, s.length() - m_delimeter.length())
                                << std::endl;
    }


    hdr.clear();
    vals.clear();
    for(const auto& nums : numeric_vals)
    {
      double res;
      std::string str_val, new_name;
      //mean
      res = std::accumulate(nums.second.begin(), nums.second.end(), 0.0);
      res /= nums.second.size();
      new_name = nums.first;
      str_val = std::to_string(res);
      m_mean.insert(std::make_pair(new_name, str_val));
      hdr.append(new_name + m_delimeter);
      vals.append(str_val + m_delimeter);
      //median
      //deviation
    }
    hdr = hdr.substr(0, hdr.length() - m_delimeter.length());
    vals = vals.substr(0, vals.length() - m_delimeter.length());

    m_cb_out_map.at("stats") << hdr << std::endl
                             << vals
                             << std::endl;
  }
};

std::vector<uint32_t> CreateSeedsNum(uint32_t num_seeds)
{
  std::vector<uint32_t> ret;
  for(int i = 0; i < num_seeds; i++)
  {
    ret.push_back(i);
  }
  return ret;
}

int main(int argc, char **argv)
{

  //=====================================
  //LogComponentEnableAll (LogLevel::LOG_ALL);
  //=====================================
  CommandLine cmd;
  cmd.Usage ("FANET simulation program.\n");

  uint32_t num_nodes = 2;
  cmd.AddValue ("nodes",  "Number of nodes", num_nodes);

  std::string mobility = "RWP";
  cmd.AddValue ("mobility",  "Mobility model", mobility);

  std::string routing = "OLSR";
  cmd.AddValue ("routing", "Routing protocol", routing);

  std::string traffic = "L3ND;UDP_CBR";
  cmd.AddValue ("traffic-models",  "Traffic models", traffic);

  double total_time = 200.0;
  cmd.AddValue ("time", "Time to simulate", total_time);

  double speed = 200.0;
  cmd.AddValue ("speed", "Average speed of nodes", speed);

  uint32_t seed_num = 1;
  cmd.AddValue ("seed", "Seed number", seed_num);

  int num_of_tests = -1;
  cmd.AddValue ("tests", "Number of tests (seed arg will be ignored)", num_of_tests);

  bool calc_stats = false;
  cmd.AddValue ("calc", "Calculate stats", calc_stats);

  std::string out_dir = Script::GetEnv("PWD") + "/out";
  cmd.AddValue ("out-dir", "Output directory", out_dir);

  cmd.Parse (argc, argv);

  std::vector<uint32_t> seeds;

  if(num_of_tests != -1)
  {
    seeds = CreateSeedsNum(num_of_tests);
  }
  else
  {
    seeds.push_back(seed_num);
  }

  //Go to output dir
  if(Script::MkDir(out_dir) != 0)
  {
    exit(1);
  }
  std::string exp_name = mobility + "-" +
                         routing + "-" +
                         std::to_string(num_nodes) + "n-" +
                         std::to_string(speed) + "v";
  out_dir += "/" + exp_name;
  int r = Script::MkDir(out_dir);
  r |= Script::ChDir(out_dir);
  if(r)
  {
    exit(1);
  }

  std::cout << "Experiment: " << exp_name << std::endl;
  std::cout << "Go to " << out_dir << std::endl;
  //=====================================
  ScalarsStatisticTracer statistics;
  if(calc_stats)
  {
    statistics.CreateOutput("scalars.csv");
    statistics.Reserv(seed_num);
  }
  for(auto s : seeds)
  {
    std::cout << "\tSeed: " << s << std::endl;
    std::string seed_dir = out_dir + "/seed-" + std::to_string(s);
    Script::MkDir(seed_dir);
    Script::ChDir(seed_dir);

    ScalarsTracer scalar_of_one_impl;
    scalar_of_one_impl.CreateOutput("impl.csv");

    Ptr<FanetRoutingExperiment> exp = CreateObject<FanetRoutingExperiment>();

    exp->SetAttribute("nodes", UintegerValue(num_nodes));
    exp->SetAttribute("stream", UintegerValue(s));
    exp->SetAttribute("time", DoubleValue(total_time));
    exp->SetAttribute("mobility", StringValue(mobility));
    exp->SetAttribute("traffic", StringValue(traffic));
    exp->SetAttribute("routing", StringValue(routing));
    exp->SetAttribute("speed", DoubleValue(speed));
    exp->Simulate(argc, argv);

    const ExpResults& r = exp->GetSimulationResults();

    scalar_of_one_impl.Dump(r);

    if(calc_stats)
    {
      statistics.Add(r);
    }
  }

  if(calc_stats)
  {
    statistics.Dump();
  }
  std::cout << "Done with " << exp_name << "\n=============================" << std::endl;

  return 0;
}
