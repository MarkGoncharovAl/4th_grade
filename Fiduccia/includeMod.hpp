#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <set>
#include <map>
#include <list>

class HGraph
{
  using Connections = std::vector<std::vector<unsigned>>;

  Connections Vertices;
  Connections Edges;

public:
  HGraph(std::ifstream &FIn);

  void dump(std::ostream &Out = std::cout) const;

  Connections const &getVertices() const { return Vertices; }
  Connections const &getEdges() const { return Edges; }
};

class Partitions
{
  std::vector<bool> VertPartitions;
  HGraph const &Graph;

  int Cost{0};
  int Side{0};

  void calculateCost();

public:
  void setCost(int CostIn) { Cost = CostIn; }
  int getCost() const { return Cost; }
  int getSide() const { return Side; }

  Partitions(HGraph const GraphIn);
  void dump(std::ostream &Out = std::cout) const;
  void out(std::ostream &Out = std::cout) const;
  void update(unsigned Vertex);

  std::vector<bool> const &getPart() const { return VertPartitions; }
};

class GainContainer
{

  using RLSide = std::map<int, std::list<int>>;
  using Move = std::pair<unsigned, int>;

  RLSide Left;
  RLSide Right;

  std::vector<int> VertGain;
  std::vector<bool> IsDeleted;
  std::vector<int> Deltas;
  std::vector<std::list<int>::iterator> Iterators;

  RLSide &getNeededSide(bool Side);
  RLSide const &getNeededSide(bool Side) const;

public:
  GainContainer(HGraph const &HG, Partitions const &Prt);
  bool isEmpty() const;
  Move bestFeasibleMove(int Displacement);
  void update(unsigned Vertex, bool Side, int Value);
  void erase(unsigned Vertex, bool Side);
  void updateDeleted(unsigned Vertex);
  std::vector<int> &getDeltas();
  std::vector<int> const &getDeltas() const;

  void dump(std::ostream &Out = std::cout) const;
};

unsigned Algorithm(HGraph const &, Partitions &);